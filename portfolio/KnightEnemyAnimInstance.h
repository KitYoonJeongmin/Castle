// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KnightEnemyAnimInstance.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnEQUIPDelegate);
DECLARE_MULTICAST_DELEGATE(FOnsheathDelegate);
DECLARE_MULTICAST_DELEGATE(FOnRunAIDelegate);
DECLARE_MULTICAST_DELEGATE(FOnStopAIDelegate);

UCLASS()
class PORTFOLIO_API UKnightEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
	UKnightEnemyAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
public:
	void PlayDrawSwordMontage();
	void PlayAttackMontage();
	void PlaySwordBlockMontage(bool isPaly);
	void PlayPainMontage();
	void PlayNuckMontage();
	void PlayAssassinationAnim();
	void JumpToAttackMontageSection(int32 NewSection);
	void JumpToSwordMontageSection(int32 NewSection);
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	FOnEQUIPDelegate OnEQUIP;
	FOnsheathDelegate Onsheath;
	FOnRunAIDelegate OnRunAI;
	FOnStopAIDelegate OnStopAI;
	
public:
	void SetDeadAnim() { IsDead = true; }
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed;
	UPROPERTY()
		class ACharacter* Character;

	//FootIK
	UPROPERTY()
		TArray<AActor*> IgnoreActors;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK", meta = (AllowPrivateAccess = "true"))
		float Displacement;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IK", meta = (AllowPrivateAccess = "true"))
		float IKInterpSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK", meta = (AllowPrivateAccess = "true"))
		FRotator RRot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK", meta = (AllowPrivateAccess = "true"))
		FRotator LRot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK", meta = (AllowPrivateAccess = "true"))
		float RIK;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK", meta = (AllowPrivateAccess = "true"))
		float LIK;
	void FootIK(float DeltaTime);
	TTuple<bool, float> CapsuleDistance(FName SocketName, ACharacter* Char);
	TTuple<bool, float, FVector> FootLineTrace(FName SocketName, ACharacter* Char);
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* EnemyAttackMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* EnemyDrawSwordMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Block, Meta = (AllowPrivateAccess = true))
		UAnimMontage* EnemySwordBlockMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Pain, Meta = (AllowPrivateAccess = true))
		UAnimMontage* EnemyPainMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Pain, Meta = (AllowPrivateAccess = true))
		UAnimMontage* EnemyNuckMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Pain, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AssassinationAnim;

	UFUNCTION()
		void AnimNotify_AttackHitCheck();

	UFUNCTION()
		void AnimNotify_NextAttackCheck();
	UFUNCTION()
		void AnimNotify_EQUIP();
	UFUNCTION()
		void AnimNotify_sheath();
	UFUNCTION()
		void AnimNotify_RunAI();
	UFUNCTION()
		void AnimNotify_StopAI();

	FName GetAttackMontageSectionName(int32 Section);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsDead;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsWeapon;
};
