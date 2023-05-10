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
	//void AttachInteractWidget(bool isView);
	void SetAimColor(bool isCanAttack);
	void WeaponChange(uint8 WeaponIdx);
	void SetHealthPointHUD(float hp);
	void SetExperiencePointHUD(float exp);
	void SetPotionNumHUD(int32 PotionNum);
	void SetGoldNumHUD(int32 GoldNum);
public:
	void LoadData();
	void SaveData();
public:
	void SetSkillUI();
	bool EnableSkillUI = false;
public:
	bool IsAttachingTargetOn = false;
	class UMainSaveGame* MainSaveGame;
public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = UI)
	TSubclassOf<UUserWidget> AnimWidgetClass;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = UI)
	class UAimUI* AimWidget;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = UI)
		TSubclassOf<UUserWidget> MainHUDClass;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = UI)
		class UMainHUDWidget* MainHUD;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = UI)
		TSubclassOf<UUserWidget> ClearWidgetClass;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = UI)
		class UTitleUserWidget* ClearWidget;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = UI)
		class USkillTreeWidget* SkillTreeWidget;
private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Character", Meta = (AllowPrivateAccess = true))
	class AMainCharacter* Player;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Save", Meta = (AllowPrivateAccess = true))
		FString SaveSlotName;
};
