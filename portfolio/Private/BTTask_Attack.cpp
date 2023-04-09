// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "MainEnemyAIController.h"
#include "KnightEnemy.h"
#include "Enemy.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;

}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	AKnightEnemy* KnightEnemy = Cast<AKnightEnemy>(Enemy);

	if (nullptr == Enemy ||(KnightEnemy!= nullptr && !KnightEnemy->IsSetWeapon()))
		return EBTNodeResult::Failed;
	
	if (KnightEnemy!= nullptr &&!KnightEnemy->IsSetWeapon())
		KnightEnemy->UseSword();
	
	Enemy->Attack();

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!Enemy->IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); //������ �������� �˷���
	}
}