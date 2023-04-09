// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdateLastTargetLoc.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MainEnemyAIController.h"
UBTService_UpdateLastTargetLoc::UBTService_UpdateLastTargetLoc()
{
	NodeName = TEXT("UpdateLastLoc");
	Interval = 1.0f;
}

void UBTService_UpdateLastTargetLoc::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	auto Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMainEnemyAIController::TargetKey));
	if(Target != nullptr)
		OwnerComp.GetBlackboardComponent()->SetValueAsVector("LastTargetLoc", Target->GetActorLocation());
}
