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
	static const FName CanHearWhistleKey;
	static const FName WhistleLocKey;
	void StopAI();
	void StartAI();

	void Sight(AActor* actor, FAIStimulus const Stimulus);
	void Hearing(AActor* actor, FAIStimulus const Stimulus);
	

public:
	UFUNCTION()
		void OnTargetDetected(AActor* actor, FAIStimulus const Stimulus);
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	//AI Perception º¯¼ö
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AISightRadius =2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AILoseSightRadius = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AIFieldOfView = 120.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AISightAge = 2.f;
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

	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = AIPerception)
		class UAISenseConfig_Sight* SightConfig;
	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly, Category = AIPerception)
		class UAISenseConfig_Hearing* HearingConfig;

private:
	bool isDetect = false;
	float Distance;
	float DetectLevel = 0.f;
	float TargetLevel = 0.f;
	TArray<float> DetectDistnace;
	IGenericTeamAgentInterface* targetTeam;
};
