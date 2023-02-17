// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainCharacter.h"
#include "MainAnimInstance.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnEQUIPDelegate);
DECLARE_MULTICAST_DELEGATE(FOnsheathDelegate);
DECLARE_MULTICAST_DELEGATE(FOnBowFireDelegate);


UCLASS()
class PORTFOLIO_API UMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMainAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	//АјАн animation
public:
	void PlayDrawSwordMontage();
	void PlayAttackMontage();

	void PlayRollMontage();
	void PlaySwordBlockMontage(bool isPaly);
	void PlaySwordSkill_1Montage();
	void PlaySwordSkill_2Montage();

	void PlayTurn90Montage(bool isRight);

	void PlayDrawBowMontage();
	void PlayBowShotMontage();

	void JumpToAttackMontageSection(int32 NewSection);
	void JumpToSwordMontageSection(int32 NewSection);
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	FOnEQUIPDelegate OnEQUIP;
	FOnsheathDelegate Onsheath;
	FOnBowFireDelegate OnBowFire;
	
public:
	void SetDeadAnim() { IsDead = true; }
	void SetUpperBodyRotation(float Yaw, float Pitch);
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, Meta = (AllowPrivateAccess = true))
		EWeapon CurrentWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bow, Meta = (AllowPrivateAccess = true))
		bool isZooming;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bow, Meta = (AllowPrivateAccess = true))
		bool isAimming;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		float CurrentPawnForwardSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		float CurrentPawnRotation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		bool IsInAir;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aim, Meta = (AllowPrivateAccess = true))
		float AimYaw;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Aim, Meta = (AllowPrivateAccess = true))
		float AimPitch;

	void FootIK(float DeltaTime);
	TTuple<bool, float> CapsuleDistance(FName SocketName, ACharacter* Char);
	TTuple<bool, float, FVector> FootLineTrace(FName SocketName, ACharacter* Char);

protected:
	UPROPERTY()
		class AMainCharacter* Character;
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
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Sword, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Sword, Meta = (AllowPrivateAccess = true))
		UAnimMontage* DrawSwordMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Sword, Meta = (AllowPrivateAccess = true))
		UAnimMontage* SwordBlockMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
		UAnimMontage* RollMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Move, Meta = (AllowPrivateAccess = true))
		UAnimMontage* Turn90Montage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Sword, Meta = (AllowPrivateAccess = true))
		UAnimMontage* SwordSkill_1Montage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Sword, Meta = (AllowPrivateAccess = true))
		UAnimMontage* SwordSkill_2Montage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Bow, Meta = (AllowPrivateAccess = true))
		UAnimMontage* DrawBowMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Bow, Meta = (AllowPrivateAccess = true))
		UAnimMontage* ShotBowMontage;

	//Bow
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Bow, Meta = (AllowPrivateAccess = true))
		UAnimMontage* BowMontage;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = CameraShacke, Meta = (AllowPrivateAccess = true))
		TSubclassOf<UCameraShakeBase> CameraShacke;


	UFUNCTION()
		void AnimNotify_AttackHitCheck();

	UFUNCTION()
		void AnimNotify_NextAttackCheck();
	UFUNCTION()
		void AnimNotify_EQUIP();
	UFUNCTION()
		void AnimNotify_sheath();
	UFUNCTION()
		void AnimNotify_DisableMovement();
	UFUNCTION()
		void AnimNotify_DisableInput();
	UFUNCTION()
		void AnimNotify_MoveForward();
	UFUNCTION()
		void AnimNotify_SlowMotionStart();
	UFUNCTION()
		void AnimNotify_SlowMotionEnd();
	UFUNCTION()
		void AnimNotify_StartDrawBow();
	UFUNCTION()
		void AnimNotify_BowFire();

	FName GetAttackMontageSectionName(int32 Section);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsDead;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bow, Meta = (AllowPrivateAccess = true))
		float UpperBodyRotationYaw;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bow, Meta = (AllowPrivateAccess = true))
		float UpperBodyRotationPitch;
	
	




};
