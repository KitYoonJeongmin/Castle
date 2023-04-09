// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdateDetectLevel.h"
#include "MainEnemyAIController.h"

UBTService_UpdateDetectLevel::UBTService_UpdateDetectLevel()
{
	NodeName = TEXT("UpdateDetectLevel");
	Interval = 1.0f;
}

void UBTService_UpdateDetectLevel::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	Cast<AMainEnemyAIController>(OwnerComp.GetAIOwner())->UpdateDetectLevel(CanIncrease);
}
