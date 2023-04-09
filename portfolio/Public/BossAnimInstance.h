// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KnightEnemyAnimInstance.h"
#include "BossAnimInstance.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnMoveToTargetDelegate);

UCLASS()
class PORTFOLIO_API UBossAnimInstance : public UKnightEnemyAnimInstance
{
	GENERATED_BODY()
public:
	UBossAnimInstance();

	void PalyMediumSkillMon();
	void PalyFarSkillMon();
	FOnMoveToTargetDelegate OnMoveToTarget;
private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
		UAnimMontage* MediumSkill;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
		UAnimMontage* FarSkill;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
		UAnimMontage* PauseMon;
	UFUNCTION()
		void AnimNotify_MoveToTarget();

};
