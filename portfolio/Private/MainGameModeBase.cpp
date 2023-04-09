// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameModeBase.h"
#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h"
#include "MainCharacter.h"
#include "MainHUDWidget.h"
#include "TitleUserWidget.h"
#include "MainSaveGame.h"
#include "SkillTreeWidget.h"
#include "SkillNodeWidget.h"
#include "AimUI.h"


AMainGameModeBase::AMainGameModeBase()
{
	DefaultPawnClass = AMainCharacter::StaticClass();
	PlayerControllerClass = AMainPlayerController::StaticClass();

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

	static ConstructorHelpers::FClassFinder<UUserWidget> CLEARWIDGET(TEXT("WidgetBlueprint'/Game/AMyDirectory/UI/Title/UI_Clear.UI_Clear_C'"));
	if (CLEARWIDGET.Succeeded())
	{
		ClearWidgetClass = CLEARWIDGET.Class;
	}
	if (IsValid(ClearWidgetClass))
	{
		ClearWidget = Cast<UTitleUserWidget>(CreateWidget(GetWorld(), ClearWidgetClass));
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> SkillTreeWidgetAsset(TEXT("WidgetBlueprint'/Game/AMyDirectory/UI/SkillTree/UI_SkillTree.UI_SkillTree_C'"));
	if (SkillTreeWidgetAsset.Succeeded())
	{
		if(IsValid(SkillTreeWidgetAsset.Class))
			SkillTreeWidget = Cast<USkillTreeWidget>(CreateWidget(GetWorld(), SkillTreeWidgetAsset.Class));
	}
	
	//Save
	SaveSlotName = TEXT("MainPlayer");
}

void AMainGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void AMainGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	MainHUD->AddToViewport();
	if (SkillTreeWidget != nullptr)
	{
		SkillTreeWidget->AddToViewport();
		SkillTreeWidget->SetVisibility(ESlateVisibility::Hidden);
	}
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

void AMainGameModeBase::SetPotionNumHUD(int32 PotionNum)
{
	MainHUD->SetPotionText(PotionNum);
}

void AMainGameModeBase::SetGoldNumHUD(int32 GoldNum)
{
	MainHUD->SetGoldText(GoldNum);
}

void AMainGameModeBase::LoadData()
{
	Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	MainSaveGame = Cast<UMainSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (nullptr == MainSaveGame)
	{
		MainSaveGame = GetMutableDefault<UMainSaveGame>(); // Gets the mutable default object of a class.
	}

	Player->HealthPoint = MainSaveGame->HealthPoint;
	Player->ManaPoint = MainSaveGame->ManaPoint;
	Player->Inventory->AddPotion(MainSaveGame->PotionNum);
	Player->Inventory->AddGold(MainSaveGame->GoldNum);
	
	SetHealthPointHUD(MainSaveGame->HealthPoint);
	SetExperiencePointHUD(MainSaveGame->ManaPoint); 
	SetPotionNumHUD(MainSaveGame->PotionNum);
	SetGoldNumHUD(MainSaveGame->GoldNum);
}

void AMainGameModeBase::SaveData()
{
	Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	UMainSaveGame* NewPlayerData = NewObject<UMainSaveGame>();
	NewPlayerData->HealthPoint = Player->HealthPoint;
	NewPlayerData->ManaPoint = Player->ManaPoint;
	NewPlayerData->PotionNum = Player->Inventory->GetPotionNum();
	NewPlayerData->GoldNum = Player->Inventory->GetGoldNum();

	if (false == UGameplayStatics::SaveGameToSlot(NewPlayerData, SaveSlotName, 0))
	{
		UE_LOG(LogClass, Warning, TEXT("SaveGame Error!"));
	}
}

void AMainGameModeBase::SetSkillUI()
{
	if (SkillTreeWidget != nullptr)
	{
		
		EnableSkillUI = !EnableSkillUI;
		SkillTreeWidget->FadeAnimationPlay(EnableSkillUI);
		APlayerController* Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		Controller->bShowMouseCursor = EnableSkillUI;
	}
}





