// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillTreeWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API USkillTreeWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	USkillTreeWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	void UpdateTree();
	void SelectNode(class USkillNodeWidget* NodeWidget);
	void FadeAnimationPlay(bool IsIn);
public:
	
	TArray<class USkillNodeWidget*> SkillTree;
	TArray<int32> TreeLoad;
	TArray<TTuple<int32, int32>> TreeConnection;
	TMap<class USkillNodeWidget*, int32> NodeNumMap;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
		class UWidgetAnimation* Fade;
	int32 NodeNum = 7;
};
