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
	static const FName DetectLevelKey;
	void StopAI();
	void StartAI();

	void StartDetect();
	void EndDetect();
	

public:
	UFUNCTION()
		void OnTargetDetected(AActor* actor, FAIStimulus const Stimulus);

	//AI Perception º¯¼ö
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AISightRadius =1100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AILoseSightRadius = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AIFieldOfView = 45.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AISightAge = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AILastSeenLocation = 0.f;
private:
	UPROPERTY()
		class UBehaviorTree* BTAsset;
	UPROPERTY()
		class UBlackboardData* BBAsset;
	UPROPERTY()
		class AKnightEnemy* KnightEnemy;


protected:

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleDefaultsOnly, Category = AIPerception)
		class UAISenseConfig_Sight* SightConfig;

private:
	bool isDetect = false;
	float DetectLevel = 0.f;
};
