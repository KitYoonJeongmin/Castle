// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbingComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/EngineTypes.h"
#include "DrawDebugHelpers.h"
#include "MainAnimInstance.h"


// Sets default values for this component's properties
UClimbingComponent::UClimbingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//Climbing
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Climbing_MONTAGE(TEXT("AnimMontage'/Game/AMyDirectory/Animations/ClimbingAnimations/Mon_MainHang_Idle.Mon_MainHang_Idle'"));
	if (Climbing_MONTAGE.Succeeded())
	{
		ClimbIdleMon = Climbing_MONTAGE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Climb_MONTAGE(TEXT("AnimMontage'/Game/AMyDirectory/Animations/ClimbingAnimations/MainClimb_Montage.MainClimb_Montage'"));
	if (Climb_MONTAGE.Succeeded())
	{
		ClimbMon = Climb_MONTAGE.Object;
	}
	
	LeftClimbArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("LeftClimbArrow"));
	LeftClimbArrow->SetRelativeLocation(FVector(60.0f, -70.0f, 40.0f));

	RightClimbArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("RightClimbArrow"));
	RightClimbArrow->SetRelativeLocation(FVector(60.0f, 70.0f, 40.0f));

	LeftLedgeArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("LeftLedgeArrow"));
	LeftLedgeArrow->SetRelativeLocation(FVector(60.0f, -150.0f, 40.0f));

	RightLedgeArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("RightLedgeArrow"));
	RightLedgeArrow->SetRelativeLocation(FVector(60.0f, 150.0f, 40.0f));

	UpArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("UpArrow"));
	UpArrow->SetRelativeLocation(FVector(65.0f, 0.0f, 150.0f));

	LeftHandArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("LeftHandArrow"));
	LeftHandArrow->SetRelativeRotation(FRotator(0.f, 0.0f, 90.0f));
	LeftHandArrow->SetRelativeLocation(FVector(0.0f, -5.0f, 70.0f));
	
	RightHandArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("RightHandArrow"));
	RightHandArrow->SetRelativeRotation(FRotator(0.f, 0.0f, 90.0f));
	RightHandArrow->SetRelativeLocation(FVector(0.0f, 5.0f, 70.0f));
}


void UClimbingComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	
}

// Called when the game starts
void UClimbingComponent::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<ACharacter>(GetOwner());
	if (Character != nullptr)
	{
		Character->GetCharacterMovement()->BrakingDecelerationFlying = 0.f;
		Cast<UMainAnimInstance>(Character->GetMesh()->GetAnimInstance())->OnClimbEnd.AddLambda([this]()->void {
			FLatentActionInfo Info;
			Info.CallbackTarget = this;
			UKismetSystemLibrary::MoveComponentTo(Character->GetCapsuleComponent(),
				Character->GetCapsuleComponent()->GetComponentLocation() + Character->GetActorForwardVector() * 50.f,
				UKismetMathLibrary::MakeRotFromX(Character->GetActorForwardVector()), false, false, 0.1f, false, EMoveComponentAction::Type::Move, Info);
			isClimbing = false;
			isFinish = false;
			Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			});

		LeftClimbArrow->AttachTo(Character->GetCapsuleComponent());
		RightClimbArrow->AttachTo(Character->GetCapsuleComponent());
		LeftLedgeArrow->AttachTo(Character->GetCapsuleComponent());
		RightLedgeArrow->AttachTo(Character->GetCapsuleComponent());
		UpArrow->AttachTo(Character->GetCapsuleComponent());
		LeftHandArrow->AttachTo(Character->GetCapsuleComponent());
		RightHandArrow->AttachTo(Character->GetCapsuleComponent());
	}
}


// Called every frame
void UClimbingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (Character->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking)
	{
		IsFalling = false;
	}
	CharLoc = Character->GetActorLocation();
	CharFor = Character->GetActorForwardVector();
	ForwardTrace();
	HeightTrace();
	IKHandTrace();
	if (isClimbing)
	{
		LeftTrace();
		RightTrace();
		MoveInLedge();
		JumpUpTrace();
		
		if (CanMoveLeft)
		{
			CanJumpLeft = false;
			CanTurnLeft = false;
		}
		else
		{
			JumpLeftTrace();
			//코너
			if (!CanJumpLeft) { TurnLeftTrace(); }
			else { CanTurnLeft = false; }
		}
		if (CanMoveRight)
		{
			CanJumpRight = false;
			CanTurnRight = false;
		}
		else
		{
			JumpRightTrace();
			//코너
			if (!CanJumpRight) { TurnRightTrace(); }
			else { CanTurnRight = false; }
		}
	}

	
}

void UClimbingComponent::ForwardTrace()
{
	if (Character == nullptr) return;
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel6));
	float HightSize = 120.f;
	if (isClimbing)
	{
		isInForward = UKismetSystemLibrary::LineTraceSingleForObjects(
			GetWorld(),
			CharLoc + FVector(0.f, 0.f, 70.f),
			(CharFor * 100.f) + CharLoc + FVector(0.f, 0.f, 70.f),
			ObjectTypes,
			false,
			ActorsToIgnore, // 무시할 것이 없다고해도 null을 넣을 수 없다.
			EDrawDebugTrace::ForOneFrame,
			HitResult,
			true
			// 여기 밑에 3개는 기본 값으로 제공됨. 바꾸려면 적으면 됨.
			, FLinearColor::Red
			, FLinearColor::Green
			, 5.0f
		);
		if (isInForward)
		{
			WallNormal = HitResult.Normal;
			WallLoc = HitResult.Location;
		}
	}
	else if (IsFalling)
	{
		isInForward = UKismetSystemLibrary::LineTraceSingleForObjects(
			GetWorld(),
			CharLoc + FVector(0.f, 0.f, 50.f),
			(CharFor * 100.f) + CharLoc + FVector(0.f, 0.f, 50.f),
			ObjectTypes,
			false,
			ActorsToIgnore, // 무시할 것이 없다고해도 null을 넣을 수 없다.
			EDrawDebugTrace::ForOneFrame,
			HitResult,
			true
			// 여기 밑에 3개는 기본 값으로 제공됨. 바꾸려면 적으면 됨.
			, FLinearColor::Red
			, FLinearColor::Green
			, 5.0f
		);
		if (isInForward)
		{
			WallNormal = HitResult.Normal;
			WallLoc = HitResult.Location;
		}
	}
	else
	{
		for (float hight = 0; hight <= HightSize * 2 && !IsFalling; hight += HightSize)
		{

			isInForward = UKismetSystemLibrary::LineTraceSingleForObjects(
				GetWorld(),
				CharLoc + FVector(0.f, 0.f, hight),
				(CharFor * 130.f) + CharLoc + FVector(0.f, 0.f, hight),
				ObjectTypes,
				false,
				ActorsToIgnore, // 무시할 것이 없다고해도 null을 넣을 수 없다.
				EDrawDebugTrace::None,
				HitResult,
				true
				// 여기 밑에 3개는 기본 값으로 제공됨. 바꾸려면 적으면 됨.
				, FLinearColor::Red
				, FLinearColor::Green
				, 5.0f
			);
			if (isInForward)
			{
				WallNormal = HitResult.Normal;
				WallLoc = HitResult.Location;
				break;

			}
		}
	}
}

void UClimbingComponent::HeightTrace()
{
	if (Character == nullptr) return;
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character); 
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel6));
	isInHeight = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		CharLoc + CharFor * 75.f,
		CharLoc + CharFor * 75.f +FVector(0.f, 0.f, 100.f),
		ObjectTypes,
		false,
		ActorsToIgnore, // 무시할 것이 없다고해도 null을 넣을 수 없다.
		EDrawDebugTrace::None,
		HitResult,
		true
		, FLinearColor::Red
		, FLinearColor::Green
		, 5.0f
	);
	if (isInHeight)
	{
		HeightLoc = HitResult.ImpactPoint;
		float height = Character->GetMesh()->GetSocketLocation(FName("PelvisSocket")).Z - HeightLoc.Z;

		if (!isClimbing && !IsTurning && isInForward)
		{
			Hang();
		}
	}
}

void UClimbingComponent::Hang()
{
	isClimbing = true;
	//UE_LOG(LogTemp, Warning, TEXT("Pelvis: %f Location : %f"), Character->GetMesh()->GetSocketLocation(FName("PelvisSocket")).Z, HeightLoc.Z);
	FVector TargetLoc((WallNormal * 50.f + WallLoc).X, (WallNormal * 50.f + WallLoc).Y, HeightLoc.Z - Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 40.f);
	FRotator TargetRot = UKismetMathLibrary::MakeRotFromX(WallNormal * -1);
	FLatentActionInfo Info;
	Info.CallbackTarget = this;

	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

	UKismetSystemLibrary::MoveComponentTo(Character->GetCapsuleComponent(), TargetLoc, TargetRot, true, true, 0.1f, false, EMoveComponentAction::Type::Move, Info);
	
	Character->GetCharacterMovement()->StopMovementImmediately();
	
	
	
	
}
void UClimbingComponent::DropDown()
{
	if (!isClimbing) return;
	isClimbing = false;
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	Character->GetMesh()->GetAnimInstance()->StopAllMontages(0.2f);
	IsFalling = true;

}

void UClimbingComponent::ClimbUp()
{

	if (!isClimbing || isFinish) return;
	if (CanJumpUp) return;
	Character->GetMesh()->GetAnimInstance()->Montage_Play(ClimbMon,1.f);
	isFinish = true;

	
}

void UClimbingComponent::LeftTrace()
{
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel6));
	ActorsToIgnore.Add(Character);
	CanMoveLeft = UKismetSystemLibrary::CapsuleTraceSingleForObjects(GetWorld(), LeftClimbArrow->GetComponentLocation(), LeftClimbArrow->GetComponentLocation(), 30.f, 60.f, ObjectTypes,
		false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

	
}

void UClimbingComponent::RightTrace()
{
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel6));
	CanMoveRight = UKismetSystemLibrary::CapsuleTraceSingleForObjects(GetWorld(), RightClimbArrow->GetComponentLocation(), RightClimbArrow->GetComponentLocation(), 30.f, 60.f, ObjectTypes,
		false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);
}

void UClimbingComponent::MoveInLedge()
{
	if (CanMoveLeft)
	{
		if (Character->InputComponent->GetAxisValue((FName("MoveRight"))) < 0.f)
		{
			FVector LeftMoveVec = UKismetMathLibrary::VInterpTo(CharLoc, CharLoc + Character->GetActorRightVector() * -20, GetWorld()->DeltaTimeSeconds, 5);
			Character->SetActorLocation(LeftMoveVec,false);
			IsMovingLeft = true;
			IsMovingRight = false;
		}
		else
		{
			IsMovingLeft = false;
		}
	}
	if (CanMoveRight)
	{
		if (Character->InputComponent->GetAxisValue((FName("MoveRight"))) > 0.f)
		{
			FVector RightMoveVec = UKismetMathLibrary::VInterpTo(CharLoc, CharLoc + Character->GetActorRightVector() * 20, GetWorld()->DeltaTimeSeconds, 5);
			Character->SetActorLocation(RightMoveVec, false);
			IsMovingLeft = false;
			IsMovingRight = true;
		}
		else
		{
			IsMovingRight = false;
		}
	}

}

void UClimbingComponent::JumpLeftTrace()
{
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel6));

	FVector StartLoc = LeftLedgeArrow->GetComponentLocation();
	FVector EndLoc = StartLoc + Character->GetActorRightVector() * -200.f;
	if (UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), StartLoc, EndLoc, 30.f, ObjectTypes,
		false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitResult, true))
	{
		SideJumpLoc = HitResult.ImpactPoint;
		if (UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), SideJumpLoc - CharFor * 100.f, SideJumpLoc, 30.f, ObjectTypes,
			false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitResult, true))
		{
			SideJumpWallLoc = HitResult.ImpactPoint;
			CanJumpLeft = !CanMoveLeft;
		}
		else
		{
			CanJumpLeft = false;
		}
		CanJumpLeft = !CanMoveLeft;
	}
	else
	{
		CanJumpLeft = false;
	}
}

void UClimbingComponent::JumpRightTrace()
{
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel6));

	FVector StartLoc = RightLedgeArrow->GetComponentLocation();
	FVector EndLoc = StartLoc + Character->GetActorRightVector()*200.f;
	if (UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), StartLoc, EndLoc, 30.f, ObjectTypes,
	   false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitResult, true))
	{
		SideJumpLoc = HitResult.ImpactPoint;
		if (UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), SideJumpLoc -CharFor*100.f, SideJumpLoc, 30.f, ObjectTypes,
			false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitResult, true))
		{
			SideJumpWallLoc = HitResult.ImpactPoint;
			CanJumpRight = !CanMoveRight;
		}
		else
		{
			CanJumpRight = false;
		}
	}
	else
	{
		CanJumpRight = false;
	}
}

void UClimbingComponent::JumpLeftLedge()
{
	if (Character->InputComponent->GetAxisValue((FName("MoveRight"))) < 0.f)
	{
		if (!IsJumping)
		{
			Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
			Cast<UMainAnimInstance>(Character->GetMesh()->GetAnimInstance())->JumpLeft(IsJumping);


			IsJumping = true;
			isClimbing = true;

			FLatentActionInfo Info;
			Info.CallbackTarget = this;
			SideJumpWallLoc.Z -= 50.f;
			UKismetSystemLibrary::MoveComponentTo(Character->GetCapsuleComponent(), SideJumpWallLoc + CharFor * -40.f, Character->GetActorRotation(), true, true, 0.4f, false, EMoveComponentAction::Type::Move, Info);
		}
	}

}

void UClimbingComponent::JumpRightLedge()
{
	if (Character->InputComponent->GetAxisValue((FName("MoveRight"))) > 0.f)
	{
		if (!IsJumping)
		{
			Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
			Cast<UMainAnimInstance>(Character->GetMesh()->GetAnimInstance())->JumpRight(IsJumping);

			IsJumping = true;
			isClimbing = true;

			FLatentActionInfo Info;
			Info.CallbackTarget = this;
			SideJumpWallLoc.Z -= 50.f;
			UKismetSystemLibrary::MoveComponentTo(Character->GetCapsuleComponent(), SideJumpWallLoc + CharFor*-40.f, Character->GetActorRotation(), true, true, 0.4f, false, EMoveComponentAction::Type::Move, Info);
			
		}
	}
}

void UClimbingComponent::JumpLeft(bool isJumpLeft)
{
	IsJumpLeft = isJumpLeft;
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	Character->GetCharacterMovement()->StopMovementImmediately();
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	IsJumping = false;
}

void UClimbingComponent::JumpRight(bool isJumpRight)
{
	IsJumpRight = isJumpRight;
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	Character->GetCharacterMovement()->StopMovementImmediately();
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	IsJumping = false;

}

void UClimbingComponent::TurnLeftTrace()
{
	Character->GetCharacterMovement()->StopMovementImmediately();

	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);

	FVector StartVec(LeftClimbArrow->GetComponentLocation());
	StartVec.Z += 60.f;
	FVector EndVec = StartVec + LeftClimbArrow->GetForwardVector() * 70.f;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel6));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	if (UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), StartVec, EndVec, 25.f, ObjectTypes,
		false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true))
	{
		CanTurnLeft = false;
	}
	else
	{
		CanTurnLeft = true;
	}
}

void UClimbingComponent::TurnRightTrace()
{
	Character->GetCharacterMovement()->StopMovementImmediately();

	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);

	FVector StartVec(RightClimbArrow->GetComponentLocation());
	StartVec.Z += 60.f;
	FVector EndVec = StartVec + RightClimbArrow->GetForwardVector() * 70.f;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel6));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	if (UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), StartVec, EndVec, 25.f, ObjectTypes,
		false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true))
	{

		CanTurnRight = false;
	}
	else
	{
		CanTurnRight = true;
	}
}

void UClimbingComponent::JumpUpTrace()
{
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel6));
	if(UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),UpArrow->GetComponentLocation(),UpArrow->GetComponentLocation() + FVector(0.f,0.f,10000.f),
		ObjectTypes,false,ActorsToIgnore,EDrawDebugTrace::None,HitResult,true
		, FLinearColor::Blue
		, FLinearColor::Green
		, 5.0f
	))
	{
		JumpHightLoc = HitResult.ImpactPoint;
		if (UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),  FVector(CharLoc.X, CharLoc.Y, JumpHightLoc.Z), FVector(CharLoc.X, CharLoc.Y, JumpHightLoc.Z) + CharFor * 100.f,
			ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true
			, FLinearColor::Blue
			, FLinearColor::Green
			, 5.0f))
		{
			JumpWallLoc = HitResult.ImpactPoint;
			JumpWallNormal = HitResult.Normal;
			CanJumpUp = true;
		}
		else
		{
			CanJumpUp = false;
		}
		

	}
	else
	{
		CanJumpUp = false;
	}
}

void UClimbingComponent::JumpUpLedge()
{
	if (Character->InputComponent->GetAxisValue((FName("MoveRight"))) == 0.f &&
		CanJumpUp && !IsJumping)
	{

		Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		FTimerHandle WaitHandle;
		float WaitTime = 0.1f; 
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
			{
				FVector TargetLoc((JumpWallNormal * 50.f + JumpWallLoc).X, (JumpWallNormal * 50.f + JumpWallLoc).Y, JumpHightLoc.Z - Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 40.f);
				FRotator TargetRot = UKismetMathLibrary::MakeRotFromX(JumpWallNormal * -1);
				FLatentActionInfo Info;
				Info.CallbackTarget = this;
				UKismetSystemLibrary::MoveComponentTo(Character->GetCapsuleComponent(), TargetLoc, TargetRot, true, true, 0.4f, false, EMoveComponentAction::Type::Move, Info);

			}), WaitTime, false); //반복도 여기서 추가 변수를 선언해 설정가능
		
		Cast<UMainAnimInstance>(Character->GetMesh()->GetAnimInstance())->JumpUp(true);
		IsJumping = true;
		Character->DisableInput(Cast<APlayerController>(Character->GetController()));
	}
}

void UClimbingComponent::JumpUp(bool jumpUp)
{
	IsJumping = jumpUp;
	Character->GetCharacterMovement()->StopMovementImmediately();
	IsJumping = false;
	
	Character->EnableInput(Cast<APlayerController>(Character->GetController()));
}

void UClimbingComponent::IKHandTrace()
{
	FHitResult HitResult;
	FHitResult HitResult2;
	TArray<AActor*> ActorsToIgnore;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel6));

	FVector LeftStartVec(LeftHandArrow->GetComponentLocation());
	FVector LeftEndVec = LeftStartVec + LeftHandArrow->GetForwardVector() * 90.f;
	if (UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), LeftStartVec, LeftEndVec, 5.f, ObjectTypes,
		false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true))
	{
		FVector HitPoint(HitResult.ImpactPoint);
		FHitResult HitResultLeft;
		if (UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), HitPoint+FVector(0.f,0.f,20.f), HitPoint, 5.f, ObjectTypes,
			false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitResultLeft, true))
		{
			IKLeftHand = HitPoint;
			IKLeftHand.Z = HitResultLeft.ImpactPoint.Z;
		}
	}

	FVector RightStartVec(RightHandArrow->GetComponentLocation());
	FVector RightEndVec = RightStartVec + RightHandArrow->GetForwardVector() * 90.f;
	if (UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), RightStartVec, RightEndVec, 5.f, ObjectTypes,
		false, ActorsToIgnore, EDrawDebugTrace::None, HitResult2, true))
	{
		FVector HitPoint(HitResult2.ImpactPoint);
		FHitResult HitResultRight;
		if (UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), HitPoint + FVector(0.f, 0.f, 20.f), HitPoint, 5.f, ObjectTypes,
			false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitResultRight, true))
		{
			IKRightHand = HitPoint;

			IKRightHand.Z = HitResultRight.ImpactPoint.Z;
		}
	}
}


