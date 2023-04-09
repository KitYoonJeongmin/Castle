// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BossMediumAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "BossCharacter.h"
UBTTask_BossMediumAttack::UBTTask_BossMediumAttack()
{
	NodeName = TEXT("SkillAttack");
}

EBTNodeResult::Type UBTTask_BossMediumAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto Controller = OwnerComp.GetAIOwner();
	auto BossCharacter = Cast<ABossCharacter>(Controller->GetPawn());
	int32 Skill = (int32)Controller->GetBlackboardComponent()->GetValueAsEnum("AttackRange");
	if(Skill==1)
		BossCharacter->MediumSkill();
	else if(Skill==2)
		BossCharacter->FarSkill();
	return EBTNodeResult::Type();
}
