// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCharacter.h"

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

}

// Called when the game starts or when spawned
void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

