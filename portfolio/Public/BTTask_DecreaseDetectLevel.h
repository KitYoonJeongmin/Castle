// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_DecreaseDetectLevel.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API UBTTask_DecreaseDetectLevel : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_DecreaseDetectLevel();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
