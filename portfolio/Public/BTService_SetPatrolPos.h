// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_SetPatrolPos.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API UBTService_SetPatrolPos : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_SetPatrolPos();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
