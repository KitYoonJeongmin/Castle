// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "MainCharacter.h"
#include "ClimbingComponent.h"
#include "RidingHorse.h"

UMainAnimInstance::UMainAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IKInterpSpeed = 4.f;
	IsDead = false;
	isZooming = false;
	isAimming = false;
	CurrentWeapon = EWeapon::Hand;

	//Main Character
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("AnimMontage'/Game/AMyDirectory/Animations/Mon_Attack.Mon_Attack'"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage.Add(ATTACK_MONTAGE.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Combo2_MONTAGE(TEXT("AnimMontage'/Game/AMyDirectory/Animations/Mon_Attack2.Mon_Attack2'"));
	if (Combo2_MONTAGE.Succeeded())
	{
		AttackMontage.Add(Combo2_MONTAGE.Object);
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
	static ConstructorHelpers::FObjectFinder<UAnimMontage> BowSkill1_MONTAGE(TEXT("AnimMontage'/Game/AMyDirectory/Animations/MonBowSkill1.MonBowSkill1'"));
	if (BowSkill1_MONTAGE.Succeeded())
	{
		BowSkill1 = BowSkill1_MONTAGE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ASSASSINATION_MONTAGE(TEXT("AnimMontage'/Game/AMyDirectory/Animations/Mon_Stealth_Assassination.Mon_Stealth_Assassination'"));
	if (ASSASSINATION_MONTAGE.Succeeded())
	{
		Assassination = ASSASSINATION_MONTAGE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ClimbJumpLeft_MONTAGE(TEXT("AnimMontage'/Game/AMyDirectory/Animations/ClimbingAnimations/Mon_JumpLeftMain.Mon_JumpLeftMain'"));
	if (ClimbJumpLeft_MONTAGE.Succeeded())
	{
		ClimbJumpLeft = ClimbJumpLeft_MONTAGE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ClimbJumpRight_MONTAGE(TEXT("AnimMontage'/Game/AMyDirectory/Animations/ClimbingAnimations/Mon_JumpRightMain.Mon_JumpRightMain'"));
	if (ClimbJumpRight_MONTAGE.Succeeded())
	{
		ClimbJumpRight = ClimbJumpRight_MONTAGE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> CornerLeft_MONTAGE(TEXT("AnimMontage'/Game/AMyDirectory/Animations/ClimbingAnimations/Mon_MainCornerLEft.Mon_MainCornerLEft'"));
	if (CornerLeft_MONTAGE.Succeeded())
	{
		CornerLeft = CornerLeft_MONTAGE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> CornerRight_MONTAGE(TEXT("AnimMontage'/Game/AMyDirectory/Animations/ClimbingAnimations/Mon_MainCornerRight.Mon_MainCornerRight'"));
	if (CornerRight_MONTAGE.Succeeded())
	{
		CornerRight = CornerRight_MONTAGE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ClimbJumpUp_MONTAGE(TEXT("AnimMontage'/Game/AMyDirectory/Animations/ClimbingAnimations/Jump_Up_MontageMain.Jump_Up_MontageMain'"));
	if (ClimbJumpUp_MONTAGE.Succeeded())
	{
		ClimbJumpUp = ClimbJumpUp_MONTAGE.Object;
	}

	//Pain
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PAIN_MONTAGE(TEXT("AnimMontage'/Game/AMyDirectory/Animations/Mon_Pain.Mon_Pain'"));
	if (PAIN_MONTAGE.Succeeded())
	{
		Pain_Montage = PAIN_MONTAGE.Object;
	}
}


void UMainAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	//Character�� ������ ����
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

		//climb
		CanMoveLeft = Character->GetClimbingComponent()->CanMoveLeft;
		CanMoveRight = Character->GetClimbingComponent()->CanMoveRight;
		IsMovingLeft = Character->GetClimbingComponent()->IsMovingLeft;
		IsMovingRight = Character->GetClimbingComponent()->IsMovingRight;
		IsClimbing = Character->GetClimbingComponent()->isClimbing;

		//HandIK
		IKLeftHandLoc = Character->GetClimbingComponent()->IKLeftHand;
		IKRightHandLoc = Character->GetClimbingComponent()->IKRightHand;

		//Horse
		if (Character->Horse != nullptr)
		{
			HorseSpeed = Cast<ARidingHorse>(Character->Horse)->ForwardSpeed;
			HorseRot = Cast<ARidingHorse>(Character->Horse)->RightSpeed / 7.f;
		}
	}

}

void UMainAnimInstance::PlayDrawSwordMontage()
{
	if(IsDead) return;
	CurrentWeapon = EWeapon::Sword;
	Montage_Play(DrawSwordMontage, 1.0f);
	
}

void UMainAnimInstance::PlayDrawBowMontage()
{
	if (IsDead) return;
	CurrentWeapon = EWeapon::Bow;
	Montage_Play(DrawBowMontage, 1.0f);
}

void UMainAnimInstance::PlayBowShotMontage()
{
	if (IsDead) return;
	Montage_Play(ShotBowMontage, 1.f);
}

void UMainAnimInstance::PlayBowSkill1Montage()
{
	if (IsDead) return;
	Montage_Play(BowSkill1, 1.f);
}

void UMainAnimInstance::PlayAssassinationMontage()
{
	if (IsDead) return;
	Montage_Play(Assassination, 1.f);
}

void UMainAnimInstance::PlayLeftCornerMontage()
{
	if (IsDead) return;
	Montage_Play(CornerLeft, 1.f);
}

void UMainAnimInstance::PlayRightCornerMontage()
{
	if (IsDead) return;
	Montage_Play(CornerRight, 1.f);
}

void UMainAnimInstance::PalyPainMontage()
{
	if (IsDead) return;
	Montage_Play(Pain_Montage, 1.0f);
}

void UMainAnimInstance::PlayAttackMontage()
{
	if (IsDead) return;
	Montage_Play(AttackMontage[AttackMonIndex], 1.2f);

}



void UMainAnimInstance::PlayRollMontage()
{
	if (IsDead) return;
	Montage_Play(RollMontage,1.3f);

}

void UMainAnimInstance::PlaySwordBlockMontage(bool isPlay)
{
	if (IsDead) return;
	if (isPlay) { Montage_Play(SwordBlockMontage, 0.5f); }
	else { Montage_Stop(0.1f, SwordBlockMontage); }
	
}

void UMainAnimInstance::PlaySwordSkill_1Montage()
{
	if (IsDead) return;
	Montage_Play(SwordSkill_1Montage, 1.f);
}

void UMainAnimInstance::PlaySwordSkill_2Montage()
{
	if (IsDead) return;
	Montage_Play(SwordSkill_2Montage, 1.f);
	
}

void UMainAnimInstance::PlayTurn90Montage(bool isRight)
{
	if (IsDead) return;

	Montage_Play(Turn90Montage, 1.f);
	if(isRight)
		Montage_JumpToSection(FName("2"), Turn90Montage);
}

void UMainAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	if (IsDead) return;
	check(Montage_IsPlaying(AttackMontage[AttackMonIndex])); 
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage[AttackMonIndex]);
	
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

void UMainAnimInstance::SetRandomAttackMon()
{
	AttackMonIndex = FMath::RandRange(0, 1);
}




void UMainAnimInstance::FootIK(float DeltaTime)
{
	if (Character && !Character->GetCharacterMovement()->IsFalling()) // No Falling
	{
		IgnoreActors.Emplace(Character);

		TTuple<bool, float> Foot_R = CapsuleDistance(FName("Foot_R"), Character);
		TTuple<bool, float> Foot_L = CapsuleDistance(FName("Foot_L"), Character);

		if (Foot_L.Get<0>() || Foot_R.Get<0>())
		{
			const float Selectfloat = UKismetMathLibrary::SelectFloat(Foot_L.Get<1>(), Foot_R.Get<1>(), Foot_L.Get<1>() >= Foot_R.Get<1>());
			Displacement = FMath::FInterpTo(Displacement, (Selectfloat - 98.f) * -1.f, DeltaTime, IKInterpSpeed);

			TTuple<bool, float, FVector> FootTrace_R = FootLineTrace("Foot_R", Character);
			TTuple<bool, float, FVector> FootTrace_L = FootLineTrace("Foot_L", Character);

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

	const FVector SocketLocation{ Char->GetMesh()->GetBoneLocation(SocketName) };

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
		EDrawDebugTrace::ForOneFrame,
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
		EDrawDebugTrace::ForOneFrame,
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
void UMainAnimInstance::AnimNotify_BowSkill1()
{
	OnBowSkill1.Broadcast();
}
void UMainAnimInstance::AnimNotify_ClimbEnd()
{
	OnClimbEnd.Broadcast();
}
void UMainAnimInstance::AnimNotify_JumpLeftEnd()
{
	Montage_Stop(0.1f);
	Character->GetClimbingComponent()->JumpLeft(false);
	IsJumpLeft = false;
}
void UMainAnimInstance::AnimNotify_JumpRightEnd()
{
	Montage_Stop(0.1f);
	Character->GetClimbingComponent()->JumpRight(false);
	IsJumpRight = false;
}
void UMainAnimInstance::AnimNotify_JumpUpEnd()
{
	Montage_Stop(0.1f);
	Character->GetClimbingComponent()->JumpUp(false);
}
void UMainAnimInstance::AnimNotify_Mount()
{
	Character->GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Flying;
	Character->GetCharacterMovement()->BrakingDecelerationFlying = 1000000.f;
	Character->AttachToActor(Character->Horse, FAttachmentTransformRules::KeepWorldTransform, "SeatSocket");
	GetWorld()->GetFirstPlayerController()->Possess(Cast<APawn>(Character->Horse));
	Character->Horse->EnableInput(GetWorld()->GetFirstPlayerController());
	FLatentActionInfo Info;
	Info.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(Character->GetCapsuleComponent(), Cast<ACharacter>(Character->Horse)->GetMesh()->GetSocketLocation(FName("SeatSocket"))- Character->Horse->GetActorLocation()
		, FRotator(0.f,0.f,0.f), false, false, 0.1f, false, EMoveComponentAction::Type::Move, Info);
	Character->MainAnim->CanMount = true;
	
	IsMount = true;
}
void UMainAnimInstance::AnimNotify_Dismount()
{
	Character->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	FLatentActionInfo Info;
	Info.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(Character->GetCapsuleComponent(), Character->GetMesh()->GetSocketLocation("Root")
		, Character->GetActorRotation(), false, false, 0.1f, false, EMoveComponentAction::Type::Move, Info);
	//Character->SetActorLocation(Character->GetMesh()->GetSocketLocation("Root"));
	Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	Character->GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
	Character->EnableInput(GetWorld()->GetFirstPlayerController());
	
	CanMount = false;
}
FName UMainAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	check(FMath::IsWithinInclusive<int32>(Section, 1,3));
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}

void UMainAnimInstance::JumpLeft(bool isJumpLeft)
{
	IsJumpLeft = isJumpLeft;
	Montage_Play(ClimbJumpLeft, 1.0f);
}

void UMainAnimInstance::JumpRight(bool isJumpRight)
{
	IsJumpRight = isJumpRight;
	Montage_Play(ClimbJumpRight, 1.0f);
}

void UMainAnimInstance::JumpUp(bool jumpUp)
{
	IsJumpUp = jumpUp;
	Montage_Play(ClimbJumpUp, 1.f);

}
