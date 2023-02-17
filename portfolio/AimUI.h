// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AimUI.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API UAimUI : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetAimColor(bool isCanAttack);
protected:
	virtual void NativeConstruct();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UImage* Aim;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FLinearColor DefaultColor;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FLinearColor ChangeColor;

	
};
