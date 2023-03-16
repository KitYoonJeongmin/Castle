// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HorseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API UHorseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UHorseAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		float CurrentPawnForwardSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		float CurrentPawnRotation;
};
