// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAnimInstance.h"


UBossAnimInstance::UBossAnimInstance()
{
	CurrentPawnSpeed = 0.f;
	IsDead = false;

	//Enemy Montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ENEMY_ATTACK_MONTAGE(TEXT("AnimMontage'/Game/Resources/PlayerCharacter/GKnight/Animation/Skill/AM_RegularAttack.AM_RegularAttack'"));
	if (ENEMY_ATTACK_MONTAGE.Succeeded())
	{
		EnemyAttackMontage = ENEMY_ATTACK_MONTAGE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> BLOCK_MONTAGE(TEXT("AnimMontage'/Game/Resources/PlayerCharacter/GKnight/Animation/Mon_Block.Mon_Block'"));
	if (BLOCK_MONTAGE.Succeeded())
	{
		EnemySwordBlockMontage = BLOCK_MONTAGE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MEDIUMSKILL_MONTAGE(TEXT("AnimMontage'/Game/Resources/PlayerCharacter/GKnight/Animation/Skill/Mon_MediumSkill.Mon_MediumSkill'"));
	if (MEDIUMSKILL_MONTAGE.Succeeded())
	{
		MediumSkill = MEDIUMSKILL_MONTAGE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> FARSKILL_MONTAGE(TEXT("AnimMontage'/Game/Resources/PlayerCharacter/GKnight/Animation/Skill/Mon_FarSkill.Mon_FarSkill'"));
	if (FARSKILL_MONTAGE.Succeeded())
	{
		FarSkill = FARSKILL_MONTAGE.Object;
	}
	
	
}

void UBossAnimInstance::PalyMediumSkillMon()
{
	check(!IsDead);
	Montage_Play(MediumSkill, 1.2f);
}

void UBossAnimInstance::PalyFarSkillMon()
{
	check(!IsDead);
	Montage_Play(FarSkill, 1.2f);
}

void UBossAnimInstance::AnimNotify_MoveToTarget()
{
	PauseMon = (UAnimMontage*)0;
	Montage_Pause(PauseMon);

	OnMoveToTarget.Broadcast();

	FTimerHandle GravityTimerHandle;
	float GravityTime=0.08f;
	GetWorld()->GetTimerManager().SetTimer(GravityTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			Montage_Resume(PauseMon);
		}), GravityTime, false);
}
