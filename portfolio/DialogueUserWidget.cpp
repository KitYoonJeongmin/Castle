// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueUserWidget.h"
#include "Components/TextBlock.h"


UDialogueUserWidget::UDialogueUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UDialogueUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	DiagloueText = Cast<UTextBlock>(GetWidgetFromName(TEXT("DialogueText")));
}

void UDialogueUserWidget::SetDiagloueText(FString Text)
{
	if (DiagloueText == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("DialogueText is not vailed")));
		return;
	}
	DiagloueText->SetText(FText::FromString(Text));
}
