// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API UShopWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UShopWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	UFUNCTION()
		void Exit();
	void UpdateState();
	TArray<class UShopItemSlot*> ItemSlotArr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UButton* ExitBtn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTextBlock* Gold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTextBlock* Potion;
private:
	UPROPERTY()
		class UDataTable* ShopItemTable;
	class UMainHUDWidget* MainHUD;
};
