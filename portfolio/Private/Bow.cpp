// Fill out your copyright notice in the Description page of Project Settings.


#include "Bow.h"
#include "Arrow.h"
#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "KnightEnemy.h"
// Sets default values
ABow::ABow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	
	RootComponent = Weapon;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("SkeletalMesh'/Game/AMyDirectory/ArrowBow/Bow/BOWunreal.BOWunreal'"));
	if (SK_WEAPON.Succeeded())
	{
		Weapon->SetSkeletalMesh(SK_WEAPON.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Bow_MONTAGE(TEXT("AnimMontage'/Game/AMyDirectory/ArrowBow/Bow/Mon_BowDraw.Mon_BowDraw'"));
	if (Bow_MONTAGE.Succeeded())
	{
		BowMontage = Bow_MONTAGE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> BowIdle_MONTAGE(TEXT("AnimMontage'/Game/AMyDirectory/ArrowBow/Bow/Mon_BowIdle.Mon_BowIdle'"));
	if (BowIdle_MONTAGE.Succeeded())
	{
		BowIdleMontage = BowIdle_MONTAGE.Object;
	}
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));
	
	
}

// Called when the game starts or when spawned 
void ABow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ABow::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}
void ABow::PlayDrawBowMon(bool IsDraw, int8 fireIndex)
{
	if (IsDraw)
	{
		Weapon->PlayAnimation(BowMontage, false);
		SpawnArrow();
		
		if (fireIndex == 1)
		{
			Arrow->SetArrowState(0);
			Arrow->SetLightMat(true);
			Arrow->GetMesh()->SetWorldScale3D(FVector(1.f, 3.f, 3.f));
		}
		else
		{
			//Arrow->GetMesh()->SetWorldScale3D(FVector(1.f, 1.f, 1.f));
		}
	}
	else
	{
		Weapon->PlayAnimation(BowIdleMontage, false);
		DefaultFire();
	}
		
}
void ABow::SpawnArrow()
{
	FActorSpawnParameters SpawnInfo;
	Arrow = GetWorld()->SpawnActor<AArrow>(SpawnInfo);
	if (Arrow != nullptr)
	{
		Arrow->AttachToComponent(Cast<AMainCharacter>(GetOwner())->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "ArrowSocket");
		Arrow->SetArrowState(ArrowType);
	}
}
void ABow::DefaultFire()
{
	if(Arrow == nullptr) return;
	//UE_LOG(LogTemp, Warning, TEXT("---Detach--"));
	Arrow->DetachRootComponentFromParent();
	
	FHitResult HitResult = Cast<AMainCharacter>(GetOwner())->LineTrace();
	
	Arrow->Fire(HitResult.Actor.Get(), Cast<AMainCharacter>(GetOwner())->FollowCamera->GetForwardVector(), HitResult.ImpactPoint);
	Arrow = nullptr;
}
void ABow::SetArrowType()
{
	//증가
	ArrowType++;
	//범위내 설정
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EArrowState"), true);
	if (!EnumPtr) return;
	if(ArrowType >= EnumPtr->NumEnums()-2)
		ArrowType=0;
	//현재 화살의 타입 결정
	if(Arrow != nullptr)
		Arrow->SetArrowState(ArrowType);
}
void ABow::SpawnWideArrow()
{
	
	Weapon->PlayAnimation(BowIdleMontage, false);
	Arrow->GetMesh()->SetWorldScale3D(FVector(1.f, 1.f, 1.f));
	Arrow->Destroy();
	Arrow = nullptr;
	WideArrowArr.Empty();
	WideEnemyArr.Empty();
	TArray<FHitResult> hits;
	TArray<TEnumAsByte<EObjectTypeQuery>> objectType;
	objectType.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

	if (UKismetSystemLibrary::SphereTraceMultiForObjects(this, GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation(), 2500.f, objectType,
		false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, hits, true))
	{
		for (int i = 0; i < hits.Num(); i++)
		{
			//필터링
			auto enemy = Cast<AKnightEnemy>(hits[i].GetActor());
			if (enemy == nullptr) continue;
			if (Cast<AMainCharacter>(GetOwner())->FindBetweenAngle(GetActorLocation(), GetActorForwardVector(), enemy->GetActorLocation()) > 75.f) continue;

			//화살 생성
			FActorSpawnParameters SpawnInfo;
			AArrow* arrow = GetWorld()->SpawnActor<AArrow>(SpawnInfo);
			if (arrow != nullptr)
			{
				arrow->AttachToComponent(Cast<AMainCharacter>(GetOwner())->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "ArrowSocket");
				WideArrowArr.Add(arrow);
				WideEnemyArr.Add(enemy);
			}
		}
	}
	WideFire();
}
void ABow::WideFire()
{
	//화살 발사
	for (int i = 0; i < WideArrowArr.Num(); i++)
	{
		AActor* enemy = WideEnemyArr[i];
		AArrow* arrow = WideArrowArr[i];
		arrow->DetachRootComponentFromParent();
		arrow->FireStrate(enemy->GetActorLocation());
	}
	
}

