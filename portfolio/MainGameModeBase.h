// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API AMainGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AMainGameModeBase();

	virtual void PostLogin(APlayerController* NewPlayer);
	virtual void BeginPlay();
	void AttachAimWidget(bool isView);
	void SetAimColor(bool isCanAttack);
	void WeaponChange(uint8 WeaponIdx);
	void SetHealthPointHUD(float hp);
	void SetExperiencePointHUD(float exp);
protected:
	UPROPERTY()
	TSubclassOf<UUserWidget> AnimWidgetClass;
	UPROPERTY()
	class UAimUI* AimWidget;
	UPROPERTY()
		TSubclassOf<UUserWidget> MainHUDClass;
	UPROPERTY()
		class UMainHUDWidget* MainHUD;

};
