// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "KnightEnemyAnimInstance.h"
#include "BossAnimInstance.h"
#include "BossAIController.h"
#include "MainHUDWidget.h"
#include "Sword.h"
#include "MainGameModeBase.h"
#include "TitleUserWidget.h"
// Sets default values
ABossCharacter::ABossCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//메시 설정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.0f), FRotator(0.f, -90.f, 0.f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHARMBASE(TEXT("SkeletalMesh'/Game/Resources/PlayerCharacter/GKnight/Meshes/SK_GothicKnight_VA.SK_GothicKnight_VA'"));
	if (SK_CHARMBASE.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CHARMBASE.Object);
	}
	//애니메이션 설정
	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM(TEXT("AnimBlueprint'/Game/Resources/PlayerCharacter/GKnight/Animation/Anim_Boss.Anim_Boss_C'"));
	if (ANIM.Succeeded())// 애니메이션 설정
	{
		GetMesh()->SetAnimInstanceClass(ANIM.Class);
	}

	
	
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("WidgetBlueprint'/Game/AMyDirectory/UI/UI_BossHP.UI_BossHP_C'"));
	if (UI_HUD.Succeeded())
	{
		MainHUDWidgetClass = UI_HUD.Class;
	}

	//AI
	AIControllerClass = ABossAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//speed
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
}

// Called when the game starts or when spawned
void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();
	//Sword
	Damage = 5.f;
	Sword = GetWorld()->SpawnActor<ASword>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != Sword)
	{
		Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("SwordSocket"));
		Sword->SetOwner(this);
	}
	//HP Widget
	HealthPoint = 1000.f;
	if (IsValid(MainHUDWidgetClass))
	{
		MainHUDWidget = Cast<UMainHUDWidget>(CreateWidget(GetWorld(), MainHUDWidgetClass));

		if (IsValid(MainHUDWidget))
		{
			// 위젯을 뷰포트에 띄우는 함수
			MainHUDWidget->AddToViewport();
		}
	}
}

// Called every frame
void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	EnemyAnim = Cast<UBossAnimInstance>(GetMesh()->GetAnimInstance());
	check(nullptr != EnemyAnim);

	EnemyAnim->OnMontageEnded.AddDynamic(this, &ABossCharacter::OnMontageEnded);
	EnemyAnim->OnAttackHitCheck.AddLambda([this]() -> void {
		AttackCheck(); 
	});
	EnemyAnim->OnNextAttackCheck.AddLambda([this]() -> void {
		EnemyAnim->JumpToAttackMontageSection(++ComboNum);
	});
	Cast<UBossAnimInstance>(EnemyAnim)->OnMoveToTarget.AddLambda([this]() -> void {
		FLatentActionInfo Info;
		Info.CallbackTarget = this;
		FVector Target = Cast<AActor>(Cast<AAIController>(GetController())->GetBlackboardComponent()->GetValueAsObject("Target"))->GetActorLocation();
		FVector AddVec = (Target - GetActorLocation());
		AddVec.Z = 0.f;
		AddVec.Normalize();
		UKismetSystemLibrary::MoveComponentTo(GetCapsuleComponent(), Target- AddVec*150.f,GetActorRotation(), false, false, 0.1f, false, EMoveComponentAction::Type::Move, Info);
	});
}

float ABossCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (HealthPoint <= 0.f)
	{
		EnemyAnim->SetDeadAnim();
		FTimerHandle WaitHandle;
		float WaitTime = 1.5f; 
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
			{
				AMainGameModeBase* GameMode = Cast<AMainGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
				GameMode->ClearWidget->AddToViewport();
				Cast<UTitleUserWidget>(GameMode->ClearWidget)->PlayFade();
			}), WaitTime, false);
		
	}
	MainHUDWidget->UpdateHPWidget(HealthPoint/10.f);
	return FinalDamage;
}

void ABossCharacter::Attack()
{
	if (IsAttacking) return;
	IsAttacking = true;
	//UE_LOG(LogTemp, Warning, TEXT("-----Is Boss Attack-----"));
	EnemyAnim->PlayAttackMontage();
}

void ABossCharacter::MediumSkill()
{
	Damage = 10.f;
	Cast<UBossAnimInstance>(EnemyAnim)->PalyMediumSkillMon();
}

void ABossCharacter::FarSkill()
{
	Damage = 20.f;
	Cast<UBossAnimInstance>(EnemyAnim)->PalyFarSkillMon();
}

void ABossCharacter::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	Super::OnMontageEnded(Montage, bInterrupted);
	ComboNum = 1;
	Damage = 5.f;
}
