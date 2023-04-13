// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API UTitleUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UTitleUserWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	UFUNCTION()
		void StartGame();
	UFUNCTION()
		void ExitGame();
	UFUNCTION()
		void PlayFade();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UButton* StartBtn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UButton* EndBtn;
	UPROPERTY(BlueprintReadOnly, Transient,  meta = (BindWidgetAnim))
		class UWidgetAnimation* Fade;
	
};
