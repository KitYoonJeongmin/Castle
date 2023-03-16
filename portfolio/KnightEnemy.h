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
	/**���ݽ� ����*/
	FOnAttackEndDelegate OnAttackEnd;
	void Attack();
	void AttackStartComboState();
	void AttackEndComboState();

	void AttackCheck();

	/**���⸦ �տ� ����*/
	void SetWeapon(class AActor* NewWeapon, FName WeaponSocket);
	void UseSword();


	UPROPERTY(VisibleAnywhere, Category = Weapon)
		class ASword* Sword;

	void Block();
	bool IsSetWeapon() { return IsWeapon; }
	//�ϻ�
	void PlayAssassination();
	//hp bar visible
	void EnableHPBar(bool isEnable);

	void EnableDetectBar(bool isEnable);
	void UpdateDetectBar(float DetectLevel);

	//Patrolling
	UPROPERTY(VisibleAnyWhere, Category = Patrol)
	class ASpline* PatrolPath;
	bool PatrolCheckPoint = false;
	FVector GetNextLocaiton();

public:
	UFUNCTION()
		/**ai�� ���� ������ �� �� �ֵ��� ���� montage�� �������� �˷��� OnMontageEnded.AddDynamic�� ���ڷ� ���*/
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
	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = HealthPoint, Meta = (AllowPrivateAccess = true))
		float HealthPoint;
	UPROPERTY(VisibleAnyWhere, Category = UI)
		class UWidgetComponent* HPBarWidget;
	UPROPERTY(VisibleAnyWhere, Category = UI)
		class UWidgetComponent* DetectWidget;

	//Patroling
private:
	float PatrolingDistance = 1.f;
	float DistanceChange = 200.f;
	int32 PointIndex = 0;
	int32 AddIndex= 1;
};
