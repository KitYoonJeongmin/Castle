 // Fill out your copyright notice in the Description page of Project Settings.


#include "KnightEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Sword.h"
#include "MainCharacter.h"
#include "MainEnemyAIController.h"
#include "KnightEnemyAnimInstance.h"
#include "MainHUDWidget.h"
#include "Spline.h"
#include "ItemGen.h"

// Sets default values
AKnightEnemy::AKnightEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	//���� tick �Լ����� ����Ǵ� ������ ������ false�� ����


	//�޽� ����
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.0f), FRotator(0.f, -90.f, 0.f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHARMBASE(TEXT("SkeletalMesh'/Game/AMyDirectory/Enemy/Meshes/T-Pose.T-Pose'"));
	if (SK_CHARMBASE.Succeeded()) { GetMesh()->SetSkeletalMesh(SK_CHARMBASE.Object); }
	//�ִϸ��̼� ����
	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM(TEXT("AnimBlueprint'/Game/AMyDirectory/Enemy/Animations/BP_KnightAnim.BP_KnightAnim_C'"));
	if (ANIM.Succeeded())// �ִϸ��̼� ����
	{
		GetMesh()->SetAnimInstanceClass(ANIM.Class);
	}
	GetMesh()->bRenderCustomDepth = true;
	GetMesh()->CustomDepthStencilValue = 1;
	//AI ����
	AIControllerClass = AMainEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;




	IsWeapon = false;

	//����
	IsAttacking = false;


	//HP
	HealthPoint = 100.f;
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	HPBarWidget->SetupAttachment(RootComponent);
	HPBarWidget->SetRelativeLocation(FVector(0, 0, 120.f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);

	DetectWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("DETECTBARWIDGET"));
	DetectWidget->SetupAttachment(HPBarWidget);
	DetectWidget->SetRelativeLocation(FVector(0, 0, 10.f));
	DetectWidget->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("WidgetBlueprint'/Game/AMyDirectory/UI/UI_EnemyHP.UI_EnemyHP_C'"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(100,15));
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> Detect_HUD(TEXT("WidgetBlueprint'/Game/AMyDirectory/UI/UI_EnemyDetect.UI_EnemyDetect_C'"));
	if (Detect_HUD.Succeeded())
	{
		DetectWidget->SetWidgetClass(Detect_HUD.Class);
		DetectWidget->SetDrawSize(FVector2D(100, 15)); 
	}
	
	
	
}

// Called when the game starts or when spawned
void AKnightEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	//Patrolling
	TArray<AActor*> ChildActor;
	GetAttachedActors(ChildActor);
	for (auto child : ChildActor)
	{
		if (Cast<ASpline>(child))
		{
			PatrolPath = Cast<ASpline>(child);
			break;
		}
	}
	if (PatrolPath != nullptr)
		PatrolPath->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	//Sword
	Sword = GetWorld()->SpawnActor<ASword>(FVector::ZeroVector, FRotator::ZeroRotator);
	SetWeapon(Sword, TEXT("sword_lSocket"));

	//Widgets
	HPBarWidget->InitWidget();
	DetectWidget->InitWidget();
	Cast<UMainHUDWidget>(HPBarWidget->GetUserWidgetObject())->UpdateHPWidget(HealthPoint);
	Cast<UMainHUDWidget>(DetectWidget->GetUserWidgetObject())->UpdateHPWidget(HealthPoint);
	EnableHPBar(false);
	EnableDetectBar(false);
	
	//Crowd(RVO)Movement
	//GetCharacterMovement()->bUseRVOAvoidance = true;
	//GetCharacterMovement()->AvoidanceConsiderationRadius = 350.f;
	
}

// Called every frame
void AKnightEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AKnightEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	EnemyAnim->OnAttackHitCheck.AddLambda([this]() -> void {
		AttackCheck();

		});
	EnemyAnim->OnEQUIP.AddLambda([this]() -> void {
		SetWeapon(Sword, TEXT("hand_rSocket"));
		});
	EnemyAnim->Onsheath.AddLambda([this]() -> void {
		SetWeapon(Sword, TEXT("sword_lSocket"));
		});
	EnemyAnim->OnRunAI.AddLambda([this]()->void {
		if (GetController() != nullptr)
		{
			Cast<AMainEnemyAIController>(GetController())->SetStun(false);
		}});

}

void AKnightEnemy::Attack()
{
	if (!IsWeapon || IsAttacking)
	{
		return;
	}
	IsAttacking = true;
	EnemyAnim->PlayAttackMontage();
}


void AKnightEnemy::SetWeapon(AActor* NewWeapon, FName WeaponSocket)
{
	if (nullptr != NewWeapon)
	{
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, WeaponSocket);
		NewWeapon->SetOwner(this);
	}
}

void AKnightEnemy::UseSword()
{
	EnemyAnim->PlayDrawSwordMontage();
	if (IsWeapon)
	{
		EnemyAnim->JumpToSwordMontageSection(2);
		IsWeapon = false;
	}
	else
	{
		IsWeapon = true;
	}
}

void AKnightEnemy::PlayAssassination()
{
	EnemyAnim->PlayAssassinationAnim();
	GetController()->UnPossess();
	SetActorEnableCollision(false);
}


void AKnightEnemy::EnableDetectBar(bool isEnable)
{
	DetectWidget->SetVisibility(isEnable);
}

void AKnightEnemy::UpdateDetectBar(float DetectLevel)
{
	Cast<UMainHUDWidget>(DetectWidget->GetUserWidgetObject())->UpdateHPWidget(DetectLevel);
}

FVector AKnightEnemy::GetNextLocaiton()
{
	if (PatrolPath == nullptr) 
	{
		PatrolCheckPoint = false;
		return this->GetActorLocation();
	}
	
	if (PointIndex == 0)
	{
		AddIndex = 1;
		PatrolCheckPoint = true;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("First Point"));
	}
	else if (PointIndex == Cast<USplineComponent>(PatrolPath->GetRootComponent())->GetNumberOfSplinePoints() - 1)
	{
		AddIndex = -1;
		PatrolCheckPoint = true;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Last Point"));
	}
	else
	{
		PatrolCheckPoint = false;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Patrol"));
	}
	PointIndex += AddIndex;
	return Cast<USplineComponent>(PatrolPath->GetRootComponent())->GetLocationAtSplinePoint(PointIndex, ESplineCoordinateSpace::World);
}

float AKnightEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (IsDead)
	{
		return FinalDamage;
	}
	if(!HPBarWidget->IsActive())
		EnableHPBar(true);
	Cast<UMainHUDWidget>(HPBarWidget->GetUserWidgetObject())->UpdateHPWidget(HealthPoint);
	
	Cast<AMainEnemyAIController>(GetController())->SetStun(true);
	if (DamageAmount == 12.f)
	{
		EnemyAnim->PlayNuckMontage();
	}
	else
	{
		FLatentActionInfo Info;
		Info.CallbackTarget = this;
		UKismetSystemLibrary::MoveComponentTo(GetCapsuleComponent(), GetActorLocation() + DamageCauser->GetActorForwardVector() * 50.f, GetActorRotation(), false, false, 0.3f, false, EMoveComponentAction::Type::Move, Info);
		EnemyAnim->PlayPainMontage();
	}
	
	return FinalDamage;
}

void AKnightEnemy::Dead()
{
	Super::Dead();
	EnableDetectBar(false);
	FActorSpawnParameters SpawnParams;
	GetWorld()->SpawnActor<AActor>(AItemGen::StaticClass(), GetActorLocation(), GetActorRotation(), SpawnParams);
}


