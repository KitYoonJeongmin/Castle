// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);
UENUM(BlueprintType)
enum class EWeapon : uint8
{
	Hand = 0 UMETA(DisplayName = "Hand"),
	Sword = 1 UMETA(DisplayName = "Sword"),
	Bow = 2 UMETA(DisplayName = "Bow"),
};
UCLASS()
class PORTFOLIO_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	AMainCharacter();
	/**기본적인 회전값을 저장할 변수*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/**기본적인 위아래값을 저장할 변수*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float CurrentVelocity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float TargetVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	FVector MoveDirection;


public:	
	/**카메라가 캐릭터 뒤에 위치*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess))
		class USpringArmComponent* CameraBoom;

	/** 따라다니는 카메라*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	

	virtual void PostInitializeComponents() override;

	
public:

	/**공격시 실행*/
	FOnAttackEndDelegate OnAttackEnd;
	void Attack();
	void AttackRelease();
	void AttackStartComboState();
	void AttackEndComboState();

	void AttackCheck();

	/**무기를 손에 장착*/
	void SwitchWeapon(class AActor* NewWeapon, FName WeaponSocket);
	void UseSword();
	void UseBow();
	void FindNearEnemy();


	UPROPERTY(VisibleAnywhere, Category = Weapon)
		class ASword* Sword;
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		class ABow* Bow;

	void Block();
	
	
public:
	void ZoomButtonPressed();
	void ZoomButtonReleased();
	void ZoomFunction(float DeltaTime);
	
public:
	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UMainCharacterStatComponent* Characterstat;


public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	EWeapon CurrentWeapon = EWeapon::Hand;
	void SetWeapon(EWeapon Weapon);

	/** 앞, 뒤 입력시 Call */
	void MoveForward(float Value);
	/** 좌, 우 입력시 Call */
	void MoveRight(float Value);
	void Jump();
	/**최종 움직임 함수*/
	void Move(float DeltaTime);
	/**변화된 회전값이 입력시 Call*/
	void Turn(float Rate);
	/**변화된 위아래값이 입력시 Call*/
	void LookUp(float Rate);

	/**Shift 입력시*/
	void SetRunSpeed() { TargetVelocity = 150.0f; }
	void SetWalkSpeed(){ TargetVelocity = 70.0f; }

	void Shot();
	FHitResult LineTrace();
	void Skill1();
	void Skill2();

	float FindBetweenAngle(FVector OwnerLocation, FVector OwnerForwardVec,FVector TargetLocation);
public:
	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
public:
	
	UFUNCTION()
		/**화살 변경*/
		void ArrowChangePlus();
	UFUNCTION()
		void SetManaPoint(float PlusPoint);
public:
	class UClimbingComponent* GetClimbingComponent();
	/**벽오르기*/
	void ClimbUp();
	/**벽 내리기*/
	void ClimbDown();
	/**코너왼쪽*/
	void ClimbCornerLeft();
	/**코너오른쪽*/
	void ClimbCornerRight();

public:
	void ToggleCrouch();

protected:
	UPROPERTY(VisibleAnywhere)
		class UClimbingComponent* ClimbingComponent;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;
	UPROPERTY()
		class UMainAnimInstance* MainAnim;

	bool IsBlock;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	
protected:
	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool CanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 CurrentCombo;
protected:
	//카메라 zoom
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zoom", meta = (AllowPrivateAccess = true))
		float DefaultFOV;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zoom", meta = (AllowPrivateAccess = true))
		float CurrentFOV;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zoom", meta = (AllowPrivateAccess = true))
		float ZoomFOV;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zoom", meta = (AllowPrivateAccess = true))
		float DefaultCL;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zoom", meta = (AllowPrivateAccess = true))
		float CurrentCL;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zoom", meta = (AllowPrivateAccess = true))
		float ZoomCL;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zoom", meta = (AllowPrivateAccess = true))
		float ZoomSpeed;
	bool isZooming;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
		float HealthPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
		float ManaPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
		USoundBase* BlockSound;
	float ArmLengthTo;
private:
	AActor* NearEnemy;
	class AArrow* Arrow;
	uint8 ArrowType=0;
	class AMainGameModeBase* MainGameMode;

};
