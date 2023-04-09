// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "GameFramework/Character.h"
#include "BossCharacter.generated.h"

UENUM(BlueprintType)
enum class EBossAttackState : uint8
{
	Near UMETA(DisplayName = "Near"),
	Medium UMETA(DisplayName = "Medium"),
	Far UMETA(DisplayName = "Far"),
};

UCLASS()
class PORTFOLIO_API ABossCharacter : public AEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Attack() override;
	void MediumSkill();
	void FarSkill();
protected:
		virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted) override;
private:
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		class ASword* Sword;
	UBossAnimInstance* BossAnim;
	int32 ComboNum = 1;
protected:
	TSubclassOf<UUserWidget> MainHUDWidgetClass;
	class UMainHUDWidget* MainHUDWidget;

};
 