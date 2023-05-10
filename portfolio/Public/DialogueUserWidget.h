// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API UDialogueUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UDialogueUserWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

	void SetDiagloueText(FString Text);
private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Climbing, Meta = (AllowPrivateAccess = true))
		class UTextBlock* DiagloueText;
};
