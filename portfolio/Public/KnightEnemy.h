// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "GameFramework/Character.h"
#include "KnightEnemy.generated.h"

UCLASS()
class PORTFOLIO_API AKnightEnemy : public AEnemy
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
	virtual void PostInitializeComponents() override;

public:
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		class ASword* Sword;
public:
	/**Sword*/
	bool IsSetWeapon() { return IsWeapon; }
	void SetWeapon(class AActor* NewWeapon, FName WeaponSocket);
	void UseSword();
	virtual void Attack() override;
	
	//Assassination
	void PlayAssassination();
	
	//Detect
	void EnableDetectBar(bool isEnable);
	void UpdateDetectBar(float DetectLevel);

	//Patrolling
	UPROPERTY(VisibleAnyWhere, Category = Patrol)
	class ASpline* PatrolPath;
	bool PatrolCheckPoint = false;
	FVector GetNextLocaiton();

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Dead() override;

protected:
	bool IsWeapon;
	bool IsBlock;
protected:
	UPROPERTY(VisibleAnyWhere, Category = UI)
		class UWidgetComponent* DetectWidget;

	//Patroling
private:
	float PatrolingDistance = 1.f;
	float DistanceChange = 200.f;
	int32 PointIndex = 0;
	int32 AddIndex= 1;

};
