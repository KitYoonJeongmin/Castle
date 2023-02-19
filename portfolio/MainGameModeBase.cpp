// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "MainCharacter.h"
#include "MainHUDWidget.h"
#include "AimUI.h"

AMainGameModeBase::AMainGameModeBase()
{
	DefaultPawnClass = AMainCharacter::StaticClass();

	static ConstructorHelpers::FClassFinder<UUserWidget> AimWidgetAsset(TEXT("WidgetBlueprint'/Game/AMyDirectory/UI/UI_Aim.UI_Aim_C'"));
	if (AimWidgetAsset.Succeeded())
		AnimWidgetClass = AimWidgetAsset.Class;
	if (IsValid(AnimWidgetClass))
	{
		AimWidget = Cast<UAimUI>(CreateWidget(GetWorld(), AnimWidgetClass));
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> MainWidgetAsset(TEXT("WidgetBlueprint'/Game/AMyDirectory/UI/UI_HUD.UI_HUD_C'"));
	if (MainWidgetAsset.Succeeded())
	{
		MainHUDClass = MainWidgetAsset.Class;
	}
	if (IsValid(MainHUDClass))
	{
		MainHUD = Cast<UMainHUDWidget>(CreateWidget(GetWorld(), MainHUDClass));
	}


}

void AMainGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void AMainGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	MainHUD->AddToViewport();
	
}

void AMainGameModeBase::AttachAimWidget(bool isView)
{
	if (IsValid(AimWidget))
	{
		if (!isView)
			AimWidget->AddToViewport();
		else
			AimWidget->RemoveFromParent();
	}
}

void AMainGameModeBase::SetAimColor(bool isCanAttack)
{
	AimWidget->SetAimColor(isCanAttack);
}

void AMainGameModeBase::WeaponChange(uint8 WeaponIdx)
{
	
	MainHUD->SetWeaponImage(WeaponIdx);
}

void AMainGameModeBase::SetHealthPointHUD(float hp)
{
	MainHUD->UpdateHPWidget(hp);
}

void AMainGameModeBase::SetExperiencePointHUD(float exp)
{
	MainHUD->UpdateExpWidget(exp);
}



