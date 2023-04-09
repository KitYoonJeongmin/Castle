// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_IsInRange.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API UBTService_IsInRange : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_IsInRange();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
