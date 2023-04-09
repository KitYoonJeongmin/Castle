// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetStrafeLocation.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "AIController.h"
#include "MainEnemyAIController.h"
#include "KnightEnemy.h"
UBTTask_GetStrafeLocation::UBTTask_GetStrafeLocation()
{

}

EBTNodeResult::Type UBTTask_GetStrafeLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto KnightEnemy = Cast<AKnightEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (KnightEnemy->IsAttacking) return EBTNodeResult::Failed;
	KnightEnemy->GetCharacterMovement()->MaxWalkSpeed = 200.f;
	int32 Random = FMath::RandRange(0, 2);
	FVector StrafeLoc = KnightEnemy->GetActorLocation();
	switch (Random)
	{
	case 1:
		StrafeLoc += KnightEnemy->GetActorForwardVector() * 50.f + KnightEnemy->GetActorRightVector() * 350.f;
		break;
	case 2:
		StrafeLoc += KnightEnemy->GetActorForwardVector() * 50.f + KnightEnemy->GetActorRightVector() * -350.f;
		break;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(AMainEnemyAIController::StrafeLocKey, StrafeLoc);
	DrawDebugLine(GetWorld(), KnightEnemy->GetActorLocation(), StrafeLoc, FColor::Cyan, false, 0.27f);
	return EBTNodeResult::Type();
}
