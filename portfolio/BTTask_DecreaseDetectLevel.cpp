// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_DecreaseDetectLevel.h"
#include "MainEnemyAIController.h"

UBTTask_DecreaseDetectLevel::UBTTask_DecreaseDetectLevel()
{
	NodeName = TEXT("DecreaseDetectLevel");
}

EBTNodeResult::Type UBTTask_DecreaseDetectLevel::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	Cast<AMainEnemyAIController>(OwnerComp.GetAIOwner())->UpdateDetectLevel(false);
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
