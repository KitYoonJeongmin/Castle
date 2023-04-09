// Fill out your copyright notice in the Description page of Project Settings.


#include "Daggle.h"

// Sets default values
ADaggle::ADaggle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("SkeletalMesh'/Game/Weapon_Pack/Skeletal_Mesh/SK_Dagger_2.SK_Dagger_2'"));
	if (SK_WEAPON.Succeeded())
	{
		Weapon->SetSkeletalMesh(SK_WEAPON.Object);
	}
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));

}

// Called when the game starts or when spawned
void ADaggle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADaggle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

