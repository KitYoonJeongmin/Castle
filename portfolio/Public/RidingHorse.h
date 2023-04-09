// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interact.h"
#include "RidingHorse.generated.h"

UCLASS()
class PORTFOLIO_API ARidingHorse : public ACharacter, public IInteract
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARidingHorse();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Activate_Implementation() override;
	virtual void SetWidget_Implementation(bool IsVisible) override;
public:
	/**ī�޶� ĳ���� �ڿ� ��ġ*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess))
		class USpringArmComponent* CameraBoom;
	/** ����ٴϴ� ī�޶�*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
	/** �ڽ� �ݸ���*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* BoxComponent;

	//�ɱ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mount, meta = (AllowPrivateAccess = "true"))
		TArray<USceneComponent*> MountPos;

	/** ��, �� �Է½� Call */
	void MoveForward(float Value);
	/** ��, �� �Է½� Call */
	void MoveRight(float Value);
	/**��ȭ�� ȸ������ �Է½� Call*/
	void Turn(float Rate);
	/**��ȭ�� ���Ʒ����� �Է½� Call*/
	void LookUp(float Rate);

	void Mount();
	void Dismount();
	float ForwardValue;
	float RightValue;

	float ForwardSpeed;
	float RightSpeed;

	FRotator NextForRot;
	FRotator NextRightRot;

	void SetRunSpeed();
	void SetWalkSpeed();
	float Velocity = 1.f;
	float TargetVel = 1.f;
private:
	UPROPERTY(VisibleAnyWhere, Category = UI)
		class UWidgetComponent* InteractWidget;
};
