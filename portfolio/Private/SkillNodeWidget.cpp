// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillNodeWidget.h"
#include "SkillTreeWidget.h"
#include "Components/Image.h" 
#include "Components/Button.h"
#include "Math/Color.h"

USkillNodeWidget::USkillNodeWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void USkillNodeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SkillImage = Cast<UImage>(GetWidgetFromName(TEXT("SkillImg")));
	SkillBorder = Cast<UImage>(GetWidgetFromName(TEXT("border")));
	SkillBtn = Cast<UButton>(GetWidgetFromName(TEXT("Btn")));

	SetSkillImage(SkillTextrue);
	SkillBtn->OnClicked.AddDynamic(this, &USkillNodeWidget::NodeButtonCallback);
}


void USkillNodeWidget::SetSkillImage(UTexture2D* Image)
{
	if (Image == nullptr) return;
	SkillImage->SetBrushFromTexture(Image, true);
}

void USkillNodeWidget::SetNodeState(int32 StateNum)
{
	CurrentState = (ESkillNodeState)StateNum;
	FLinearColor SkillImageColor(1.f,1.f,1.f,1.f);
	FLinearColor SkillBorderColor(1.f, 1.f, 1.f, 1.f);
	bool EnableBtn = false;
	switch (CurrentState)
	{
	case ESkillNodeState::E_Enable:
		EnableBtn = true;
		break;
	case ESkillNodeState::E_Disable:
		SkillImageColor.A = 0.15f;
		SkillBorderColor.A = 0.15f;
		break;
	case ESkillNodeState::E_Select:
		SkillBorderColor.B = 0.f;
		break;
	}

	SkillBorder->SetColorAndOpacity(SkillBorderColor);
	SkillImage->SetColorAndOpacity(SkillImageColor);
	SkillBtn->SetIsEnabled(EnableBtn);

	

}

void USkillNodeWidget::NodeButtonCallback()
{
	if(ParentWidget == nullptr) return;
	ParentWidget->SelectNode(this);

	ParentWidget->UpdateTree();

	UE_LOG(LogClass, Warning, TEXT("SelectNode: %d"), ParentWidget->TreeLoad.Num());
}
