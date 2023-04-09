// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "MainGameModeBase.h"
// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	PotionNum = 0;
	GoldNum = 0;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	MainGameMode = Cast<AMainGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInventoryComponent::AddPotion(int32 AddNum)
{
	PotionNum+= AddNum;
	MainGameMode->SetPotionNumHUD(PotionNum);
}

bool UInventoryComponent::MinusPotion(int32 MinusNum)
{
	if (PotionNum < MinusNum)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot use Potion: %s"), PotionNum);
		return false;
	}
	PotionNum -= MinusNum;
	MainGameMode->SetPotionNumHUD(PotionNum);
	return true;
}

void UInventoryComponent::AddGold(int32 AddNum)
{
	GoldNum += AddNum;
	MainGameMode->SetGoldNumHUD(GoldNum);
}

bool UInventoryComponent::MinusGold(int32 MinusNum)
{
	if (GoldNum < MinusNum) return false;
	GoldNum -= MinusNum;
	MainGameMode->SetGoldNumHUD(GoldNum);
	return true;
}

