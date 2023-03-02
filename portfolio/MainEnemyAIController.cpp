// Fill out your copyright notice in the Description page of Project Settings.


#include "MainEnemyAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "MainCharacter.h"
#include "KnightEnemy.h"

const FName AMainEnemyAIController::HomePosKey(TEXT("HomePos"));
const FName AMainEnemyAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AMainEnemyAIController::TargetKey(TEXT("Target"));
const FName AMainEnemyAIController::CanSeePlayerKey(TEXT("CanSeePlayer"));
const FName AMainEnemyAIController::IsDeadKey(TEXT("IsDead"));
const FName AMainEnemyAIController::DetectLevelKey(TEXT("DetectLevel"));
const FName AMainEnemyAIController::CanHearWhistleKey(TEXT("CanHearWhistle"));
const FName AMainEnemyAIController::WhistleLocKey(TEXT("WhistleLoc"));

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
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false; // 소속별 탐지 팀
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true; // 소속별 탐지 중립
	
	GetPerceptionComponent()->SetDominantSense(SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AMainEnemyAIController::OnTargetDetected); //지정된 대상에 대해 인식 정보가 업데이트되었음을 ​​바인딩된 모든 개체에 알림

	//AI Perception::Hearing 설정
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));

	HearingConfig->HearingRange = 3000.f;	//범위 설정
	HearingConfig->LoSHearingRange = 3000.f;	//범위 디버거
	HearingConfig->SetMaxAge(1.f);

	HearingConfig->DetectionByAffiliation.bDetectEnemies = true; // 소속별 탐지 적
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true; // 소속별 탐지 팀
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true; // 소속별 탐지 중립

	GetPerceptionComponent()->ConfigureSense(*HearingConfig);	//감각 추가

	//Detect Distance
	DetectDistnace.Add(600.f);
	DetectDistnace.Add(1200.f);
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
	//UE_LOG(LogTemp, Warning, TEXT("Sense Update: %f"), Stimulus.GetAge());
	if (Blackboard->GetValueAsBool(IsDeadKey)) return;
	
	switch (Stimulus.Type) 
	{
	case 0:	//Sight
		Sight(actor, Stimulus);
		break;
	case 1:	//Hearing
		
		Hearing(actor, Stimulus);
		break;
	}
}

ETeamAttitude::Type AMainEnemyAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (APawn const* OtherPawn = Cast<APawn>(&Other))
	{
		if (auto const TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			if (TeamAgent->GetGenericTeamId() == FGenericTeamId(1))
			{
				return ETeamAttitude::Hostile;
			}
			else if(TeamAgent->GetGenericTeamId() == FGenericTeamId(3))
			{
				return ETeamAttitude::Friendly;
			}
		}
	}
	return ETeamAttitude::Neutral;
	return ETeamAttitude::Type();
}

void AMainEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (isDetect && Blackboard->GetValueAsObject(TargetKey) != nullptr && targetTeam != nullptr && targetTeam->GetGenericTeamId() == FGenericTeamId(1))
	{

		DrawDebugLine(GetWorld(), KnightEnemy->GetActorLocation(), Cast<AActor>(Blackboard->GetValueAsObject(TargetKey))->GetActorLocation(), FColor::Blue, false, 0.27f);
		if (Blackboard->GetValueAsObject(TargetKey))
		{
			Distance = FVector::Distance(KnightEnemy->GetActorLocation(), Cast<AActor>(Blackboard->GetValueAsObject(TargetKey))->GetActorLocation());
		}
		else { Distance = 1000000000.f; }

		if (Distance < DetectDistnace[0])
		{
			DetectLevel = 100.f;
			TargetLevel = 100.f;

		}
		else if (Distance < DetectDistnace[1])
		{
			TargetLevel = 100.f;
		}
		else
		{
			TargetLevel = 0.f;
		}

	}
	DetectLevel = FMath::Lerp(DetectLevel, TargetLevel, 0.005f);
	//DetectBar
	if (DetectLevel <= 1.f)
	{
		Blackboard->SetValueAsBool(CanSeePlayerKey, false);
		Blackboard->SetValueAsObject(TargetKey, nullptr);
		KnightEnemy->EnableDetectBar(false);
		if (KnightEnemy->IsSetWeapon())
		{
			KnightEnemy->UseSword();
		}
	}
	else
	{
		KnightEnemy->EnableDetectBar(true);
	}
	//속도
	if (DetectLevel >= 99.f)
	{
		if (!KnightEnemy->IsSetWeapon())
		{
			KnightEnemy->UseSword();
		}
		KnightEnemy->GetCharacterMovement()->MaxWalkSpeed = FMath::Lerp(KnightEnemy->GetCharacterMovement()->MaxWalkSpeed, 400.f, 0.02f);
	}
	else
	{
		KnightEnemy->GetCharacterMovement()->MaxWalkSpeed = FMath::Lerp(KnightEnemy->GetCharacterMovement()->MaxWalkSpeed, 200.f, 0.02f);
		if (targetTeam != nullptr && targetTeam->GetGenericTeamId() != FGenericTeamId(1))
		{
			isDetect = false;
			TargetLevel = 0.f;
		}
	}
	KnightEnemy->UpdateDetectBar(DetectLevel);
	Blackboard->SetValueAsFloat(DetectLevelKey, DetectLevel);

	
}

void AMainEnemyAIController::Sight(AActor* actor, FAIStimulus const Stimulus)
{
	if (Cast<AMainCharacter>(actor))
	{
		Blackboard->SetValueAsBool(CanSeePlayerKey, true);
		Blackboard->SetValueAsObject(TargetKey, actor);
		targetTeam = Cast<IGenericTeamAgentInterface>(Cast<APawn>(actor)->GetController());
		isDetect = true;
	}

	if(!Stimulus.WasSuccessfullySensed())
	{
		isDetect = false;
		//Blackboard->SetValueAsBool(CanSeePlayerKey, false);
		//Blackboard->SetValueAsObject(TargetKey, nullptr);
		TargetLevel = 0.f;
	}
}

void AMainEnemyAIController::Hearing(AActor* actor, FAIStimulus const Stimulus)
{
	//UE_LOG(LogTemp, Warning, TEXT("Hearing"));
	if (Stimulus.Tag == FName("Whistle"))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hearing"));
		Blackboard->SetValueAsVector(WhistleLocKey, actor->GetActorLocation());
		Blackboard->SetValueAsBool(CanHearWhistleKey, true);
	}
	if(!Stimulus.WasSuccessfullySensed())
	{
		//Blackboard->SetValueAsVector(WhistleLocKey, newVe);
		Blackboard->SetValueAsBool(CanHearWhistleKey, false);
	}
}



