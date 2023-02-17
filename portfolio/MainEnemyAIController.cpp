// Fill out your copyright notice in the Description page of Project Settings.


#include "MainEnemyAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainCharacter.h"
#include "KnightEnemy.h"

const FName AMainEnemyAIController::HomePosKey(TEXT("HomePos"));
const FName AMainEnemyAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AMainEnemyAIController::TargetKey(TEXT("Target"));
const FName AMainEnemyAIController::CanSeePlayerKey(TEXT("CanSeePlayer"));
const FName AMainEnemyAIController::IsDeadKey(TEXT("IsDead"));


AMainEnemyAIController::AMainEnemyAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/AMyDirectory/Enemy/AI/BB_MainEnemy.BB_MainEnemy'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/AMyDirectory/Enemy/AI/BT_MainEnemy.BT_MainEnemy'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
	//AI Perception을 사용하기 위해 AIPerceitonComponent 설정
	SetPerceptionComponent(*CreateOptionalDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception")));

	//AI Perception::Sight 설정
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	SightConfig->SightRadius = AISightRadius; // 시각 반경
	SightConfig->LoseSightRadius = SightConfig->SightRadius + AILoseSightRadius; // 시각 상실 반경
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView; // 주변 시야각
	SightConfig->SetMaxAge(AISightAge); //자극이 잊히기 까지의 시간 (0이면 잊지않음)
	SightConfig->AutoSuccessRangeFromLastSeenLocation = AILastSeenLocation;	//마지막으로 본 위치의 해당 범위내에 있으면 이미 본 대상을 항상 볼 수 있음

	SightConfig->DetectionByAffiliation.bDetectEnemies = true; // 소속별 탐지 적
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true; // 소속별 탐지 팀
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true; // 소속별 탐지 중립
	
	GetPerceptionComponent()->SetDominantSense(SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AMainEnemyAIController::OnTargetDetected); //지정된 대상에 대해 인식 정보가 업데이트되었음을 ​​바인딩된 모든 개체에 알림
}

void AMainEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp,Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}
	if (Cast<AKnightEnemy>(InPawn))
	{
		KnightEnemy = Cast<AKnightEnemy>(InPawn);
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't possess to knightEnemy"));
	}
	
	
}


void AMainEnemyAIController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (IsValid(BehaviorTreeComponent))
	{
		//UE_LOG(LogTemp, Warning, TEXT("-------Stop---------"));
		Blackboard->SetValueAsBool(IsDeadKey, true);
		BehaviorTreeComponent->StopLogic(FString::Printf(TEXT("")));
	}
}

void AMainEnemyAIController::StartAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (IsValid(BehaviorTreeComponent))
	{
		//UE_LOG(LogTemp, Warning, TEXT("-------Start---------"));
		Blackboard->SetValueAsBool(IsDeadKey, false);
		BehaviorTreeComponent->StartLogic();
	}
}

void AMainEnemyAIController::OnTargetDetected(AActor* actor, FAIStimulus const Stimulus)
{
	if (Blackboard->GetValueAsBool(IsDeadKey)) return;
	if (auto const player = Cast<AMainCharacter>(actor))
	{
		
		if (!KnightEnemy->IsSetWeapon())
		{
			KnightEnemy->UseSword();
		}
		if (Stimulus.WasSuccessfullySensed())
		{
			KnightEnemy->GetCharacterMovement()->MaxWalkSpeed = 400.f;

		}
		//성공적으로 감지하면 블랙보드에 bool값을 넣어준다.
		Blackboard->SetValueAsBool(CanSeePlayerKey, Stimulus.WasSuccessfullySensed());
		Blackboard->SetValueAsObject(TargetKey, player);

	}
	if(!Stimulus.WasSuccessfullySensed())
	{
		Blackboard->SetValueAsObject(TargetKey, nullptr);
		if (KnightEnemy->IsSetWeapon())
		{
			KnightEnemy->UseSword();
		}
		KnightEnemy->GetCharacterMovement()->MaxWalkSpeed = 200.f;
	}
	
}





