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
	

private:

	UPROPERTY()
		class UProgressBar* HPBar;

	UPROPERTY()
		class UProgressBar* ExpBar;

	UPROPERTY()
		class UImage* WeaponImg;
	UPROPERTY()
		TArray<UTexture2D*> WeaponImagArr;
};
