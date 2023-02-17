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

	UPROPERTY(VisibleAnywhere, Category = Weapon)
		USkeletalMeshComponent* Weapon;


	void PlayDrawBowMon(bool IsDraw);
private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Bow, Meta = (AllowPrivateAccess = true))
		UAnimMontage* BowMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Bow, Meta = (AllowPrivateAccess = true))
		UAnimMontage* BowIdleMontage;
	class AArrow* Arrow;
};
