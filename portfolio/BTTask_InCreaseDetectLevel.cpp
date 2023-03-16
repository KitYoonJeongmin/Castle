// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_InCreaseDetectLevel.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainEnemyAIController.h"
#include "KnightEnemy.h"

UBTTask_InCreaseDetectLevel::UBTTask_InCreaseDetectLevel()
{
	NodeName = TEXT("InCreaseDetectLevel");
}

EBTNodeResult::Type UBTTask_InCreaseDetectLevel::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto AIController = Cast<AMainEnemyAIController>(OwnerComp.GetAIOwner());
	auto KnightEnemy = Cast<AKnightEnemy>(AIController->GetPawn());
	AIController->UpdateDetectLevel(true);

	auto Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMainEnemyAIController::TargetKey));
	if (nullptr == Target)
		return EBTNodeResult::Failed;
	FRotator TurnRot = UKismetMathLibrary::FindLookAtRotation(KnightEnemy->GetActorLocation(), Target->GetActorLocation());
	KnightEnemy->SetActorRotation(TurnRot);
	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
