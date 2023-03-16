// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_SetMaxSpeed.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTDecorator_SetMaxSpeed::UBTDecorator_SetMaxSpeed()
{
	NodeName = TEXT("ChangeSpeed");
}

bool UBTDecorator_SetMaxSpeed::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn())->GetCharacterMovement()->MaxWalkSpeed = TargetSpeed;
	return bResult;
}
