// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "MainCharacter.h"

UMainAnimInstance::UMainAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IKInterpSpeed = 13.0;
	IsDead = false;
	isZooming = false;
	isAimming = false;
	CurrentWeapon = EWeapon::Hand;



	//Main Character
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("AnimMontage'/Game/AMyDirectory/Animations/Mon_Attack.Mon_Attack'"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DrawSword_MONTAGE(TEXT("AnimMontage'/Game/AMyDirectory/Animations/DrawSwordMontage.DrawSwordMontage'"));
	if (DrawSword_MONTAGE.Succeeded())
	{
		DrawSwordMontage = DrawSword_MONTAGE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SWORD_BLOCK_MONTAGE(TEXT("AnimMontage'/Game/AMyDirectory/Animations/Mon_SwordBlock.Mon_SwordBlock'"));
	if (SWORD_BLOCK_MONTAGE.Succeeded())
	{
		SwordBlockMontage = SWORD_BLOCK_MONTAGE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ROLL_MONTAGE(TEXT("AnimMontage'/Game/AMyDirectory/Animations/Mon_Roll.Mon_Roll'"));
	if (ROLL_MONTAGE.Succeeded())
	{
		RollMontage = ROLL_MONTAGE.Object;
	}
	static ConstructorHelpers::FClassFinder<UCameraShakeBase> CAMERA_SHAKE(TEXT("Blueprint'/Game/AMyDirectory/Blueprints/BP_CameraShake.BP_CameraShake_C'"));
	if (CAMERA_SHAKE.Succeeded())
	{
		CameraShacke = CAMERA_SHAKE.Class;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SwordSkill_1(TEXT("AnimMontage'/Game/AMyDirectory/Animations/Mon_Sword_Skill1.Mon_Sword_Skill1'"));
	if (SwordSkill_1.Succeeded())
	{
		SwordSkill_1Montage = SwordSkill_1.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SwordSkill_2(TEXT("AnimMontage'/Game/AMyDirectory/Animations/Mon_SwordSkill2.Mon_SwordSkill2'"));
	if (SwordSkill_2.Succeeded())
	{
		SwordSkill_2Montage = SwordSkill_2.Object;
	}
	
	//Bow
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DrawBow_MONTAGE(TEXT("AnimMontage'/Game/AMyDirectory/Animations/Mon_DrawBow.Mon_DrawBow'"));
	if (DrawBow_MONTAGE.Succeeded())
	{
		DrawBowMontage = DrawBow_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Bow_MONTAGE(TEXT("AnimMontage'/Game/AMyDirectory/ArrowBow/Bow/Mon_BowDraw.Mon_BowDraw'"));
	if (Bow_MONTAGE.Succeeded())
	{
		BowMontage = Bow_MONTAGE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Turn90Montage_MONTAGE(TEXT("AnimMontage'/Game/AMyDirectory/Animations/AimTurn.AimTurn'"));
	if (Turn90Montage_MONTAGE.Succeeded())
	{
		Turn90Montage = Turn90Montage_MONTAGE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ShotBow_MONTAGE(TEXT("AnimMontage'/Game/AMyDirectory/Animations/Mon_BowShot.Mon_BowShot'"));
	if (ShotBow_MONTAGE.Succeeded())
	{
		ShotBowMontage = ShotBow_MONTAGE.Object;
	}
	
}


void UMainAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	//Character의 정보를 얻어옴
	Super::NativeUpdateAnimation(DeltaSeconds);
	Character = Cast<AMainCharacter>(TryGetPawnOwner());

	if (!::IsValid(Character)) { return; }

	if(!IsDead)
	{
		CurrentPawnForwardSpeed = Character->GetVelocity().X;
		CurrentPawnSpeed = Character->GetVelocity().Size();
		CurrentPawnRotation = CalculateDirection(Character->GetVelocity(), Character->GetActorRotation());
		IsInAir = Character->GetMovementComponent()->IsFalling();
		FootIK(DeltaSeconds);

		FRotator CurrentRot = UKismetMathLibrary::MakeRotator(0.f, AimPitch, AimYaw);
		FRotator ContRot = Character->GetControlRotation();
		FRotator CharRot = Character->GetActorRotation();
		FRotator TargetRot = UKismetMathLibrary::NormalizedDeltaRotator(ContRot, CharRot);
		FRotator Degree = FMath::RInterpTo(CurrentRot, TargetRot, DeltaSeconds, 15.f);
		AimPitch = FMath::Clamp(Degree.Pitch, -60.f, 60.f);
		AimYaw = FMath::Clamp(Degree.Yaw, -60.f, 60.f);
	}

}

void UMainAnimInstance::PlayDrawSwordMontage()
{
	check(!IsDead);
	CurrentWeapon = EWeapon::Sword;
	Montage_Play(DrawSwordMontage, 1.0f);
	
}

void UMainAnimInstance::PlayDrawBowMontage()
{
	check(!IsDead);
	CurrentWeapon = EWeapon::Bow;
	Montage_Play(DrawBowMontage, 1.0f);
}

void UMainAnimInstance::PlayBowShotMontage()
{
	check(!IsDead);
	Montage_Play(ShotBowMontage, 1.f);
}

void UMainAnimInstance::PlayAttackMontage()
{
	check(!IsDead);
	Montage_Play(AttackMontage, 1.2f);

}



void UMainAnimInstance::PlayRollMontage()
{
	check(!IsDead);
	Montage_Play(RollMontage,1.3f);
	FLatentActionInfo Info;
	Info.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(Character->GetCapsuleComponent(), Character->GetActorLocation() + Character->GetActorForwardVector() * 400.f, Character->GetActorRotation(), false, false, 1.2f, false, EMoveComponentAction::Type::Move, Info);
	
}

void UMainAnimInstance::PlaySwordBlockMontage(bool isPlay)
{
	check(!IsDead);
	if (isPlay) { Montage_Play(SwordBlockMontage, 0.5f); }
	else { Montage_Stop(0.1f, SwordBlockMontage); }
	
}

void UMainAnimInstance::PlaySwordSkill_1Montage()
{
	check(!IsDead);
	Montage_Play(SwordSkill_1Montage, 1.f);

	FTimerHandle WaitHandle;
	float WaitTime = 0.8f; //시간을 설정하고
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(CameraShacke);
		}), WaitTime, false);
	
}

void UMainAnimInstance::PlaySwordSkill_2Montage()
{
	check(!IsDead);
	Montage_Play(SwordSkill_2Montage, 1.f);
	
}

void UMainAnimInstance::PlayTurn90Montage(bool isRight)
{
	check(!IsDead);

	Montage_Play(Turn90Montage, 1.f);
	if(isRight)
		Montage_JumpToSection(FName("2"), Turn90Montage);
}

void UMainAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	check(!IsDead);
	check(Montage_IsPlaying(AttackMontage));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
	
}

void UMainAnimInstance::JumpToSwordMontageSection(int32 NewSection)
{
	switch (CurrentWeapon)
	{
	case EWeapon::Sword:	
		check(Montage_IsPlaying(DrawSwordMontage));
		Montage_JumpToSection(FName(*FString::Printf(TEXT("%d"), NewSection)), DrawSwordMontage);
		break;
	case EWeapon::Bow:
		check(Montage_IsPlaying(DrawBowMontage));
		Montage_JumpToSection(FName(*FString::Printf(TEXT("%d"), NewSection)), DrawBowMontage);
		break;
	}
	CurrentWeapon = EWeapon::Hand;

}

void UMainAnimInstance::SetUpperBodyRotation(float Yaw, float Pitch)
{
	UpperBodyRotationYaw = Yaw;
	UpperBodyRotationPitch = Pitch;
}




void UMainAnimInstance::FootIK(float DeltaTime)
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

TTuple<bool, float> UMainAnimInstance::CapsuleDistance(FName SocketName, ACharacter* Char)
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

TTuple<bool, float, FVector> UMainAnimInstance::FootLineTrace(FName SocketName, ACharacter* Char)
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

void UMainAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UMainAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

void UMainAnimInstance::AnimNotify_EQUIP()
{
	OnEQUIP.Broadcast();
}
void UMainAnimInstance::AnimNotify_sheath()
{
	Onsheath.Broadcast();
}

void UMainAnimInstance::AnimNotify_DisableMovement()
{
	Character->GetMovementComponent()->SetActive(false);
}

void UMainAnimInstance::AnimNotify_DisableInput()
{
	Character->DisableInput(Cast<APlayerController>(Character->GetController()));
}


void UMainAnimInstance::AnimNotify_MoveForward()
{
	//Character->GetCharacterMovement()->AddImpulse(Character->GetActorForwardVector() * 2500.f, true);
}

void UMainAnimInstance::AnimNotify_SlowMotionStart()
{
	GetWorld()->GetWorldSettings()->SetTimeDilation(0.1f);
}
void UMainAnimInstance::AnimNotify_SlowMotionEnd()
{
	GetWorld()->GetWorldSettings()->SetTimeDilation(1.0f);
}
void UMainAnimInstance::AnimNotify_StartDrawBow()
{
	Montage_Play(BowMontage, 1.0f);
}
void UMainAnimInstance::AnimNotify_BowFire()
{
	OnBowFire.Broadcast();
}
FName UMainAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	check(FMath::IsWithinInclusive<int32>(Section, 1,3));
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}
