// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUDWidget.h"
#include "Components/ProgressBar.h"

void UMainHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbHP")));
	//check(nullptr != HPBar);

	//ExpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbExp")));
	//check(nullptr != ExpBar);



}

void UMainHUDWidget::UpdateHPWidget(float HPpercent)
{
	if (nullptr != HPBar)
	{
		HPBar->SetPercent(HPpercent/100.f);
	}
}
