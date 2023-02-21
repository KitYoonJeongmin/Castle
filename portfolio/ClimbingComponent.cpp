// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbingComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "MainAnimInstance.h"

// Sets default values for this component's properties
UClimbingComponent::UClimbingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//Climbing
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Climbing_MONTAGE(TEXT("AnimMontage'/Game/AMyDirectory/Animations/ClimbingAnimations/Mon_MainHang_Idle.Mon_MainHang_Idle'"));
	if (Climbing_MONTAGE.Succeeded())
	{
		ClimbIdleMon = Climbing_MONTAGE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Climb_MONTAGE(TEXT("AnimMontage'/Game/AMyDirectory/Animations/ClimbingAnimations/MainClimb_Montage.MainClimb_Montage'"));
	if (Climb_MONTAGE.Succeeded())
	{
		ClimbMon = Climb_MONTAGE.Object;
	}
	
	LeftClimbArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("LeftClimbArrow"));
	LeftClimbArrow->SetRelativeLocation(FVector(60.0f, -60.0f, 40.0f));

	RightClimbArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("RightClimbArrow"));
	RightClimbArrow->SetRelativeLocation(FVector(60.0f, 60.0f, 40.0f));

	LeftLedgeArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("LeftLedgeArrow"));
	LeftLedgeArrow->SetRelativeLocation(FVector(60.0f, -150.0f, 40.0f));
	RightLedgeArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("RightLedgeArrow"));
	RightLedgeArrow->SetRelativeLocation(FVector(60.0f, 150.0f, 40.0f));

	UpArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("UpArrow"));
	UpArrow->SetRelativeLocation(FVector(65.0f, 0.0f, 200.0f));
}


void UClimbingComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	
}

// Called when the game starts
void UClimbingComponent::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<ACharacter>(GetOwner());
	if (Character != nullptr)
	{

		Cast<UMainAnimInstance>(Character->GetMesh()->GetAnimInstance())->OnClimbEnd.AddLambda([this]()->void {
			FLatentActionInfo Info;
			Info.CallbackTarget = this;
			UKismetSystemLibrary::MoveComponentTo(Character->GetCapsuleComponent(),
				Character->GetCapsuleComponent()->GetComponentLocation() + Character->GetActorForwardVector() * 50.f,
				UKismetMathLibrary::MakeRotFromX(Character->GetActorForwardVector()), false, false, 0.1f, false, EMoveComponentAction::Type::Move, Info);
			isClimbing = false;
			isFinish = false;
			Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			});

		LeftClimbArrow->AttachTo(Character->GetCapsuleComponent());
		RightClimbArrow->AttachTo(Character->GetCapsuleComponent());
		LeftLedgeArrow->AttachTo(Character->GetCapsuleComponent());
		RightLedgeArrow->AttachTo(Character->GetCapsuleComponent());
		UpArrow->AttachTo(Character->GetCapsuleComponent());
	}
}


// Called every frame
void UClimbingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CharLoc = Character->GetActorLocation();
	CharFor = Character->GetActorForwardVector();
	ForwardTrace();
	HeightTrace();
	JumpUpTrace();
	if (isClimbing)
	{
		LeftTrace();
		RightTrace();
		MoveInLedge();
		if (CanMoveLeft)
		{
			CanJumpLeft = false;
			CanTurnLeft = false;
		}
		else
		{
			JumpLeftTrace();
			//코너
			if (!CanJumpLeft) { TurnLeftTrace(); }
			else { CanTurnLeft = false; }
		}
		if (CanMoveRight)
		{
			CanJumpRight = false;
			CanTurnRight = false;
		}
		else
		{
			JumpRightTrace();
			//코너
			if (!CanJumpRight) { TurnRightTrace(); }
			else { CanTurnRight = false; }
		}
	}
	
}

void UClimbingComponent::ForwardTrace()
{
	if (Character == nullptr) return;
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);
	isInForward = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), CharLoc, (CharFor * 130.f) + CharLoc, 10.f, ETraceTypeQuery::TraceTypeQuery3,
		false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitResult, true);
	if (isInForward)
	{
		WallNormal = HitResult.Normal;
		WallLoc = HitResult.Location;
	}
	

	
}

void UClimbingComponent::HeightTrace()
{
	if (Character == nullptr) return;
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);
	isInHeight = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), CharLoc + FVector(0.f, 0.f, 500.f) + CharFor * 75.f, (CharFor * 75.f) + CharLoc, 10.f, ETraceTypeQuery::TraceTypeQuery3,
		false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitResult, true); 

	if (isInHeight)
	{
		HeightLoc = HitResult.ImpactPoint;
		float height = Character->GetMesh()->GetSocketLocation(FName("PelvisSocket")).Z - HeightLoc.Z;

		
		if (-50.f < height && height < -30.f)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Pelvis: %f Location : %f Height: %f"), Character->GetMesh()->GetSocketLocation(FName("PelvisSocket")).Z, HitResult.Location.Z, height);
			if (!isClimbing && !IsTurning)
			{
				Hang();
			}
		}
	}
}

void UClimbingComponent::Hang()
{
	isClimbing = true;
	//UE_LOG(LogTemp, Warning, TEXT("Pelvis: %f Location : %f"), Character->GetMesh()->GetSocketLocation(FName("PelvisSocket")).Z, HeightLoc.Z);
	FVector TargetLoc((WallNormal * 40.f + WallLoc).X, (WallNormal * 40.f + WallLoc).Y, HeightLoc.Z - Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 15.f);
	FRotator TargetRot = UKismetMathLibrary::MakeRotFromX(WallNormal * -1);
	FLatentActionInfo Info;
	Info.CallbackTarget = this;

	UKismetSystemLibrary::MoveComponentTo(Character->GetCapsuleComponent(), TargetLoc, TargetRot, false, false, 0.13f, false, EMoveComponentAction::Move, Info);

	Character->GetCharacterMovement()->DisableMovement();
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	Character->GetCharacterMovement()->StopMovementImmediately();
	
}
void UClimbingComponent::DropDown()
{
	if (!isClimbing) return;
	isClimbing = false;
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	Character->GetMesh()->GetAnimInstance()->StopAllMontages(0.2f);

}

void UClimbingComponent::ClimbUp()
{

	if (!isClimbing || isFinish) return;
	Character->GetMesh()->GetAnimInstance()->Montage_Play(ClimbMon,1.f);
	isFinish = true;

	
}

void UClimbingComponent::LeftTrace()
{
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);
	CanMoveLeft = UKismetSystemLibrary::CapsuleTraceSingle(GetWorld(), LeftClimbArrow->GetComponentLocation(), LeftClimbArrow->GetComponentLocation(), 30.f,60.f, ETraceTypeQuery::TraceTypeQuery3,
		false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitResult, true);

	
}

void UClimbingComponent::RightTrace()
{
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);
	CanMoveRight = UKismetSystemLibrary::CapsuleTraceSingle(GetWorld(), RightClimbArrow->GetComponentLocation(), RightClimbArrow->GetComponentLocation(), 30.f, 60.f, ETraceTypeQuery::TraceTypeQuery3,
		false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitResult, true);
}

void UClimbingComponent::MoveInLedge()
{
	if (CanMoveLeft)
	{
		if (Character->InputComponent->GetAxisValue((FName("MoveRight"))) < 0.f)
		{
			FVector LeftMoveVec = UKismetMathLibrary::VInterpTo(CharLoc, CharLoc + Character->GetActorRightVector() * -20, GetWorld()->DeltaTimeSeconds, 5);
			Character->SetActorLocation(LeftMoveVec,false);
			IsMovingLeft = true;
			IsMovingRight = false;
		}
		else
		{
			IsMovingLeft = false;
		}
	}
	if (CanMoveRight)
	{
		if (Character->InputComponent->GetAxisValue((FName("MoveRight"))) > 0.f)
		{
			FVector RightMoveVec = UKismetMathLibrary::VInterpTo(CharLoc, CharLoc + Character->GetActorRightVector() * 20, GetWorld()->DeltaTimeSeconds, 5);
			Character->SetActorLocation(RightMoveVec, false);
			IsMovingLeft = false;
			IsMovingRight = true;
		}
		else
		{
			IsMovingRight = false;
		}
	}

}

void UClimbingComponent::JumpLeftTrace()
{
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);
	if (UKismetSystemLibrary::CapsuleTraceSingle(GetWorld(), LeftLedgeArrow->GetComponentLocation(), LeftLedgeArrow->GetComponentLocation(), 30.f, 60.f, ETraceTypeQuery::TraceTypeQuery3,
		false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitResult, true))
	{
		CanJumpLeft = !CanMoveLeft;
	}
	else
	{
		CanJumpLeft = false;
	}
}

void UClimbingComponent::JumpRightTrace()
{
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);
	if (UKismetSystemLibrary::CapsuleTraceSingle(GetWorld(), RightLedgeArrow->GetComponentLocation(), RightLedgeArrow->GetComponentLocation(), 30.f, 60.f, ETraceTypeQuery::TraceTypeQuery3,
		false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitResult, true))
	{
		CanJumpRight = !CanMoveRight;

	}
	else
	{
		CanJumpRight = false;
	}
}

void UClimbingComponent::JumpLeftLedge()
{
	if (Character->InputComponent->GetAxisValue((FName("MoveRight"))) < 0.f)
	{
		if (!IsJumping)
		{
			Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
			Cast<UMainAnimInstance>(Character->GetMesh()->GetAnimInstance())->JumpLeft(IsJumping);


			IsJumping = true;
			isClimbing = true;

			FTimerHandle WaitHandle;
			float WaitTime = 0.8f; 
			GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
				{
					Hang();
				}), WaitTime, false); 
		}
	}

}

void UClimbingComponent::JumpRightLedge()
{
	if (Character->InputComponent->GetAxisValue((FName("MoveRight"))) > 0.f)
	{
		if (!IsJumping)
		{
			Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
			Cast<UMainAnimInstance>(Character->GetMesh()->GetAnimInstance())->JumpRight(IsJumping);

			IsJumping = true;
			isClimbing = true;

			FTimerHandle WaitHandle;
			float WaitTime = 0.8f;
			GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
				{
					Hang();
				}), WaitTime, false);
		}
	}
}

void UClimbingComponent::JumpLeft(bool isJumpLeft)
{
	IsJumpLeft = isJumpLeft;
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	Character->GetCharacterMovement()->StopMovementImmediately();
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	IsJumping = false;
}

void UClimbingComponent::JumpRight(bool isJumpRight)
{
	IsJumpRight = isJumpRight;
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	Character->GetCharacterMovement()->StopMovementImmediately();
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	IsJumping = false;

}

void UClimbingComponent::TurnLeftTrace()
{
	Character->GetCharacterMovement()->StopMovementImmediately();

	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);

	FVector StartVec(LeftClimbArrow->GetComponentLocation());
	StartVec.Z += 60.f;
	FVector EndVec = StartVec + LeftClimbArrow->GetForwardVector() * 70.f;
	if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartVec, EndVec, 30.f, ETraceTypeQuery::TraceTypeQuery3,
		false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitResult, true))
	{
		CanTurnLeft = false;
	}
	else
	{
		CanTurnLeft = true;
	}
}

void UClimbingComponent::TurnRightTrace()
{
	Character->GetCharacterMovement()->StopMovementImmediately();

	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);

	FVector StartVec(RightClimbArrow->GetComponentLocation());
	StartVec.Z += 60.f;
	FVector EndVec = StartVec + RightClimbArrow->GetForwardVector() * 70.f;
	if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartVec, EndVec, 30.f, ETraceTypeQuery::TraceTypeQuery3,
		false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitResult, true))
	{
		CanTurnRight = false;
	}
	else
	{
		CanTurnRight = true;
	}
}

void UClimbingComponent::JumpUpTrace()
{
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);
	if (UKismetSystemLibrary::CapsuleTraceSingle(GetWorld(), UpArrow->GetComponentLocation(), UpArrow->GetComponentLocation(), 30.f, 100.f, ETraceTypeQuery::TraceTypeQuery3,
		false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitResult, true))
	{
		CanJumpUp = true;
	}
	else
	{
		CanJumpUp = false;
	}
}

void UClimbingComponent::JumpUpLedge()
{
	if (Character->InputComponent->GetAxisValue((FName("MoveRight"))) == 0.f &&
		CanJumpUp && !IsJumping)
	{
		Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		Cast<UMainAnimInstance>(Character->GetMesh()->GetAnimInstance())->JumpUp(true);
		IsJumping = true;
		Character->DisableInput(Cast<APlayerController>(Character->GetController()));
	}
}

void UClimbingComponent::JumpUp(bool jumpUp)
{
	IsJumping = jumpUp;
	Character->GetCharacterMovement()->StopMovementImmediately();
	IsJumping = false;
	Hang();
	Character->EnableInput(Cast<APlayerController>(Character->GetController()));
}

