// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h" 
#include "Engine/DataTable.h"
#include "ShopItemSlot.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FShopItem: public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* ItemImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Cost;
};

UCLASS()
class PORTFOLIO_API UShopItemSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UShopItemSlot(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	UFUNCTION(BlueprintCallable)
	void SlotButtonCallback();
	void SlotInit();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UImage* ItemImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTextBlock* ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTextBlock* Cost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UButton* ItemButton;
	FShopItem ShopItemStruct;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Shop")
	class UShopWidget* ParentWidget;
};
