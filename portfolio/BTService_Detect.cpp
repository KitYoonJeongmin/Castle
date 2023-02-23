// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "MainEnemyAIController.h"
#include "KnightEnemy.h"
#include "MainCharacter.h"

UBTService_IsInRange::UBTService_IsInRange()
{
	NodeName = TEXT("CanAttack");
	Interval = 1.0f;
}

void UBTService_IsInRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto KnightEnemy = Cast<AKnightEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == KnightEnemy)
		return ;

	UWorld* World = KnightEnemy->GetWorld();
	FVector Center = KnightEnemy->GetActorLocation();
	float DetectRadius = 650.0f;

	if (nullptr == World) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, KnightEnemy);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			AMainCharacter* MainCharacter = Cast<AMainCharacter>(OverlapResult.GetActor());
			if (MainCharacter && MainCharacter->GetController()->IsPlayerController())
			{
				
				
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMainEnemyAIController::TargetKey, MainCharacter);
				//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				//DrawDebugPoint(World, MainCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				//DrawDebugLine(World, KnightEnemy->GetActorLocation(), MainCharacter->GetActorLocation(), FColor::Blue, false, 0.27f);
				if (KnightEnemy->GetDistanceTo(MainCharacter) <= 300.0f)
				{
					KnightEnemy->GetCharacterMovement()->MaxWalkSpeed = 200.f;
				}
				else if(OwnerComp.GetBlackboardComponent()->GetValueAsFloat(AMainEnemyAIController::DetectLevelKey)==100.f)
				{
					KnightEnemy->GetCharacterMovement()->MaxWalkSpeed = 400.f;
				}
				
				return;
			}
		}
	}
	
	KnightEnemy->GetCharacterMovement()->MaxWalkSpeed = 200.f;
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMainEnemyAIController::TargetKey, nullptr);
	//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}
