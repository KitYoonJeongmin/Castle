// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainEnemyAIController.h"
#include "KnightEnemy.h"
#include "MainCharacter.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{

	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	AKnightEnemy* ControllingPawn = Cast<AKnightEnemy>(OwnerComp.GetAIOwner()->GetCharacter());
	if (nullptr == ControllingPawn)
		return false;
	auto Target = Cast<AMainCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMainEnemyAIController::TargetKey));
	if (nullptr == Target)
		return false;

	bResult = (Target->GetDistanceTo(ControllingPawn) <= 500.0f);
	if (bResult&& ControllingPawn->GetCharacterMovement()->MaxWalkSpeed>200.f)
	{
		ControllingPawn->GetCharacterMovement()->MaxWalkSpeed = 200.f;
	}

	return bResult;
}
