// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API UMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UMainHUDWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;	
public:
	void UpdateHPWidget(float HPpercent);
	void UpdateExpWidget(float HPpercent);
	void SetWeaponImage(uint8 WeaponNum);
	void SetPotionText(int32 PotionNum);
	void SetGoldText(int32 GoldNum);
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* pbHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UProgressBar* ExpBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UImage* WeaponImg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UTexture2D*> WeaponImagArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTextBlock* PotionText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTextBlock* GoldText;
}; 
