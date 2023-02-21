// Fill out your copyright notice in the Description page of Project Settings.


#include "KnightEnemyAnimInstance.h"
#include "KnightEnemy.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

UKnightEnemyAnimInstance::UKnightEnemyAnimInstance()
{
	CurrentPawnSpeed = 0.f;
	IsDead = false;

	//Enemy Montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ENEMY_ATTACK_MONTAGE(TEXT("AnimMontage'/Game/AMyDirectory/Enemy/Animations/Mon_Attack.Mon_Attack'"));
	if (ENEMY_ATTACK_MONTAGE.Succeeded())
	{
		EnemyAttackMontage = ENEMY_ATTACK_MONTAGE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ENEMY_DrawSword_MONTAGE(TEXT("AnimMontage'/Game/AMyDirectory/Enemy/Animations/DrawSwordMontage.DrawSwordMontage'"));
	if (ENEMY_DrawSword_MONTAGE.Succeeded())
	{
		EnemyDrawSwordMontage = ENEMY_DrawSword_MONTAGE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ENEMY_SWORD_BLOCK_MONTAGE(TEXT("AnimMontage'/Game/AMyDirectory/Enemy/Animations/Mon_SwordBlock.Mon_SwordBlock'"));
	if (ENEMY_SWORD_BLOCK_MONTAGE.Succeeded())
	{
		EnemySwordBlockMontage = ENEMY_SWORD_BLOCK_MONTAGE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PAIN_MONTAGE(TEXT("AnimMontage'/Game/AMyDirectory/Enemy/Animations/Mon_Pain.Mon_Pain'"));
	if (PAIN_MONTAGE.Succeeded())
	{
		EnemyPainMontage = PAIN_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> NUCK_MONTAGE(TEXT("AnimMontage'/Game/AMyDirectory/Enemy/Animations/Mon_Nuck.Mon_Nuck'"));
	if (NUCK_MONTAGE.Succeeded())
	{
		EnemyNuckMontage = NUCK_MONTAGE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ASSASSINATION_ANIM(TEXT("AnimMontage'/Game/AMyDirectory/Enemy/Animations/Mon_Stealth_Assassination_Enemy.Mon_Stealth_Assassination_Enemy'"));
	if (ASSASSINATION_ANIM.Succeeded())
	{
		AssassinationAnim = ASSASSINATION_ANIM.Object;
	}
	
}

void UKnightEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	Character = Cast<AKnightEnemy>(TryGetPawnOwner());

	if (!::IsValid(Character)) { return; }

	if (!IsDead)
	{
		CurrentPawnSpeed = Character->GetVelocity().Size();
		FootIK(DeltaSeconds);
	}

}
void UKnightEnemyAnimInstance::PlayDrawSwordMontage()
{
	check(!IsDead);
	IsWeapon = true;
	Montage_Play(EnemyDrawSwordMontage, 1.0f);
}
void UKnightEnemyAnimInstance::PlayAttackMontage()
{
	check(!IsDead);
	Montage_Play(EnemyAttackMontage, 1.2f);
}
void UKnightEnemyAnimInstance::PlaySwordBlockMontage(bool isPaly)
{
	check(!IsDead);
	if (isPaly) { Montage_Play(EnemySwordBlockMontage, 0.5f); }
	else { Montage_Stop(0.1f, EnemySwordBlockMontage); }
}
void UKnightEnemyAnimInstance::PlayPainMontage()
{
	check(!IsDead);
	Montage_Play(EnemyPainMontage, 1.f);
}
void UKnightEnemyAnimInstance::PlayNuckMontage()
{
	check(!IsDead);
	Montage_Play(EnemyNuckMontage, 1.f);
}
void UKnightEnemyAnimInstance::PlayAssassinationAnim()
{
	check(!IsDead);
	Montage_Play(AssassinationAnim, 1.f);
}
void UKnightEnemyAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	check(!IsDead);
	check(Montage_IsPlaying(EnemyAttackMontage));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), EnemyAttackMontage);
}
void UKnightEnemyAnimInstance::JumpToSwordMontageSection(int32 NewSection)
{
	check(Montage_IsPlaying(EnemyDrawSwordMontage));
	IsWeapon = false;
	Montage_JumpToSection(FName(*FString::Printf(TEXT("%d"), NewSection)), EnemyDrawSwordMontage);
}



void UKnightEnemyAnimInstance::FootIK(float DeltaTime)
{
	if (Character && !Character->GetCharacterMovement()->IsFalling()) // No Falling
	{
		IgnoreActors.Emplace(Character);

		TTuple<bool, float> Foot_R = CapsuleDistance("ik_foot_r", Character);
		TTuple<bool, float> Foot_L = CapsuleDistance("ik_foot_l", Character);

		if (Foot_L.Get<0>() || Foot_R.Get<0>())
		{
			const float Selectfloat = UKismetMathLibrary::SelectFloat(Foot_L.Get<1>(), Foot_R.Get<1>(), Foot_L.Get<1>() >= Foot_R.Get<1>());
			Displacement = FMath::FInterpTo(Displacement, (Selectfloat - 98.f) * -1.f, DeltaTime, IKInterpSpeed);

			TTuple<bool, float, FVector> FootTrace_R = FootLineTrace("ik_foot_r", Character);
			TTuple<bool, float, FVector> FootTrace_L = FootLineTrace("ik_foot_l", Character);

			const float Distance_R = FootTrace_R.Get<1>();
			const FVector FootRVector(FootTrace_R.Get<2>());
			const FRotator MakeRRot(UKismetMathLibrary::DegAtan2(FootRVector.X, FootRVector.Z) * -1.f, 0.f, UKismetMathLibrary::DegAtan2(FootRVector.Y, FootRVector.Z));

			RRot = FMath::RInterpTo(RRot, MakeRRot, DeltaTime, IKInterpSpeed);
			RIK = FMath::FInterpTo(RIK, (Distance_R - 110.f) / -45.f, DeltaTime, IKInterpSpeed);

			const float Distance_L = FootTrace_L.Get<1>();
			const FVector FootLVector(FootTrace_L.Get<2>());
			const FRotator MakeLRot(UKismetMathLibrary::DegAtan2(FootLVector.X, FootLVector.Z) * -1.f, 0.f, UKismetMathLibrary::DegAtan2(FootLVector.Y, FootLVector.Z));

			LRot = FMath::RInterpTo(LRot, MakeLRot, DeltaTime, IKInterpSpeed);
			LIK = FMath::FInterpTo(LIK, (Distance_L - 110.f) / -45.f, DeltaTime, IKInterpSpeed);
		}
	}
	else
	{
		LRot = FMath::RInterpTo(LRot, FRotator::ZeroRotator, DeltaTime, IKInterpSpeed);
		LIK = FMath::FInterpTo(LIK, 0.f, DeltaTime, IKInterpSpeed);

		RRot = FMath::RInterpTo(RRot, FRotator::ZeroRotator, DeltaTime, IKInterpSpeed);
		RIK = FMath::FInterpTo(RIK, 0.f, DeltaTime, IKInterpSpeed);
	}
}

TTuple<bool, float> UKnightEnemyAnimInstance::CapsuleDistance(FName SocketName, ACharacter* Char)
{
	const FVector WorldLocation{ Char->GetMesh()->GetComponentLocation() };
	const FVector BreakVector{ WorldLocation + FVector(0.f,0.f,98.f) };

	const FVector SocketLocation{ Char->GetMesh()->GetSocketLocation(SocketName) };

	const FVector Start{ SocketLocation.X,SocketLocation.Y,BreakVector.Z };
	const FVector End{ Start - FVector(0.f,0.f,151.f) };

	FHitResult HitResult;

	UKismetSystemLibrary::LineTraceSingle(
		this,
		Start,
		End,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		true,
		IgnoreActors,
		EDrawDebugTrace::None,
		HitResult,
		false);

	const bool Result(HitResult.bBlockingHit);

	return MakeTuple(Result, HitResult.Distance);
}

TTuple<bool, float, FVector> UKnightEnemyAnimInstance::FootLineTrace(FName SocketName, ACharacter* Char)
{
	const FVector SocketLocation{ Char->GetMesh()->GetSocketLocation(SocketName) };
	const FVector RootLocation(Char->GetMesh()->GetSocketLocation("root"));

	const FVector Start{ SocketLocation.X,SocketLocation.Y,RootLocation.Z };

	FHitResult HitResult;

	UKismetSystemLibrary::LineTraceSingle(
		this,
		Start + FVector(0.f, 0.f, 105.f),
		Start + FVector(0.f, 0.f, -105.f),
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		true,
		IgnoreActors,
		EDrawDebugTrace::None,
		HitResult,
		false);

	const bool Result(HitResult.bBlockingHit);

	if (HitResult.bBlockingHit)
	{
		return MakeTuple(Result, HitResult.Distance, HitResult.Normal);
	}
	else
	{
		return MakeTuple(Result, 999.f, FVector::ZeroVector);
	}
}

void UKnightEnemyAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UKnightEnemyAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

void UKnightEnemyAnimInstance::AnimNotify_EQUIP()
{
	OnEQUIP.Broadcast();
}

void UKnightEnemyAnimInstance::AnimNotify_sheath()
{
	Onsheath.Broadcast();
}

void UKnightEnemyAnimInstance::AnimNotify_RunAI()
{
	OnRunAI.Broadcast();
}

void UKnightEnemyAnimInstance::AnimNotify_StopAI()
{
	OnStopAI.Broadcast();
}

FName UKnightEnemyAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	check(FMath::IsWithinInclusive<int32>(Section, 1, 3));
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}
