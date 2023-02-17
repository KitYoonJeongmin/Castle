// Fill out your copyright notice in the Description page of Project Settings.


#include "AimUI.h"
#include "Math/Color.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"

void UAimUI::NativeConstruct()
{
	Super::NativeConstruct();
	Aim = Cast<UImage>(GetWidgetFromName(FName("Aim_Image")));


	DefaultColor = FLinearColor(1.f, 1.f, 1.f, 1.f);
	ChangeColor = FLinearColor(1.f, 0.f, 0.f, 1.f);
}

void UAimUI::SetAimColor(bool isCanAttack)
{
	if (Aim != nullptr)
	{
		if(isCanAttack)
			Aim->SetColorAndOpacity(ChangeColor);
		else
			Aim->SetColorAndOpacity(DefaultColor);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("is not connect..."));
	}
}
