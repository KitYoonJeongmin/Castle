// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/ArrowComponent.h"
#include "ClimbingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_API UClimbingComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UClimbingComponent();
	virtual void InitializeComponent() override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//climb up& Drop
	void ForwardTrace();
	void HeightTrace();
	void Hang();
	void DropDown();
	void ClimbUp();

	bool isClimbing = false;
	bool isInForward = false;
	bool isInHeight = false;
	bool isFinish = false;
	
	//left&right movement
	void LeftTrace();
	void RightTrace();
	void MoveInLedge();

	bool CanMoveLeft = false;
	bool CanMoveRight = false;
	bool IsMovingLeft = false;
	bool IsMovingRight = false;

	//jump to side wall
	void JumpLeftTrace();
	void JumpRightTrace();
	void JumpLeftLedge();
	void JumpRightLedge();
	void JumpLeft(bool isJumpLeft);
	void JumpRight(bool isJumpRight);

	bool CanJumpLeft = false;
	bool CanJumpRight = false;
	bool IsJumping = false;
	bool IsJumpLeft = false;
	bool IsJumpRight = false;

	//Turn Corner
	void TurnLeftTrace();
	void TurnRightTrace();

	bool CanTurnLeft = false;
	bool CanTurnRight = false;
	bool IsTurning = false;

	//JumpUp
	void JumpUpTrace();
	void JumpUpLedge();
	void JumpUp(bool jumpUp);
	bool CanJumpUp = false;
private:
	FVector CharLoc;
	FVector CharFor;
	FVector HeightLoc;
	FVector WallNormal;
	FVector WallLoc;

	ACharacter* Character;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Climbing, Meta = (AllowPrivateAccess = true))
		UAnimMontage* ClimbIdleMon;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Climbing, Meta = (AllowPrivateAccess = true))
		UAnimMontage* ClimbMon;

	UPROPERTY(VisibleAnywhere, Category = "TraceArrow")
		UArrowComponent* LeftClimbArrow;
	UPROPERTY(VisibleAnywhere, Category = "TraceArrow")
		UArrowComponent* RightClimbArrow;
	UPROPERTY(VisibleAnywhere, Category = "TraceArrow")
		UArrowComponent* LeftLedgeArrow;
	UPROPERTY(VisibleAnywhere, Category = "TraceArrow")
		UArrowComponent* RightLedgeArrow;
	UPROPERTY(VisibleAnywhere, Category = "TraceArrow")
		UArrowComponent* UpArrow;
};
