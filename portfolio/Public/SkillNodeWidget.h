// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillNodeWidget.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ESkillNodeState : uint8
{
	E_Enable = 0 UMETA(DisplayName = "Enable"),
	E_Disable = 1 UMETA(DisplayName = "Disable"),
	E_Select = 2 UMETA(DisplayName = "Select"),
};

UCLASS()
class PORTFOLIO_API USkillNodeWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	USkillNodeWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
public:
	void SetSkillImage(UTexture2D* Image);

	void SetNodeState(int StateNum);
	UFUNCTION(BlueprintCallable)
		void NodeButtonCallback();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UImage* SkillImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UImage* SkillBorder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* SkillTextrue;
	class UButton* SkillBtn;
public:
	UPROPERTY()
		TArray<int32> NextSkillNodes;
	UPROPERTY()
		ESkillNodeState CurrentState = ESkillNodeState::E_Disable;
	UPROPERTY()
		class USkillTreeWidget* ParentWidget;
};
