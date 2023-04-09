// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopWidget.h"
#include "ShopItemSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "MainGameModeBase.h"
#include "MainHUDWidget.h"

UShopWidget::UShopWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_SHOP(TEXT("DataTable'/Game/AMyDirectory/CharacterStatData/ShopData.ShopData'"));


	if(DT_SHOP.Succeeded())
		ShopItemTable = DT_SHOP.Object;

}

void UShopWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ExitBtn = Cast<UButton>(GetWidgetFromName("ExitButton"));
	ExitBtn->OnClicked.AddDynamic(this, &UShopWidget::Exit);
	Gold = Cast<UTextBlock>(GetWidgetFromName("PlayerGold"));
	Potion = Cast<UTextBlock>(GetWidgetFromName("PlayerPotion"));
	MainHUD = Cast<AMainGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->MainHUD;
	UpdateState();
	//Slot init
	int32 i = 0;
	while (true)
	{
		FString SlotName = FString::Printf(TEXT("ItemSlot%d"), i);
		UShopItemSlot* CurrentSlot = Cast<UShopItemSlot>(GetWidgetFromName(FName(*SlotName)));
		if (CurrentSlot != nullptr)
		{
			FShopItem* ShopItem = ShopItemTable->FindRow<FShopItem>(*FString::FromInt(i), TEXT(""));
			if (ShopItem == nullptr)
			{
				return;
			}
			CurrentSlot->ParentWidget = this;
			CurrentSlot->ShopItemStruct.ItemName = ShopItem->ItemName;
			CurrentSlot->ShopItemStruct.ItemImage = ShopItem->ItemImage;
			CurrentSlot->ShopItemStruct.Cost = ShopItem->Cost;
			CurrentSlot->SlotInit();
			ItemSlotArr.Add(CurrentSlot);
			i++;
		}
		else
		{
			break;
		}	
	}

}

void UShopWidget::Exit()
{
	UE_LOG(LogTemp, Warning, TEXT("Exit"));
	this->RemoveFromViewport();
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->bShowMouseCursor = false;
}

void UShopWidget::UpdateState()
{
	if (MainHUD == nullptr) return;
	if(Gold == nullptr) return;
	Gold->SetText(MainHUD->GoldText->GetText());
	Potion->SetText(MainHUD->PotionText->GetText());
}
