// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetStrafeLocation.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API UBTTask_GetStrafeLocation : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_GetStrafeLocation();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
