// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MainSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API UMainSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UMainSaveGame();
public:
	UPROPERTY()
	float HealthPoint;

	UPROPERTY()
	float ManaPoint;

	UPROPERTY()
	int32 PotionNum;

	UPROPERTY()
	int32 GoldNum;

};
