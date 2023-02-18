// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "KnightEnemy.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/Classes/Components/SphereComponent.h"
#include "Engine/Classes/GameFramework/ProjectileMovementComponent.h"
// Sets default values
AArrow::AArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Arrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ARROW"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ArrowStaticMesh(TEXT("StaticMesh'/Game/AMyDirectory/ArrowBow/Arrow/arrowyfbx.arrowyfbx'"));
	if (ArrowStaticMesh.Succeeded())
	{
		DefaultArrowMesh = ArrowStaticMesh.Object;
		Arrow->SetStaticMesh(DefaultArrowMesh);
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> LightArrowStaticMesh(TEXT("StaticMesh'/Game/AMyDirectory/ArrowBow/Arrow/lightarrowyfbx.lightarrowyfbx'"));
	if (LightArrowStaticMesh.Succeeded())
	{
		LightArrowMesh = LightArrowStaticMesh.Object;
	}
	
	Arrow->SetEnableGravity(true); 
	Arrow->SetSimulatePhysics(false);
	Arrow->SetCollisionProfileName(TEXT("NoCollision"));
	RootComponent = Arrow;
	

	//파티클
	static ConstructorHelpers::FObjectFinder<UParticleSystem> Fire(TEXT("ParticleSystem'/Game/AMyDirectory/Materials/P_Buff_JumpDiveFire_02.P_Buff_JumpDiveFire_02'"));
	if (Fire.Succeeded())
	{
		FireParticle = Fire.Object;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem> Poision(TEXT("ParticleSystem'/Game/AMyDirectory/Materials/P_Poision.P_Poision'"));
	if (Poision.Succeeded())
	{
		PoisionParticle = Poision.Object;
	}
	//머티리얼
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("Material'/Game/AMyDirectory/Materials/M_ArrowSkill1.M_ArrowSkill1'"));
	if (MaterialAsset.Succeeded())
	{
		LightingArrowMat = MaterialAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterial> DefaultMaterialAsset(TEXT("Material'/Game/AMyDirectory/ArrowBow/Arrow/M_Arrow.M_Arrow'"));
	if (DefaultMaterialAsset.Succeeded())
	{
		DefaultArrowMat = DefaultMaterialAsset.Object;
	}


	//독
	PoisionTime = 10.f;	//10초
	
}

// Called when the game starts or when spawned
void AArrow::BeginPlay()
{
	Super::BeginPlay();
	//SetLightMat(false);
	
	// 겹침 델리게이트 함수 등록
	Arrow->OnComponentBeginOverlap.AddDynamic(this, &AArrow::ArrowBeginOverlap);
	
}

void AArrow::ArrowBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//이름 출력(디버깅)
	UE_LOG(LogTemp, Warning, TEXT("Arrow Hit Actor: %s"), *OtherActor->GetName());
	
	//충돌감지 비활성화
	Arrow->SetSimulatePhysics(false);
	Arrow->SetNotifyRigidBodyCollision(false);
	Arrow->SetCollisionProfileName(TEXT("NoCollision"));
	//SetActorLocation(GetActorLocation());
	
	//
	this->AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
	//enemy라면 데미지 전달
	if (Cast<AKnightEnemy>(OtherActor) != nullptr)
	{
		FDamageEvent DamageEvent;
		switch (CurrentState)
		{
		case EArrowState::Default:
			OtherActor->TakeDamage(10.f, DamageEvent, GetWorld()->GetFirstPlayerController(), this);
			break;
		case EArrowState::Fire:
			OtherActor->TakeDamage(15.f, DamageEvent, GetWorld()->GetFirstPlayerController(), this);
			break;
		case EArrowState::Poision:
			OtherActor->TakeDamage(10.f, DamageEvent, GetWorld()->GetFirstPlayerController(), this);
			PoisionActor = OtherActor;
			break;
		}
		
	}

}


// Called every frame
void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//독 지속 공격
	if (CurrentState == EArrowState::Poision && PoisionActor != nullptr)
	{
		RemainTime -= DeltaTime;
		if (RemainTime <= 0.f)
		{
			RemainTime = PoisionTime;
			FDamageEvent DamageEvent;
			PoisionActor->TakeDamage(3.f, DamageEvent, GetWorld()->GetFirstPlayerController(), this);
		}
	}
}

void AArrow::Fire(AActor* Target, FVector ForwardVec)
{
	//콜리전 설정
	Arrow->SetSimulatePhysics(true);
	Arrow->SetNotifyRigidBodyCollision(true);
	Arrow->SetCollisionProfileName(TEXT("Arrow"));

	//발사 관련 변수 설정
	FVector startLoc = GetActorLocation();      // 발사 지점
	FVector targetLoc; // 타겟 지점.

	if (Target == nullptr)
	{
		
		targetLoc = startLoc + ForwardVec * 9999.f;
	}
	else
	{
		//디버그
		//UE_LOG(LogTemp, Warning, TEXT("Arrow Target Actor: %s"), *HitResult.Actor->GetName());
		targetLoc = Target->GetActorLocation();
	}
	
	//포물선 발사
	float arcValue = 0.993f;                       // ArcParam (0.0-1.0)
	FVector outVelocity = FVector::ZeroVector;   // 결과 Velocity
	if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, outVelocity, startLoc, targetLoc, GetWorld()->GetGravityZ(), arcValue))
	{
		//FPredictProjectilePathParams predictParams(20.0f, startLoc, outVelocity, 15.0f);   // 20: tracing 보여질 프로젝타일 크기, 15: 시물레이션되는 Max 시간(초)
		//predictParams.DrawDebugTime = 15.0f;     //디버그 라인 보여지는 시간 (초)
		//predictParams.DrawDebugType = EDrawDebugTrace::Type::ForDuration;  // DrawDebugTime 을 지정하면 EDrawDebugTrace::Type::ForDuration 필요.
		//predictParams.OverrideGravityZ = GetWorld()->GetGravityZ();
		//FPredictProjectilePathResult result;
		//UGameplayStatics::PredictProjectilePath(this, predictParams, result);
		
		Arrow->AddImpulse(outVelocity);
		Arrow->AddForce(ForwardVec*9000.f);
	}
	
}

void AArrow::FireStrate(FVector TargetLoc)
{
	//콜리전 설정
	Arrow->SetSimulatePhysics(true);
	Arrow->SetNotifyRigidBodyCollision(true);
	Arrow->SetCollisionProfileName(TEXT("Arrow"));

	Arrow->AddForce((TargetLoc - GetActorLocation())*1000.f);  //AddForce(TargetLoc * 100.f);
}

void AArrow::SetArrowState(uint8 ArrowStateNum)
{
	//const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EArrowState"), true);
	//if (!EnumPtr) return;
	//CurrentState = EnumPtr->GetNameByValue(ArrowStateNum);
	CurrentState = (EArrowState)ArrowStateNum;
	if (ArrowParticle != nullptr)
		ArrowParticle->DestroyComponent();

	switch (CurrentState)
	{
	case EArrowState::Default:
		break;
	case EArrowState::Fire:
		ArrowParticle = UGameplayStatics::SpawnEmitterAttached(FireParticle, Arrow, FName("ParticleSocket"));
		break;
	case EArrowState::Poision:
		ArrowParticle = UGameplayStatics::SpawnEmitterAttached(PoisionParticle, Arrow, FName("ParticleSocket"));
		break;
	}
	
}

void AArrow::SetLightMat(bool isLight)
{
	if (isLight)
		Arrow->SetStaticMesh(LightArrowMesh);
	else
		Arrow->SetStaticMesh(DefaultArrowMesh);


	//RootComponent = CurrentMesh;
	//UStaticMesh a = (Arrow->GetStaticMesh());
	//Arrow->SetStaticMesh(CurrentMesh);
	
}

UStaticMeshComponent* AArrow::GetMesh()
{
	return Arrow;
}


