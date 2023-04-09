// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossAIController.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API ABossAIController : public AAIController
{
	GENERATED_BODY()
public:
	ABossAIController();
	virtual void OnPossess(APawn* InPawn) override;
private:
	UPROPERTY()
		class UBehaviorTree* BTAsset;
	UPROPERTY()
		class UBlackboardData* BBAsset;
protected:
	virtual void BeginPlay() override;
};
