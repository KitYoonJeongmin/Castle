// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"

UMainGameInstance::UMainGameInstance()
{
	FString CharacterDataPath = TEXT("DataTable'/Game/AMyDirectory/CharacterStatData/MainCharacterData.MainCharacterData'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_MAINCHARACTER(*CharacterDataPath);
	check(DT_MAINCHARACTER.Succeeded());
	MainCharacterTable = DT_MAINCHARACTER.Object;
	checkf(MainCharacterTable->GetRowMap().Num() > 0, TEXT("Row of Main Character Table is not availd"));
}

void UMainGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("DropExp of Level 20 ABCharacter : %d"), GetMainCharacterData(20)->DropExp);
}

FMainCharacterData* UMainGameInstance::GetMainCharacterData(int32 Level)
{
	return MainCharacterTable->FindRow<FMainCharacterData>(*FString::FromInt(Level),TEXT(""));
}
