// Fill out your copyright notice in the Description page of Project Settings.


#include "Bow.h"
#include "Arrow.h"
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

void ABow::PlayDrawBowMon(bool IsDraw)
{
	if (IsDraw)
	{
		Weapon->PlayAnimation(BowMontage, false);
	}
		
	else
	{
		Weapon->PlayAnimation(BowIdleMontage, false);
	}
		
}

