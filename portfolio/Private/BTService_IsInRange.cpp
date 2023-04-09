// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_IsInRange.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "MainEnemyAIController.h"
#include "KnightEnemy.h"
#include "MainCharacter.h"

UBTService_IsInRange::UBTService_IsInRange()
{
	NodeName = TEXT("DetactRange");
	Interval = 1.0f;
}

void UBTService_IsInRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto KnightEnemy = Cast<AKnightEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == KnightEnemy)
		return ;
	auto Target = Cast<AMainCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMainEnemyAIController::TargetKey));
	if (Target == nullptr)
		return;

	if (KnightEnemy->GetDistanceTo(Target) <= 400.f)
	{
		Cast<AMainEnemyAIController>(KnightEnemy->GetController())->DetectLevel = 100.f;
		if (!KnightEnemy->IsSetWeapon())
			KnightEnemy->UseSword();
	}
	return;
}
