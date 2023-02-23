// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "MainEnemyAIController.h"
#include "KnightEnemy.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto KnightEnemy = Cast<AKnightEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == KnightEnemy || !KnightEnemy->IsSetWeapon())
		return EBTNodeResult::Failed;


	KnightEnemy->Attack();
	IsAttacking = true;
	KnightEnemy->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;
	});

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); //°ø°ÝÀÌ ³¡³µÀ½À» ¾Ë·ÁÁÜ
	}
}