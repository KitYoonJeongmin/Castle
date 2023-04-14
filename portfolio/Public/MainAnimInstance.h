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
DECLARE_MULTICAST_DELEGATE(FOnBowSkill1Delegate);
DECLARE_MULTICAST_DELEGATE(FOnClimbEndDelegate);

UCLASS()
class PORTFOLIO_API UMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMainAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	//���� animation
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
	void PlayBowSkill1Montage();

	void PlayAssassinationMontage();

	void PlayLeftCornerMontage();
	void PlayRightCornerMontage();

	void PalyPainMontage();

	void JumpToAttackMontageSection(int32 NewSection);
	void JumpToSwordMontageSection(int32 NewSection);
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	FOnEQUIPDelegate OnEQUIP;
	FOnsheathDelegate Onsheath;
	FOnBowFireDelegate OnBowFire;
	FOnBowSkill1Delegate OnBowSkill1;
	FOnClimbEndDelegate OnClimbEnd;
	
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
	void SetRandomAttackMon();
private:
	int32 AttackMonIndex = 0;
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
		TArray<UAnimMontage*> AttackMontage;
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
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Bow, Meta = (AllowPrivateAccess = true))
		UAnimMontage* BowSkill1;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Hand, Meta = (AllowPrivateAccess = true))
		UAnimMontage* Assassination;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Climb, Meta = (AllowPrivateAccess = true))
		UAnimMontage* ClimbJumpLeft;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Climb, Meta = (AllowPrivateAccess = true))
		UAnimMontage* ClimbJumpRight;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Climb, Meta = (AllowPrivateAccess = true))
		UAnimMontage* CornerLeft;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Climb, Meta = (AllowPrivateAccess = true))
		UAnimMontage* CornerRight;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Climb, Meta = (AllowPrivateAccess = true))
		UAnimMontage* ClimbJumpUp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Climb, Meta = (AllowPrivateAccess = true))
		UAnimMontage* Pain_Montage;

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
	UFUNCTION()
		void AnimNotify_BowSkill1();
	UFUNCTION()
		void AnimNotify_ClimbEnd();
	UFUNCTION()
		void AnimNotify_JumpLeftEnd();
	UFUNCTION()
		void AnimNotify_JumpRightEnd();
	UFUNCTION()
		void AnimNotify_JumpUpEnd();
	UFUNCTION()
		void AnimNotify_Mount();
	UFUNCTION()
		void AnimNotify_Ride();
	UFUNCTION()
		void AnimNotify_Dismount();


	FName GetAttackMontageSectionName(int32 Section);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsDead;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bow, Meta = (AllowPrivateAccess = true))
		float UpperBodyRotationYaw;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bow, Meta = (AllowPrivateAccess = true))
		float UpperBodyRotationPitch;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Climb, Meta = (AllowPrivateAccess = true))
	bool CanMoveLeft = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Climb, Meta = (AllowPrivateAccess = true))
	bool CanMoveRight = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Climb, Meta = (AllowPrivateAccess = true))
	bool IsMovingLeft = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Climb, Meta = (AllowPrivateAccess = true))
	bool IsMovingRight = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Climb, Meta = (AllowPrivateAccess = true))
	bool IsClimbing = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Crouch, Meta = (AllowPrivateAccess = true))
	bool IsCrouch = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HandIK, Meta = (AllowPrivateAccess = true))
	FVector IKLeftHandLoc;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HandIK, Meta = (AllowPrivateAccess = true))
	FVector IKRightHandLoc;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Horse, Meta = (AllowPrivateAccess = true))
	bool IsMount = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Horse, Meta = (AllowPrivateAccess = true))
	bool CanMount = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Horse, Meta = (AllowPrivateAccess = true))
	float MountF = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Horse, Meta = (AllowPrivateAccess = true))
	float MountR = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Horse, Meta = (AllowPrivateAccess = true))
	float HorseSpeed = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Horse, Meta = (AllowPrivateAccess = true))
	float HorseRot = 0.f;
public:
	void JumpLeft(bool isJumpLeft);
	void JumpRight(bool isJumpRight);
	void JumpUp(bool jumpUp);

public:
	void SetCrouch(bool isCrouch) { IsCrouch = isCrouch; }
private:
	bool IsJumpRight = false;
	bool IsJumpLeft = false;
	bool IsJumpUp = false;



};
