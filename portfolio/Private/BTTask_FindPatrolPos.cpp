// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPos.h"
#include "Components/SplineComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MainEnemyAIController.h"
#include "KnightEnemy.h"
UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Controller = Cast<AMainEnemyAIController>(OwnerComp.GetAIOwner());
	auto ControllingPawn = Cast<AKnightEnemy>(Controller->GetPawn());

	if (nullptr == ControllingPawn) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Enemy Initialize faild!"));
		return EBTNodeResult::Failed;
	}
	
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(AMainEnemyAIController::PatrolPosKey, ControllingPawn->GetNextLocaiton());
	if (ControllingPawn->PatrolCheckPoint)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat("CheckPointWaitTime", 5.f);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat("CheckPointWaitTime", 0.f);
	}
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}