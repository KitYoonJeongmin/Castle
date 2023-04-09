// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterStatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MainGameInstance.h"
// Sets default values for this component's properties
UMainCharacterStatComponent::UMainCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	Level = 1;
}


// Called when the game starts
void UMainCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
	SetNewLevel(Level);
	// ...
	
}

void UMainCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
}


// Called every frame
void UMainCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMainCharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	
	UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	check(nullptr != MainGameInstance);
	CurrentStatData = MainGameInstance->GetMainCharacterData(NewLevel);
	if (nullptr != CurrentStatData)
	{
		Level = NewLevel;
		SetHP(CurrentStatData->MaxHP);
	}
	else
	{
		UE_LOG(LogTemp,Error, TEXT("Level (%d) data doesn't exist"), NewLevel);
	}
}

void UMainCharacterStatComponent::SetDamage(float NewDamage)
{
	check(nullptr != CurrentStatData);
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.f, CurrentStatData->MaxHP));
}

void UMainCharacterStatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
}

float UMainCharacterStatComponent::GetAttack()
{
	check(nullptr != CurrentStatData);
	return CurrentStatData->Attack;
}

float UMainCharacterStatComponent::GetHPRatio()
{
	check(nullptr != CurrentStatData);
	return (CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / CurrentStatData->MaxHP);
}

