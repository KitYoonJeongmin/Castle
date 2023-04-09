// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddPotion(int32 AddNum);
	bool MinusPotion(int32 MinusNum);
	void AddGold(int32 AddNum);
	bool MinusGold(int32 MinusNum);
	class AMainGameModeBase* MainGameMode;

	int32 GetPotionNum() { return PotionNum; }
	int32 GetGoldNum() { return GoldNum; }
private:
	int32 PotionNum;
	int32 GoldNum;
};
