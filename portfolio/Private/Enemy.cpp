// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "MainCharacter.h"
#include "KnightEnemyAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PhysicsActor"));

	this->Tags.Add(FName("Enemy"));
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 450.f;
	GetCharacterMovement()->AirControl = 0.1f;
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
	Damage = 1.f;
	
	//Widget
	TargetOn = CreateDefaultSubobject<UWidgetComponent>(TEXT("TARGETONWIDGET"));
	TargetOn->SetupAttachment(RootComponent);
	TargetOn->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	TargetOn->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> TargetOnWidget(TEXT("WidgetBlueprint'/Game/AMyDirectory/UI/UI_TargetOn.UI_TargetOn_C'"));
	if (TargetOnWidget.Succeeded())
	{
		TargetOn->SetWidgetClass(TargetOnWidget.Class);
	}
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	TargetOn->SetVisibility(false);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	EnemyAnim = Cast<UKnightEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	check(nullptr != EnemyAnim);

	EnemyAnim->OnMontageEnded.AddDynamic(this, &AEnemy::OnMontageEnded);
}



void AEnemy::AttackCheck()
{
	//�ݸ����� ������ hit actor����
	FHitResult HitResult;	//hit ������ ����
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 200.0f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(50.0f),
		Params);


#if ENABLE_DRAW_DEBUG	// DrawDebug�� ����� ������ ���� �ݸ����� �׷���

	FVector TraceVec = GetActorForwardVector() * 200.f;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = 200.f * 0.5f + 50.f;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

	/*DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		50.f,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);*/


#endif	//Damage ����
	if (bResult)
	{
		FDamageEvent DamageEvent;
		HitResult.Actor->TakeDamage(Damage, DamageEvent, GetController(), this);
	}
}

void AEnemy::EnableHPBar(bool isEnable)
{
	if (HPBarWidget == nullptr) return;
	HPBarWidget->SetVisibility(isEnable);
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (IsDead)
	{
		return FinalDamage;
	}

	Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this, 0))->SetManaPoint(5.f);
	if (Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this, 0))->HitParticle)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this, 0))->HitParticle, GetActorLocation());
	HealthPoint -= FinalDamage;
	if (HealthPoint <= 0.0f)
	{
		Dead();
	}
	return FinalDamage;
}
void AEnemy::Dead()
{
	EnemyAnim->SetDeadAnim();
	GetController()->UnPossess();
	SetActorEnableCollision(false);
	EnableHPBar(false);


	HealthPoint = 0.f;


	EnableTargetOnWidget(false);
	IsDead = true;
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	for (auto* Attached : AttachedActors)
	{
		Attached->Destroy();
	}
}

void AEnemy::EnableTargetOnWidget(bool IsEnable)
{
	TargetOn->SetVisibility(IsEnable);
}


void AEnemy::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if ((Montage->GetName()).Contains("Attack") && IsAttacking)
	{
		IsAttacking = false;
	}
}

