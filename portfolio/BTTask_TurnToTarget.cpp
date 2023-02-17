// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToTarget.h"
#include "MainEnemyAIController.h"
#include "KnightEnemy.h"
#include "MainCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto CurrentEnemy = Cast<AKnightEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == CurrentEnemy)
		return EBTNodeResult::Failed;

	auto Target = Cast<AMainCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMainEnemyAIController::TargetKey));
	if (nullptr == Target)
		return EBTNodeResult::Failed;

	FVector LookVector = Target->GetActorLocation() - CurrentEnemy->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	CurrentEnemy->SetActorRotation(FMath::RInterpTo(CurrentEnemy->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	return EBTNodeResult::Succeeded;
}