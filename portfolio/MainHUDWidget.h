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
		/*
public:
	void BindCharacterStat(class UMainCharacterStatComponent* CharacterStat);
	void BindPlayerState(class AABPlayerState* PlayerState);

protected:
	virtual void NativeConstruct() override;
	void UpdateCharacterStat();
	void UpdatePlayerState();

private:
	TWeakObjectPtr<class UMainCharacterStatComponent> CurrentCharacterStat;
	TWeakObjectPtr<class AABPlayerState> CurrentPlayerState;
	UPROPERTY()
		class UProgressBar* HPBar;

	UPROPERTY()
		class UProgressBar* ExpBar;

	UPROPERTY()
		class UTextBlock* PlayerLevel;*/
};
