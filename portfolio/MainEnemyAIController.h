// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "MainEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_API AMainEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:
	AMainEnemyAIController();
	virtual void OnPossess(APawn* InPawn) override;
	

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
	static const FName CanSeePlayerKey;
	static const FName IsDeadKey;
	void StopAI();
	void StartAI();


	

public:
	UFUNCTION()
		void OnTargetDetected(AActor* actor, FAIStimulus const Stimulus);

	//AI Perception º¯¼ö
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AISightRadius = 1500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AILoseSightRadius = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AIFieldOfView = 90.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AISightAge = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AILastSeenLocation = 900.f;
private:
	UPROPERTY()
		class UBehaviorTree* BTAsset;
	UPROPERTY()
		class UBlackboardData* BBAsset;
	UPROPERTY()
		class AKnightEnemy* KnightEnemy;

protected:

	UPROPERTY(VisibleDefaultsOnly, Category = AIPerception)
		class UAISenseConfig_Sight* SightConfig;
};
