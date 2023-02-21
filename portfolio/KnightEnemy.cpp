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



// Sets default values
AKnightEnemy::AKnightEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	//현재 tick 함수에서 진행되는 과정이 없으니 false로 변경

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PhysicsActor"));



	//메시 설정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.0f), FRotator(0.f, -90.f, 0.f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHARMBASE(TEXT("SkeletalMesh'/Game/AMyDirectory/Enemy/Meshes/T-Pose.T-Pose'"));
	if (SK_CHARMBASE.Succeeded()) { GetMesh()->SetSkeletalMesh(SK_CHARMBASE.Object); }
	//애니메이션 설정
	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM(TEXT("AnimBlueprint'/Game/AMyDirectory/Enemy/Animations/BP_KnightAnim.BP_KnightAnim_C'"));
	if (ANIM.Succeeded())// 애니메이션 설정
	{
		GetMesh()->SetAnimInstanceClass(ANIM.Class);
	}
	//AI 설정
	AIControllerClass = AMainEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;




	//캐릭터 움직임 구성
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 450.f;
	GetCharacterMovement()->AirControl = 0.1f;
	GetCharacterMovement()->MaxWalkSpeed = 200.f;

	IsWeapon = false;

	//공격
	IsAttacking = false;
	IsBlock = false;
	AttackEndComboState();

	//HP
	HealthPoint = 100.f;
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	HPBarWidget->SetupAttachment(RootComponent);

	HPBarWidget->SetRelativeLocation(FVector(0, 0, 120.f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("WidgetBlueprint'/Game/AMyDirectory/UI/UI_EnemyHP.UI_EnemyHP_C'"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(100,15));
	}
	
}

// Called when the game starts or when spawned
void AKnightEnemy::BeginPlay()
{
	Super::BeginPlay();
	Sword = GetWorld()->SpawnActor<ASword>(FVector::ZeroVector, FRotator::ZeroRotator);
	SetWeapon(Sword, TEXT("sword_lSocket"));
	
	Cast<UMainHUDWidget>(HPBarWidget->GetUserWidgetObject())->UpdateHPWidget(HealthPoint);
}

// Called every frame
void AKnightEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AKnightEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AKnightEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *(GetMesh()->GetAnimInstance()->GetName()));
	EnemyAnim = Cast<UKnightEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	check(nullptr != EnemyAnim);

	EnemyAnim->OnMontageEnded.AddDynamic(this, &AKnightEnemy::OnAttackMontageEnded);
	EnemyAnim->OnNextAttackCheck.AddLambda([this]() -> void {
		//UE_LOG(LogTemp, Warning, TEXT("OnNextAttackCheck"));
		CanNextCombo = false;

		if (IsComboInputOn)
		{
			AttackStartComboState();
			//UE_LOG(LogTemp, Warning, TEXT("Combo: %d"), CurrentCombo);
			EnemyAnim->JumpToAttackMontageSection(CurrentCombo);
		}
		});
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
			Cast<AMainEnemyAIController>(GetController())->StartAI();
		}});
	EnemyAnim->OnStopAI.AddLambda([this]()->void {
		if (GetController() != nullptr)
		{
			Cast<AMainEnemyAIController>(GetController())->StopAI();
			GetCharacterMovement()->DisableMovement();
		}});
}

void AKnightEnemy::Attack()
{
	if (!IsWeapon)
	{
		return;
	}

	//공격
	if (IsAttacking)
	{
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		check(CurrentCombo == 0);
		IsAttacking = true;
		AttackStartComboState();

		EnemyAnim->PlayAttackMontage();
	}
}

void AKnightEnemy::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	CurrentCombo += 1;
}

void AKnightEnemy::AttackEndComboState()
{
	CanNextCombo = false;
	IsComboInputOn = false;
	CurrentCombo = 0;
}

void AKnightEnemy::AttackCheck()
{
	//콜리전을 생성해 hit actor구분
	FHitResult HitResult;	//hit 정보를 담음
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 200.0f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(50.0f),
		Params);


#if ENABLE_DRAW_DEBUG	// DrawDebug를 사용해 위에서 만든 콜리전을 그려줌

	FVector TraceVec = GetActorForwardVector() * 200.f;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = 200.f * 0.5f + 50.f;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		50.f,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);


#endif	//Damage 전달
	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());
		}

		FDamageEvent DamageEvent;
		HitResult.Actor->TakeDamage(50.0f, DamageEvent, GetController(), this);
	}
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

void AKnightEnemy::Block()
{
	if (!IsWeapon) return;
	IsBlock = !IsBlock;
	EnemyAnim->PlaySwordBlockMontage(IsBlock);
}

void AKnightEnemy::PlayAssassination()
{
	EnemyAnim->PlayAssassinationAnim();
}

void AKnightEnemy::DisableHPBar()
{
	HPBarWidget->SetVisibility(false);
}

void AKnightEnemy::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if ((Montage->GetName()).Contains("Attack") &&IsAttacking)
	{
		check(CurrentCombo > 0);
		IsAttacking = false;
		AttackEndComboState();
		OnAttackEnd.Broadcast();
	}
}

float AKnightEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	HealthPoint -= FinalDamage;
	Cast<AMainEnemyAIController>(GetController())->StopAI();
	if (HealthPoint <= 0.0f)
	{
		EnemyAnim->SetDeadAnim();
		GetController()->UnPossess();
		SetActorEnableCollision(false);
		DisableHPBar();
	}
	else 
	{ 
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
		
	}
	Cast<UMainHUDWidget>(HPBarWidget->GetUserWidgetObject())->UpdateHPWidget(HealthPoint);
	Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this,0))->SetManaPoint(5.f);
	return FinalDamage;
}

