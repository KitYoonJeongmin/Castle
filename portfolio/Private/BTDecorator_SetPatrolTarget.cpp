// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_SetPatrolTarget.h"
#include "Components/SplineComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MainEnemyAIController.h"
#include "KnightEnemy.h"

UBTDecorator_SetPatrolTarget::UBTDecorator_SetPatrolTarget()
{
	NodeName = TEXT("FindPatrolPos");
}

bool UBTDecorator_SetPatrolTarget::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	auto Controller = Cast<AMainEnemyAIController>(OwnerComp.GetAIOwner());
	auto ControllingPawn = Cast<AKnightEnemy>(Controller->GetPawn());

	if (nullptr == ControllingPawn) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Enemy Initialize faild!"));
		return EBTNodeResult::Failed;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Set PatrolPosKey"));
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(AMainEnemyAIController::PatrolPosKey, ControllingPawn->GetNextLocaiton());
	
	return bResult;
}
