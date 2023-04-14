// Fill out your copyright notice in the Description page of Project Settings.


#include "RidingHorse.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Components/WidgetComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathVectorCommon.h"
#include "MainCharacter.h"
#include "MainAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARidingHorse::ARidingHorse()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//�޽� ����
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.0f), FRotator(0.f, -90.f, 0.f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHARMBASE(TEXT("SkeletalMesh'/Game/HorseRidingSystem/Horse/Horse_Realistic.Horse_Realistic'"));
	if (SK_CHARMBASE.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CHARMBASE.Object);
	}
	//�ִϸ��̼� ����
	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM(TEXT("AnimBlueprint'/Game/AMyDirectory/RindingHorseSystem/Horse/Anim_Horse.Anim_Horse_C'"));
	if (ANIM.Succeeded())// �ִϸ��̼� ����
	{
		GetMesh()->SetAnimInstanceClass(ANIM.Class);
	}
	// SpringArm
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 450.f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bDoCollisionTest = false;

	// Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	

	// Box Collision
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetRelativeLocation(FVector(25.f, 0.f, 0.f));
	BoxComponent->SetBoxExtent(FVector(95.f, 95.f, 62.f));

	//mount
	MountPos = CreateDefaultSubobject<UActorComponent>(TEXT("RiderPos"));

	//Movement
	GetCharacterMovement()->MaxWalkSpeed = 1200.f;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	
	//ī�޶󿡸� ȸ���� ����
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//Widget
	InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("INTERACTWIDGET"));
	InteractWidget->SetupAttachment(RootComponent);
	InteractWidget->SetDrawAtDesiredSize(true);
	InteractWidget->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	InteractWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> INTERACT_HUD(TEXT("WidgetBlueprint'/Game/AMyDirectory/UI/UI_Interactiion.UI_Interactiion_C'"));
	if (INTERACT_HUD.Succeeded())
	{
		InteractWidget->SetWidgetClass(INTERACT_HUD.Class);
		InteractWidget->SetDrawSize(FVector2D(1266.f, 636.f));
	}
}

// Called when the game starts or when spawned
void ARidingHorse::BeginPlay()
{
	Super::BeginPlay();
	InteractWidget->SetVisibility(false);
}

// Called every frame
void ARidingHorse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//ForwardSpeed
	float Target = FVector::DotProduct(GetVelocity(), GetActorForwardVector());
	float Alpha;
	if (ForwardSpeed >= Target) Alpha = 0.1f;
	else Alpha = 0.5f;

	ForwardSpeed = FMath::Lerp(ForwardSpeed, Target, Alpha);

	//RightSpeed
	Target = FVector::DotProduct(GetVelocity(), GetActorRightVector());
	RightSpeed = FMath::Lerp(RightSpeed, Target, 0.1f);

	//Velocity
	Velocity = FMath::Lerp(Velocity, TargetVel, 0.01f);

}

// Called to bind functionality to input
void ARidingHorse::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ARidingHorse::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARidingHorse::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ARidingHorse::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ARidingHorse::LookUp);

	//Interact
	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &ARidingHorse::Dismount);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ARidingHorse::SetRunSpeed);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ARidingHorse::SetWalkSpeed);
}

void ARidingHorse::Activate_Implementation()
{
	Mount();
}

void ARidingHorse::SetWidget_Implementation(bool IsVisible)
{
	if (InteractWidget == nullptr) return;

	InteractWidget->SetVisibility(IsVisible);
}

void ARidingHorse::MoveForward(float Value)
{
	if (Controller == nullptr) return;
	if (Value < 0.f) return;
	FRotator CameraRot = FollowCamera->GetComponentRotation();
	CameraRot.Yaw += RightValue * 25.f;
	FVector Direction = FRotationMatrix(CameraRot).GetScaledAxis(EAxis::X);
	Direction.Z = 0.0f;
	Direction.Normalize();

	AddMovementInput(Direction, Value * 0.4f* Velocity);
	ForwardValue = Value;
	if (Value != 0.f)
	{
		FRotator ActorRot = GetActorRotation();
		ActorRot.Roll = 0.f; ActorRot.Pitch = 0.f;
		FQuat ActorQuat = ActorRot.Quaternion();
		FQuat CameraQuat = CameraRot.Quaternion();
		NextForRot = FQuat::Slerp(ActorQuat, CameraQuat, 0.005f* TargetVel).Rotator();
		SetActorRotation(NextForRot);
	}
} 

void ARidingHorse::MoveRight(float Value)
{
	if (Controller == nullptr) return;

	RightValue = Value;
	FRotator ActorRot = GetActorRotation();


	FVector DirectionX = FRotationMatrix(GetActorRotation()).GetScaledAxis(EAxis::X);
	DirectionX.Z = 0.0f;
	DirectionX.Normalize();
	if(ForwardValue==0.f)
		AddMovementInput(DirectionX,abs(Value)*  0.4f * Velocity);
	if (Value != 0.f)
	{
		ActorRot.Roll = 0.f; ActorRot.Pitch = 0.f;
		ActorRot.Yaw += Value*0.4f;
		SetActorRotation(ActorRot);
	}
	
}

void ARidingHorse::Turn(float Rate)
{

	AddControllerYawInput(Rate * 45.f * GetWorld()->GetDeltaSeconds());
		
}

void ARidingHorse::LookUp(float Rate)
{
	AddControllerPitchInput(Rate * 45.f * GetWorld()->GetDeltaSeconds());
}

void ARidingHorse::Mount()
{
	AMainCharacter* Character = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	Character->SetActorRotation(this->GetActorRotation());
	FVector ToHorseVec = this->GetActorLocation() - Character->GetActorLocation();
	Character->MainAnim->MountR = FVector::DotProduct(ToHorseVec, Character->GetActorRightVector());
	Character->MainAnim->MountF = FVector::DotProduct(ToHorseVec, Character->GetActorForwardVector());
	Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	FVector CharLoc(Character->GetActorLocation());
	CharLoc.Z = this->GetActorLocation().Z;
	Character->SetActorLocation(CharLoc);
	FVector TargetLoc = Character->GetActorLocation() + Character->MainAnim->MountF * Character->GetActorForwardVector() + Character->MainAnim->MountR * Character->GetActorRightVector() + Character->GetActorUpVector() * 20.f;
	Character->SetActorLocation(TargetLoc);

	Character->MainAnim->CanMount = true;
	//Character->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform, "SeatSocket");
}

void ARidingHorse::Dismount()
{
	if (GetVelocity().Size() != 0.f) return;
	TArray<AActor*> AttachedActor;
	this->GetAttachedActors(AttachedActor);
	for (auto actor : AttachedActor)
	{
		if (Cast<AMainCharacter>(actor))
		{
			//Cast<AMainCharacter>(actor)->MainAnim->CanMount = false;
			Cast<AMainCharacter>(actor)->MainAnim->IsMount = false;
			Controller->Possess(Cast<APawn>(actor));
			DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			break;
		}
	}
}

void ARidingHorse::SetRunSpeed()
{
	TargetVel = 4.f;
}

void ARidingHorse::SetWalkSpeed()
{
	TargetVel = 1.f;
}

