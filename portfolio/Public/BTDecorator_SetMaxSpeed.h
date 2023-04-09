// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_SetMaxSpeed.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API UBTDecorator_SetMaxSpeed : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_SetMaxSpeed();
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float TargetSpeed;
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
