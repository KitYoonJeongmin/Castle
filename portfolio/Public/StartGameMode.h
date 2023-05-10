// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StartGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API AStartGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AStartGameMode();
	virtual void BeginPlay() override;
private:
	TSubclassOf<UUserWidget> TitleUserWidgetClass;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "TitleUI", Meta = (AllowPrivateAccess = true))
	class UTitleUserWidget* TitleUserWidget;
	
};
