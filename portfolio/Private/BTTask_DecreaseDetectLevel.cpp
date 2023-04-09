// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_DecreaseDetectLevel.h"
#include "MainEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_DecreaseDetectLevel::UBTTask_DecreaseDetectLevel()
{
	NodeName = TEXT("DecreaseDetectLevel");
}

EBTNodeResult::Type UBTTask_DecreaseDetectLevel::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	AMainEnemyAIController* Controller = Cast<AMainEnemyAIController>(OwnerComp.GetAIOwner());
	Controller->UpdateDetectLevel(false);
	if (Controller->DetectLevel <1.f)
	{
		Controller->GetBlackboardComponent()->SetValueAsObject(AMainEnemyAIController::TargetKey, nullptr);
	}
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
