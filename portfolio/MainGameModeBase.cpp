// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameModeBase.h"
#include "MainCharacter.h"
#include "Blueprint/UserWidget.h"
#include "AimUI.h"

AMainGameModeBase::AMainGameModeBase()
{
	DefaultPawnClass = AMainCharacter::StaticClass();

	static ConstructorHelpers::FClassFinder<UUserWidget> AimWidgetAsset(TEXT("WidgetBlueprint'/Game/AMyDirectory/UI/UI_Aim.UI_Aim_C'"));

	// TSubclassOf ���ø� Ŭ���� ��ü�� �������Ʈ Ŭ������ �־��ش�
	if (AimWidgetAsset.Succeeded())
		AnimWidgetClass = AimWidgetAsset.Class;
	if (IsValid(AnimWidgetClass))
	{
		AimWidget = Cast<UAimUI>(CreateWidget(GetWorld(), AnimWidgetClass));
	}
}

void AMainGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void AMainGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	 
	
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



