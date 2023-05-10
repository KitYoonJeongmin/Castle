// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bow.generated.h"

UCLASS()
class PORTFOLIO_API ABow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	void PlayDrawBowMon(bool IsDraw, int8 fireIndex);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
		USkeletalMeshComponent* Weapon;
	
public:
	/**화살 생성*/
	void SpawnArrow();
	/**활의 기본화살 발사*/
	void DefaultFire();
	/**화살의 타입을 결정*/
	void SetArrowType();
	/**주변 적 다수에게 날릴 화살 준비*/
	void SpawnWideArrow();
	/**주변 적 다수에게 화살을 날림*/
	void WideFire();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Arrow)
	class AArrow* Arrow;
private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Bow, Meta = (AllowPrivateAccess = true))
		UAnimMontage* BowMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Bow, Meta = (AllowPrivateAccess = true))
		UAnimMontage* BowIdleMontage;
	
	uint8 ArrowType;

private:
	//wide fire
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WideFire, Meta = (AllowPrivateAccess = true))
		TArray<AActor*> WideEnemyArr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WideFire, Meta = (AllowPrivateAccess = true))
		TArray<AArrow*> WideArrowArr;
};
