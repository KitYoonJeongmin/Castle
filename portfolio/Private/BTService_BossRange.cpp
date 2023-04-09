// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_BossRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTService_BossRange::UBTService_BossRange()
{
	NodeName = TEXT("AttackRange");
	Interval = 1.0f;
}
void UBTService_BossRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto ControllingPawn = (OwnerComp.GetAIOwner()->GetPawn());
	auto Target = Cast<ACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Target"));
	if (ControllingPawn->GetDistanceTo((AActor*)Target) <= 700.f)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum("AttackRange",0);//near
	}
	else if (ControllingPawn->GetDistanceTo((AActor*)Target) <= 1400.f)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum("AttackRange", 1);//Medium
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum("AttackRange", 2);//Far
	}
}