// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SetPatrolPos.h"
#include "Components/SplineComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MainEnemyAIController.h"
#include "KnightEnemy.h"
UBTService_SetPatrolPos::UBTService_SetPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
	Interval = 1.0f;
}

void UBTService_SetPatrolPos::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	auto Controller = Cast<AMainEnemyAIController>(OwnerComp.GetAIOwner());
	auto ControllingPawn = Cast<AKnightEnemy>(Controller->GetPawn());

	if (nullptr == ControllingPawn) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Enemy Initialize faild!"));
		return;
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
	
	return;
}
