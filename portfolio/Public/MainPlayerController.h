// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API AMainPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = AIPerceptionTeam, Meta = (AllowPrivateAccess = true))
		FGenericTeamId TeamId = FGenericTeamId(1);
	virtual FGenericTeamId GetGenericTeamId() const override;

};
