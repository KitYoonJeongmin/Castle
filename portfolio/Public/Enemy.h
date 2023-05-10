// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS(ABSTRACT)
class PORTFOLIO_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void PostInitializeComponents() override;

public:
	//Attack
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;
	UFUNCTION()
		virtual void Attack() PURE_VIRTUAL(AEnemy::Attack, );
	void AttackCheck();
public:
	//hp bar visible
	void EnableHPBar(bool isEnable);
	//Damage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Dead();
	bool IsDead = false;
public:
	void EnableTargetOnWidget(bool IsEnable);
protected:
	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = HealthPoint, Meta = (AllowPrivateAccess = true))
		float HealthPoint;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = UI, Meta = (AllowPrivateAccess = true))
		class UWidgetComponent* HPBarWidget;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Anim, Meta = (AllowPrivateAccess = true))
		class UKnightEnemyAnimInstance* EnemyAnim;
	UFUNCTION()
		virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	float Damage;
	
private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = UI, Meta = (AllowPrivateAccess = true))
		class UWidgetComponent* TargetOn;
};
