// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h" 
#include "Engine/Texture2D.h"
#include "Styling/SlateBrush.h"

UMainHUDWidget::UMainHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> FIST_IMAGE(TEXT("Texture2D'/Game/AMyDirectory/UI/Assets/fist.fist'"));
	if (FIST_IMAGE.Succeeded())
	{
		WeaponImagArr.Add(FIST_IMAGE.Object);
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> SWORD_IMAGE(TEXT("Texture2D'/Game/AMyDirectory/UI/Assets/SwordWhite.SwordWhite'"));
	if (SWORD_IMAGE.Succeeded())
	{
		WeaponImagArr.Add(SWORD_IMAGE.Object);
	}
	static ConstructorHelpers::FObjectFinder<UTexture2D> BOW_IMAGE(TEXT("Texture2D'/Game/AMyDirectory/UI/Assets/BowWhite.BowWhite'"));
	if (BOW_IMAGE.Succeeded())
	{
		WeaponImagArr.Add(BOW_IMAGE.Object);
	}
}
void UMainHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	ExpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbExp")));
	//check(nullptr != ExpBar);

	WeaponImg = Cast<UImage>(GetWidgetFromName(TEXT("WeaponImage")));
}

void UMainHUDWidget::UpdateHPWidget(float HPpercent)
{
	if (nullptr != pbHP)
	{
		pbHP->SetPercent(HPpercent/100.f);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HPBar is nullptr : %s"),*this->GetName());
	}
}

void UMainHUDWidget::UpdateExpWidget(float EXpercent)
{
	if (nullptr != ExpBar)
	{
		ExpBar->SetPercent(EXpercent / 100.f);
	}
}

void UMainHUDWidget::SetWeaponImage(uint8 WeaponNum)
{
	WeaponImg->SetBrushFromTexture(WeaponImagArr[WeaponNum],true);
}
