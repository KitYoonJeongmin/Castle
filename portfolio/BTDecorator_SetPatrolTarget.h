// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_SetPatrolTarget.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API UBTDecorator_SetPatrolTarget : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_SetPatrolTarget();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
