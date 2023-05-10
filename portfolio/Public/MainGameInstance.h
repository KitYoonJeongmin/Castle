// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FMainCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FMainCharacterData() : Level(1), MaxHP(100.0f), Attack(10.0f), DropExp(10), NextExp(30) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 DropExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 NextExp;
};
UCLASS()
class PORTFOLIO_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UMainGameInstance();

	virtual void Init() override;

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Data")
	FMainCharacterData* GetMainCharacterData(int32 Level);

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Data", Meta = (AllowPrivateAccess = true))
	class UDataTable* MainCharacterTable;

};
