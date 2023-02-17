// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KnightEnemy.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class PORTFOLIO_API AKnightEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKnightEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

public:
	/**공격시 실행*/
	FOnAttackEndDelegate OnAttackEnd;
	void Attack();
	void AttackStartComboState();
	void AttackEndComboState();

	void AttackCheck();

	/**무기를 손에 장착*/
	void SetWeapon(class AActor* NewWeapon, FName WeaponSocket);
	void UseSword();


	UPROPERTY(VisibleAnywhere, Category = Weapon)
		class ASword* Sword;

	void Block();
	bool IsSetWeapon() { return IsWeapon; }
public:
	UFUNCTION()
		/**ai가 다음 동작을 할 수 있도록 공격 montage가 끝났음을 알려줌 OnMontageEnded.AddDynamic의 인자로 사용*/
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;
	UPROPERTY()
		class UKnightEnemyAnimInstance* EnemyAnim;
	bool IsWeapon;
	bool IsBlock;

	

protected:
	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool CanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 CurrentCombo;
private:
	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float HealthPoint;
};
