// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPos.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "MainEnemyAIController.h"
UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Controller = Cast<AMainEnemyAIController>(OwnerComp.GetAIOwner());
	auto ControllingPawn = Controller->GetPawn();
	if (nullptr == ControllingPawn)
		return EBTNodeResult::Failed;

	if (nullptr == ControllingPawn) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Enemy Initialize faild!"));
		return EBTNodeResult::Failed;
	}
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (nullptr == NavSystem)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Enemy in Navi"));
		return EBTNodeResult::Failed;
	}
		
	FVector Origin = ControllingPawn->GetActorLocation();
	FNavLocation NextPatrol;

	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 1500.0f, NextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AMainEnemyAIController::PatrolPosKey, NextPatrol.Location);

	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s"), *NextPatrol.Location.ToString()));
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}