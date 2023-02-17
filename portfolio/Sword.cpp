// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"

// Sets default values
ASword::ASword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("SkeletalMesh'/Game/Weapon_Pack/Skeletal_Mesh/SK_Sword.SK_Sword'"));
	if (SK_WEAPON.Succeeded())
	{
		Weapon->SetSkeletalMesh(SK_WEAPON.Object);
	}
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void ASword::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

