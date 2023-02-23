// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "DrawDebugHelpers.h"
#include "Bow.h"
#include "Sword.h"
#include "Daggle.h"
#include "MainAnimInstance.h"
#include "MainCharacterStatComponent.h"
#include "MainGameModeBase.h"
#include "KnightEnemy.h"
#include "Arrow.h"
#include "ClimbingComponent.h"
#include "Components/SceneComponent.h"


// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	//���� tick �Լ����� ����Ǵ� ������ ������ false�� ����

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MainCharacter"));

	MoveDirection = FVector::ZeroVector;

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	CurrentVelocity = 70.f;
	TargetVelocity = 70.f;


	//ī�޶󿡸� ȸ���� ����
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//�޽� ����
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.0f), FRotator(0.f, -90.f, 0.f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHARMBASE(TEXT("SkeletalMesh'/Game/ParagonSparrow/Characters/Heroes/Sparrow/Skins/ZechinHuntress/Meshes/Sparrow_ZechinHuntress.Sparrow_ZechinHuntress'"));
	if (SK_CHARMBASE.Succeeded()) 
	{ 
		GetMesh()->SetSkeletalMesh(SK_CHARMBASE.Object); 
	}
	//�ִϸ��̼� ����
	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM(TEXT("AnimBlueprint'/Game/AMyDirectory/Animations/BP_AnimGraph.BP_AnimGraph_C'"));
	if (ANIM.Succeeded())// �ִϸ��̼� ����
	{
		GetMesh()->SetAnimInstanceClass(ANIM.Class);
	}


	//ĳ���� ������ ����
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 260.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	// Spring Arm
	ArmLengthTo = 350.f;
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 450.f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraLag = true;
	// Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	DefaultFOV = 0.f;
	CurrentFOV = 0.f;
	ZoomFOV = 50.f;
	ZoomSpeed = 10.f;

	DefaultCL = 350.f;
	ZoomCL = 130.f;


	//����
	IsAttacking = false;
	IsBlock = false;
	AttackEndComboState();

	//����
	Characterstat = CreateDefaultSubobject<UMainCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	HealthPoint = 100.f;
	ManaPoint = 0.f;
	//block sound
	static ConstructorHelpers::FObjectFinder<USoundBase> BLOCKSOUND(TEXT("SoundWave'/Game/AMyDirectory/Sounds/sword_block.sword_block'"));
	if (BLOCKSOUND.Succeeded())
	{
		BlockSound = BLOCKSOUND.Object;
	}
	
	//���� ������
	ClimbingComponent = CreateDefaultSubobject<UClimbingComponent>(TEXT("Climbing"));
	ClimbingComponent->SetupAttachment(RootComponent);
}

void AMainCharacter::Shot()
{

	//LineTrace
	FHitResult HitResult = LineTrace();
	if (HitResult.Actor.IsValid())
	{
		FDamageEvent DamageEvent;
		HitResult.Actor->TakeDamage(50.0f, DamageEvent, GetController(), this);
	}
	
}

FHitResult AMainCharacter::LineTrace()
{
	FVector CameraLoc = FollowCamera->GetComponentLocation();
	FVector CameraForward = FollowCamera->GetForwardVector();
	FVector CameraUpVector = FollowCamera->GetUpVector();
	FVector StartLoc = CameraLoc + (CameraForward * 400.0f); // ������ ���� ����.
	FVector EndLoc = StartLoc + (CameraForward * 5000.0f);// ������ ������ ����.


	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // ��Ʈ ������ ������Ʈ ������.
	TEnumAsByte<EObjectTypeQuery> Enemy = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody);
	ObjectTypes.Add(Enemy);

	TArray<AActor*> IgnoreActors; // ������ ���͵�.

	FHitResult HitResult; // ��Ʈ ��� �� ���� ����.

	bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		StartLoc,
		EndLoc,
		ObjectTypes,
		false,
		IgnoreActors, // ������ ���� ���ٰ��ص� null�� ���� �� ����.
		EDrawDebugTrace::None,
		HitResult,
		true
		// ���� �ؿ� 3���� �⺻ ������ ������. �ٲٷ��� ������ ��.
		//, FLinearColor::Red
		//, FLinearColor::Green
		//, 5.0f
	);

	if (HitResult.Actor.IsValid())
	{
		if (MainGameMode)
			MainGameMode->SetAimColor(true);
	}
	else
	{
		if (MainGameMode)
			MainGameMode->SetAimColor(false);
	}

	return HitResult;
}

void AMainCharacter::Skill1()
{
	if (ManaPoint < 20.f) return;
	FTimerHandle WaitHandle;
	float WaitTime = 0.8f; //�ð��� �����ϰ�
	switch (CurrentWeapon)
	{
	case EWeapon::Sword:
		MainAnim->PlaySwordSkill_1Montage();
		ManaPoint -= 20;
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
			{
				TArray<FHitResult> hits;

				TArray<TEnumAsByte<EObjectTypeQuery>> objectType;
				objectType.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
				if (UKismetSystemLibrary::SphereTraceMultiForObjects(this, GetActorLocation(), GetActorLocation(), 700.f, objectType,
					false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, hits, true))
				{
					for (int i = 0; i < hits.Num(); i++)
					{
						auto enemy = Cast<AKnightEnemy>(hits[i].GetActor());
						if (enemy == nullptr) continue;
						FDamageEvent DamageEvent;
						enemy->TakeDamage(12.0f, DamageEvent, GetController(), this);
						
					}
				}
			}), WaitTime, false);
		break;
	case EWeapon::Bow :
		if (isZooming) break;
		
		//�ִϸ��̼�
		MainAnim->PlayBowSkill1Montage();
		Bow->PlayDrawBowMon(true,1);
		ManaPoint -= 20;

		break;
	}

}

void AMainCharacter::Skill2()
{
	if (ManaPoint < 20.f) return;
	switch (CurrentWeapon)
	{
	case EWeapon::Sword:
		MainAnim->PlaySwordSkill_2Montage();
		ManaPoint -= 20;

		ArmLengthTo = 700.f;
		FTimerHandle WaitHandle;
		float WaitTime = 0.6f; //�ð��� ����
		
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
			{
				float MoveForwardSize = 800.f;
				FVector BeforePos = GetActorLocation();
				FRotator ForwardRot = FollowCamera->GetComponentRotation();
				ForwardRot.Pitch = 0.f;
				FVector ForwardPos = BeforePos + ForwardRot.Vector() * MoveForwardSize;
	
				FLatentActionInfo Info;
				Info.CallbackTarget = this;
				UKismetSystemLibrary::MoveComponentTo(GetCapsuleComponent(), ForwardPos, ForwardRot, false, false, 0.1f, false, EMoveComponentAction::Type::Move, Info);
				
				TArray<FHitResult> hits;
				TArray<TEnumAsByte<EObjectTypeQuery>> objectType;
				objectType.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
				if (UKismetSystemLibrary::SphereTraceMultiForObjects(this, BeforePos, ForwardPos, 150.f, objectType,
					false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, hits, true))
				{
					for (int i = 0; i < hits.Num(); i++)
					{
						auto enemy = Cast<AKnightEnemy>(hits[i].GetActor());
						if (enemy == nullptr) continue;
						FDamageEvent DamageEvent;
						enemy->TakeDamage(12.0f, DamageEvent, GetController(), this);
					}
				}
				ArmLengthTo = DefaultCL;
			}), WaitTime, false);

		break;
	}
}

float AMainCharacter::FindBetweenAngle(FVector OwnerLocation, FVector OwnerForwardVec, FVector TargetLocation)
{
	
	//FVector OwnerLocation = GetActorLocation();
	//Ÿ���� ���ϴ� ���� ���
	FVector ToTargetVec = (TargetLocation - OwnerLocation);
	//Z ���� �Ұ��� (���� ���� ���ϱ� ����)
	ToTargetVec *= FVector(1.f, 1.f, 0.f);
	//����ȭ 
	ToTargetVec.Normalize();

	//ĳ���� ���� Forward Vector 
	//FVector OwnerForwardVec = GetActorForwardVector();

	//������ ã�� ���� ���� (����� cos ��Ÿ) 
	float InnerProduct = FVector::DotProduct(OwnerForwardVec, ToTargetVec);

	//Arc Cosine (����� Unsigned Degree) 
	float TargetDegree = UKismetMathLibrary::DegAcos(InnerProduct);

	//��ȣ�� ã�� ���� ���� 
	FVector OutterProduct = FVector::CrossProduct(OwnerForwardVec, ToTargetVec);

	//Yaw ���� ��ȣ ������ ���� Z ��ȣ�� ������ 
	float DegSign = UKismetMathLibrary::SignOfFloat(OutterProduct.Z);

	//���� ����� ������ ��ȣ�� ���� �������� ������������ ������ -180 ~ 180 ���� ������ ������ 
	float Result = TargetDegree * DegSign; return Result;
	return Result;
}

void AMainCharacter::Block()
{
	if (CurrentWeapon != EWeapon::Sword) return;
	IsBlock = !IsBlock;
	MainAnim->PlaySwordBlockMontage(IsBlock);
}


void AMainCharacter::ZoomButtonPressed()
{
	if (CurrentWeapon != EWeapon::Bow) return;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->MaxWalkSpeed = 400.f;

	isZooming = true;
	MainAnim->isZooming = true;
	//widget
	if(MainGameMode)
		MainGameMode->AttachAimWidget(false);
}

void AMainCharacter::ZoomButtonReleased()
{
	if (CurrentWeapon != EWeapon::Bow) return;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	isZooming = false;
	MainAnim->isZooming = false;
	if (MainGameMode)
		MainGameMode->AttachAimWidget(true);
}

void AMainCharacter::ZoomFunction(float DeltaTime)
{
	FVector TargetOffset;
	if(GetCharacterMovement()->IsCrouching())
		TargetOffset = FVector(0.f, 0.f, CrouchedEyeHeight+50.f);
	else
		TargetOffset = FVector(0.f, 0.f, BaseEyeHeight);
	if (isZooming)
	{
		TargetOffset.Y = 40.f;
		ArmLengthTo = ZoomCL;
		CurrentFOV = FMath::FInterpTo(CurrentFOV, ZoomFOV, DeltaTime, ZoomSpeed);
	}
	else
	{
		TargetOffset.Y = 0.f;
		ArmLengthTo = DefaultCL;
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaTime, ZoomSpeed);
	}

	CameraBoom->SocketOffset = (TargetOffset);

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	Sword = GetWorld()->SpawnActor<ASword>(FVector::ZeroVector, FRotator::ZeroRotator);
	SwitchWeapon(Sword, TEXT("sword_lSocket"));
	Bow = GetWorld()->SpawnActor<ABow>(FVector::ZeroVector, FRotator::ZeroRotator);
	SwitchWeapon(Bow, TEXT("BowBackSocket"));
	//ī�޶� ȭ�� ���ϱ�
	if (FollowCamera)
	{
		DefaultFOV = FollowCamera->FieldOfView;
	}
	CurrentFOV = DefaultFOV;
	MainGameMode = Cast<AMainGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}

void AMainCharacter::SetWeapon(EWeapon Weapon)
{
	SwitchWeapon(Sword, TEXT("sword_lSocket"));
	SwitchWeapon(Bow, TEXT("BowBackSocket"));
	CurrentWeapon = Weapon;
	switch (CurrentWeapon)
	{
	case EWeapon::Hand:
		
		break;

	case EWeapon::Sword:
		
		break;

	case EWeapon::Bow:

		break;
	}
}

void AMainCharacter::MoveForward(float Value)
{
	if (ClimbingComponent->isClimbing) return;
	if ((Controller != nullptr) && (Value != 0.f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		Direction.Normalize();
		MoveDirection += Direction * FMath::Clamp(Value, -1.0f, 1.0f);

	}
}

void AMainCharacter::MoveRight(float Value)
{
	if (ClimbingComponent->isClimbing) return;
	if ((Controller != nullptr) && (Value != 0.f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		Direction.Normalize();
		MoveDirection += Direction * FMath::Clamp(Value, -1.0f, 1.0f);

	}
}

void AMainCharacter::Jump()
{
	if (ClimbingComponent->isInHeight)
	{
		if (!(ClimbingComponent->isClimbing))
		{
			Super::Jump();
		}
		else if (ClimbingComponent->CanJumpLeft && InputComponent->GetAxisValue((FName("MoveRight"))) < 0.f)
		{
			ClimbingComponent->JumpLeftLedge();
		}
		else if (ClimbingComponent->CanJumpRight && InputComponent->GetAxisValue((FName("MoveRight"))) > 0.f)
		{
			ClimbingComponent->JumpRightLedge();
		}
		else if(ClimbingComponent->CanJumpUp)
		{
			ClimbingComponent->JumpUpLedge();
		}
		
	}
	else //������
	{
		FRotator TargetRot = GetControlRotation();
		TargetRot.Pitch = 0;
		TargetRot.Roll = 0;
		DisableInput(Cast<APlayerController>(GetController()));
		if (InputComponent->GetAxisValue((FName("MoveRight"))) > 0.f)
		{

			SetActorRotation(TargetRot + FRotator(0.f, 90.f, 0.f));
		}
		else if (InputComponent->GetAxisValue((FName("MoveRight"))) < 0.f)
		{
			SetActorRotation(TargetRot + FRotator(0.f, -90.f, 0.f));
		}
		else if (InputComponent->GetAxisValue((FName("MoveForward"))) < 0.f)
		{
			SetActorRotation(TargetRot + FRotator(0.f, -180.f, 0.f));
		}
		MainAnim->PlayRollMontage();
	}
	
}

void AMainCharacter::Move(float DeltaTime)
{
	if (GetMovementComponent()->IsFalling() || GetMovementComponent()->IsFlying()) { MoveDirection.Set(0.0f, 0.0f, 0.0f); return; }
	//�����϶� ����
	MoveDirection.Normalize();
	if (TargetVelocity != CurrentVelocity)
	{
		CurrentVelocity = FMath::FInterpTo(CurrentVelocity, TargetVelocity, DeltaTime, 1.5f);
	}
	AddMovementInput(MoveDirection, CurrentVelocity * DeltaTime);
	MoveDirection.Set(0.0f, 0.0f, 0.0f);
}

void AMainCharacter::Turn(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::LookUp(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::Attack()
{
	FHitResult HitResult;
	FLatentActionInfo Info;
	Info.CallbackTarget = this;
	switch (CurrentWeapon)
	{
	case EWeapon::Hand:
		
		HitResult = LineTrace();
		
		if (HitResult.Actor.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *HitResult.Actor->GetName());
			AKnightEnemy* Enemy = Cast<AKnightEnemy>(HitResult.Actor);
			float Distance = FVector::Distance(Enemy->GetActorLocation(), GetActorLocation());
			float degree = FVector::DotProduct(FollowCamera->GetForwardVector(), Enemy->GetActorForwardVector());
			if (Distance < 700.f  && degree > 0)
			{
				
				UKismetSystemLibrary::MoveComponentTo(GetCapsuleComponent(), Enemy->GetActorLocation()-Enemy->GetActorRotation().Vector()*200.f, Enemy->GetActorRotation(), false, false, 0.3f, false, EMoveComponentAction::Type::Move, Info);
				MainAnim->PlayAssassinationMontage();
				Enemy->GetCharacterMovement()->DisableMovement();
				Enemy->DisableHPBar();
				Enemy->PlayAssassination();
				//FDamageEvent DamageEvent;
				//Enemy->TakeDamage(100.0f, DamageEvent, GetController(), this);
			}
		}
		break;
	case EWeapon::Sword:
		if (IsAttacking)
		{
			if (CanNextCombo)
			{

				IsComboInputOn = true;
			}
		}
		else
		{
			check(CurrentCombo == 0);
			IsAttacking = true;
			AttackStartComboState();
			if (NearEnemy != nullptr)
			{
				FVector NearEnemyLoc = NearEnemy->GetActorLocation();
				FRotator LookRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), NearEnemyLoc);
				FRotator PlayerRot = GetActorRotation();
				PlayerRot.Yaw = LookRot.Yaw;
				UKismetSystemLibrary::MoveComponentTo(GetCapsuleComponent(), GetActorLocation() + GetActorRotation().Vector() * 30, PlayerRot, false, false, 0.3f, false, EMoveComponentAction::Type::Move, Info);
			}
			MainAnim->PlayAttackMontage();
		}
		break;
	case EWeapon::Bow:
		MainAnim->isAimming = true;
		Bow->PlayDrawBowMon(true,0);
		break;
	}

	//����
	
}

void AMainCharacter::AttackRelease()
{
	switch (CurrentWeapon)
	{
	case EWeapon::Bow:
		MainAnim->isAimming = false;
		break;
	}
}

void AMainCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if ((Montage->GetName()).Contains("Attack")&&IsAttacking)
	{
		check(CurrentCombo > 0);
		IsAttacking = false;
		AttackEndComboState();
		GetMovementComponent()->SetActive(true);
		OnAttackEnd.Broadcast();
	}
	if ((Montage->GetName()).Contains("Roll"))
	{
		EnableInput(Cast<APlayerController>(GetController()));
	}
	EnableInput(Cast<APlayerController>(GetController()));
}

void AMainCharacter::ArrowChangePlus()
{
	Bow->SetArrowType();
}

void AMainCharacter::SetManaPoint(float PlusPoint)
{


	ManaPoint += PlusPoint;
	if (ManaPoint > 100.f)
	{
		ManaPoint = 100.f;
	}
}

class UClimbingComponent* AMainCharacter::GetClimbingComponent()
{
	return ClimbingComponent;
}

void AMainCharacter::ClimbUp()
{
	ClimbingComponent->ClimbUp();
}

void AMainCharacter::ClimbDown()
{
	ClimbingComponent->DropDown();
}

void AMainCharacter::ClimbCornerLeft()
{
	
	if (ClimbingComponent->isClimbing && 
		!ClimbingComponent->CanJumpLeft && 
		ClimbingComponent->CanTurnLeft)
	{
		ClimbingComponent->IsTurning = true;
		DisableInput(UGameplayStatics::GetPlayerController(this,0));
		MainAnim->PlayLeftCornerMontage();
		FTimerHandle WaitHandle;
		float WaitTime = 1.0f;
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
			{
				EnableInput(UGameplayStatics::GetPlayerController(this, 0));
			}), WaitTime, false);
		FTimerHandle WaitHandle2;
		float WaitTime2 = 0.8f;
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
			{
				ClimbingComponent->IsTurning = false;
				ClimbingComponent->Hang();
			}), WaitTime, false);
	}
}

void AMainCharacter::ClimbCornerRight()
{
	if (ClimbingComponent->isClimbing &&
		!ClimbingComponent->CanJumpRight &&
		ClimbingComponent->CanTurnRight)
	{
		ClimbingComponent->IsTurning = true;
		DisableInput(UGameplayStatics::GetPlayerController(this, 0));
		MainAnim->PlayRightCornerMontage();
		FTimerHandle WaitHandle;
		float WaitTime = 1.0f;
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
			{
				EnableInput(UGameplayStatics::GetPlayerController(this, 0));
			}), WaitTime, false);
		FTimerHandle WaitHandle2;
		float WaitTime2 = 0.8f;
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
			{
				ClimbingComponent->IsTurning = false;
				ClimbingComponent->Hang();
			}), WaitTime, false);
	}
}

void AMainCharacter::ToggleCrouch()
{
	if (CurrentWeapon == EWeapon::Sword) { return; }
	
	if (!GetCharacterMovement()->IsCrouching()) { Crouch(); }
	else { UnCrouch(); }
	
	MainAnim->SetCrouch(GetCharacterMovement()->IsCrouching());
}



void AMainCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	CurrentCombo += 1;
}

void AMainCharacter::AttackEndComboState()
{
	CanNextCombo = false;
	IsComboInputOn = false;
	CurrentCombo = 0;
}

void AMainCharacter::AttackCheck()
{
	//�ݸ����� ������ hit actor����
	FHitResult HitResult;	//hit ������ ����
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 200.0f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(50.0f),
		Params);


#if ENABLE_DRAW_DEBUG	// DrawDebug�� ����� ������ ���� �ݸ����� �׷���

	FVector TraceVec = GetActorForwardVector() * 200.f;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = 200.f * 0.5f + 50.f;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		50.f,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);


#endif	//Damage ����
	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());
		}

		FDamageEvent DamageEvent;
		HitResult.Actor->TakeDamage(10.0f, DamageEvent, GetController(), this);
	}
}

float AMainCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	//UE_LOG(LogTemp, Warning, TEXT("Actor: %s took Damage : %f"), *GetName(), FinalDamage);
	if (IsBlock)
	{
		UGameplayStatics::SpawnSoundAtLocation(this, BlockSound, GetActorLocation());
		
		return FinalDamage;
	}
	if (FinalDamage > 0.0f)
	{
		HealthPoint -= FinalDamage;
		
		if (HealthPoint <= 0)
		{
			MainAnim->SetDeadAnim();
			SetActorEnableCollision(false);
		}
	}
	SetActorRelativeLocation(GetActorLocation() + DamageCauser->GetActorRotation().Vector() * 30);
	
	//HUD
	if (MainGameMode)
		MainGameMode->SetHealthPointHUD(HealthPoint);
	return FinalDamage;
}


// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//DrawDebugCapsule(GetWorld(), GetActorLocation(), 96.0f, 42.f,FRotationMatrix::MakeFromZ(GetActorUpVector()).ToQuat(), FColor::Green, false, DeltaTime*2.f);
	CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, ArmLengthTo, DeltaTime, 3.f);
	
	Move(DeltaTime);
	ZoomFunction(DeltaTime);
	if (isZooming) { LineTrace(); }
	
	FindNearEnemy(); 
	if (CurrentWeapon == EWeapon::Sword &&  NearEnemy != nullptr)
	{
		
		float AddYaw = FindBetweenAngle(FollowCamera->GetComponentLocation(), FollowCamera->GetForwardVector(), NearEnemy->GetActorLocation());
		if (abs(AddYaw) < 75.f)
		{
			float LookYaw = FMath::FInterpTo(GetController()->GetControlRotation().Yaw, GetController()->GetControlRotation().Yaw+AddYaw, DeltaTime, 1.f);
			//����
			GetController()->SetControlRotation(FRotator(GetControlRotation().Pitch, LookYaw, GetControlRotation().Roll));
		}
		
	}

	//Actor Rotation
	if (isZooming)
	{
		FRotator TargetRot = GetController()->GetControlRotation();
		TargetRot.Roll = 0.f;

		float AddYaw = FindBetweenAngle(GetActorLocation(),GetActorForwardVector(),GetActorLocation() + TargetRot.Vector() * 1000.f);
		if (abs(AddYaw) > 60.f)
		{
			
			//Anim
			if (AddYaw < 0)//left
			{
				MainAnim->PlayTurn90Montage(false);
			}
			if (AddYaw > 0)//right
			{
				MainAnim->PlayTurn90Montage(true);
			}
			GetMesh()->SuspendClothingSimulation();
			SetActorRotation(FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw + AddYaw, GetActorRotation().Roll));
			GetMesh()->ResumeClothingSimulation();
		}
		MainAnim->SetUpperBodyRotation( AddYaw+100.f, TargetRot.Pitch+82.f);
		
	}

	//HUD weapon image 
	if (MainGameMode)
	{
		MainGameMode->WeaponChange((uint8)CurrentWeapon);
		MainGameMode->SetExperiencePointHUD(ManaPoint);
	}
		
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	{
		Super::SetupPlayerInputComponent(PlayerInputComponent);

		PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::Jump);
		PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AMainCharacter::SetRunSpeed);
		PlayerInputComponent->BindAction("Run", IE_Released, this, &AMainCharacter::SetWalkSpeed);
		PlayerInputComponent->BindAction("DefaultAttack", IE_Pressed, this, &AMainCharacter::Attack);
		PlayerInputComponent->BindAction("DefaultAttack", IE_Released, this, &AMainCharacter::AttackRelease);
		PlayerInputComponent->BindAction("Block", IE_Pressed, this, &AMainCharacter::Block);
		PlayerInputComponent->BindAction("Block", IE_Released, this, &AMainCharacter::Block);
		PlayerInputComponent->BindAction("Shot", IE_Pressed, this, &AMainCharacter::Shot);
		PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &AMainCharacter::ZoomButtonPressed);
		PlayerInputComponent->BindAction("Zoom", IE_Released, this, &AMainCharacter::ZoomButtonReleased);
		PlayerInputComponent->BindAction("Skill1", IE_Pressed, this, &AMainCharacter::Skill1);
		PlayerInputComponent->BindAction("Skill2", IE_Pressed, this, &AMainCharacter::Skill2);
		PlayerInputComponent->BindAction("Sword", IE_Pressed, this, &AMainCharacter::UseSword);
		//PlayerInputComponent->BindAction("Hand", IE_Pressed, this, &AMainCharacter::UseHand);
		PlayerInputComponent->BindAction("Bow", IE_Pressed, this, &AMainCharacter::UseBow);
		PlayerInputComponent->BindAction("ArrowChangePlus", IE_Pressed, this, &AMainCharacter::ArrowChangePlus);
		//climb
		PlayerInputComponent->BindAction("ClimbUp", IE_Pressed, this, &AMainCharacter::ClimbUp);
		PlayerInputComponent->BindAction("ClimbDown", IE_Pressed, this, &AMainCharacter::ClimbDown);
		PlayerInputComponent->BindAction("ClimbCornerLeft", IE_Pressed, this, &AMainCharacter::ClimbCornerLeft);
		PlayerInputComponent->BindAction("ClimbCornerRight", IE_Pressed, this, &AMainCharacter::ClimbCornerRight);
		//Crouch
		PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMainCharacter::ToggleCrouch);

		PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
		PlayerInputComponent->BindAxis("Turn", this, &AMainCharacter::Turn);
		PlayerInputComponent->BindAxis("LookUp", this, &AMainCharacter::LookUp);
	}
}

void AMainCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *(GetMesh()->GetAnimInstance()->GetName()));
	MainAnim = Cast<UMainAnimInstance>(GetMesh()->GetAnimInstance());
	check(nullptr != MainAnim);

	MainAnim->OnMontageEnded.AddDynamic(this, &AMainCharacter::OnAttackMontageEnded);
	MainAnim->OnNextAttackCheck.AddLambda([this]() -> void {
		//UE_LOG(LogTemp, Warning, TEXT("OnNextAttackCheck"));
		CanNextCombo = false;

		if (IsComboInputOn)
		{
			
			if (NearEnemy != nullptr)
			{
				FVector NearEnemyLoc = NearEnemy->GetActorLocation();
				FLatentActionInfo Info;
				FRotator LookRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), NearEnemyLoc);
				FRotator PlayerRot = GetActorRotation();
				PlayerRot.Yaw = LookRot.Yaw;
				Info.CallbackTarget = this;
				UKismetSystemLibrary::MoveComponentTo(GetCapsuleComponent(), GetActorLocation() + GetActorRotation().Vector() * 30, PlayerRot, false, false, 0.3f, false, EMoveComponentAction::Type::Move, Info);
			}
			//SetActorRelativeLocation(GetActorLocation() + GetActorRotation().Vector() * 30);
			AttackStartComboState();
			//UE_LOG(LogTemp, Warning, TEXT("Combo: %d"), CurrentCombo);
			MainAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});
	MainAnim->OnAttackHitCheck.AddLambda([this]() -> void {
		AttackCheck();

	});
	MainAnim->OnEQUIP.AddLambda([this]() -> void {
		switch (CurrentWeapon)
		{
		case EWeapon::Sword:
			SwitchWeapon(Sword, TEXT("hand_rSocket"));
			break;
		case EWeapon::Bow:
			SwitchWeapon(Bow, TEXT("BowSocket"));
			break;
		}
		
	});
	MainAnim->Onsheath.AddLambda([this]() -> void {
		switch (CurrentWeapon)
		{
		case EWeapon::Sword:
			SwitchWeapon(Sword, TEXT("sword_lSocket"));
			break;
		case EWeapon::Bow:
			SwitchWeapon(Bow, TEXT("BowBackSocket"));
			break;
		}
		SetWeapon(EWeapon::Hand);
	});
	MainAnim->OnBowFire.AddLambda([this]()->void{
		Bow->PlayDrawBowMon(false,0);
	});
	MainAnim->OnBowSkill1.AddLambda([this]()->void {
		Bow->SpawnWideArrow();
		});
	
}

void AMainCharacter::SwitchWeapon(AActor* NewWeapon, FName WeaponSocket)
{
	
	if (nullptr != NewWeapon)
	{
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, WeaponSocket);
		NewWeapon->SetOwner(this);
	}
}

void AMainCharacter::UseSword()
{
	if (isZooming) return;
	if (GetCharacterMovement()->IsCrouching())
	{
		ToggleCrouch();
	}
	MainAnim->PlayDrawSwordMontage();
	if (CurrentWeapon == EWeapon::Sword)
	{
		MainAnim->JumpToSwordMontageSection(2);

	}
	else
	{
		SetWeapon(EWeapon::Sword);
	}
	
	
}

void AMainCharacter::UseBow()
{
	if (isZooming) return;

	MainAnim->PlayDrawBowMontage();
	if (CurrentWeapon == EWeapon::Bow)
	{
		MainAnim->JumpToSwordMontageSection(2);
	}
	else
	{
		SetWeapon(EWeapon::Bow);
	}
}

void AMainCharacter::FindNearEnemy()
{
	FVector CameraVec = FollowCamera->GetForwardVector();
	CameraVec.Z = 0;
	FVector StartPos = GetActorLocation();
	FVector EndPos = StartPos + CameraVec * 3000.f;
	TArray<FHitResult> hits;
	TArray<TEnumAsByte<EObjectTypeQuery>> objectType;
	TEnumAsByte<EObjectTypeQuery> Enemy = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody);
	objectType.Emplace(Enemy);
	AActor* closetEnemy = nullptr;
	if (UKismetSystemLibrary::SphereTraceMultiForObjects(this, StartPos, EndPos, 300.f, objectType, false, TArray<AActor*>(), EDrawDebugTrace::None, hits, true))
	{
		for (int i = 0; i < hits.Num(); i++)
		{
			auto enemy = Cast<AKnightEnemy>(hits[i].GetActor());
			if (enemy == nullptr) continue;
			if (closetEnemy == nullptr)
			{
				closetEnemy = enemy;
			}
			else if (this->GetDistanceTo(closetEnemy) > this->GetDistanceTo(enemy))
			{
				
				closetEnemy = enemy;
			}
		}
	}
	NearEnemy = closetEnemy;
}



