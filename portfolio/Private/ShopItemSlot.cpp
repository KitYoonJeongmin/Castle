// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopItemSlot.h"
#include "ShopWidget.h"
#include "MainCharacter.h"
#include "InventoryComponent.h"
#include "Components/Image.h" 
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
UShopItemSlot::UShopItemSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UShopItemSlot::NativeConstruct()
{
	Super::NativeConstruct();
	ItemImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemImage")));
	ItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemName")));
	Cost = Cast<UTextBlock>(GetWidgetFromName(TEXT("GoldNum")));
	ItemButton = Cast<UButton>(GetWidgetFromName(TEXT("ItemButton")));

	

	ItemButton->OnClicked.AddDynamic(this, &UShopItemSlot::SlotButtonCallback);
}

void UShopItemSlot::SlotButtonCallback()
{
	UInventoryComponent* CharInven = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0))->Inventory;
	if (CharInven->MinusGold(ShopItemStruct.Cost))
	{
		if (ShopItemStruct.ItemName == "Potion")
		{
			CharInven->AddPotion(1);
		}
		//팔고싶은 다른 아이템 추가 else if(ShopItemStruct.ItemName == "다른아이템")
		ParentWidget->UpdateState();
	}
	
	
}
void UShopItemSlot::SlotInit()
{
	ItemImage->SetBrushFromTexture(ShopItemStruct.ItemImage,false);
	ItemName->SetText(FText::FromString(ShopItemStruct.ItemName));
	Cost->SetText(FText::FromString(FString::FromInt(ShopItemStruct.Cost)));
}

