// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arrow.generated.h"

UENUM(BlueprintType)
enum class EArrowState : uint8
{
	Default = 0 UMETA(DisplayName = "Default"),
	Fire = 1 UMETA(DisplayName = "Fire"),
	Poision = 2 UMETA(DisplayName = "Poision"),
	Light = 3 UMETA(DisplayName = "Poision"),

};

UCLASS()
class PORTFOLIO_API AArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArrow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	// 화살의 Overlap이 감지돼 공격할 때 실행
	UFUNCTION()
		void ArrowBeginOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Fire(AActor* Target ,FVector ForwardVec, FVector HitPoint);
	void FireStrate(FVector TargetLoc);
	void SetArrowState(uint8 num);
	void SetLightMat(bool isLight);
	UStaticMeshComponent* GetMesh();
private:
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		UStaticMeshComponent* Arrow;
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		UStaticMesh* LightArrowMesh;
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		UStaticMesh* DefaultArrowMesh;
	UPROPERTY(VisibleAnywhere, Category = State)
		EArrowState CurrentState;

	//파티클
	UPROPERTY(VisibleAnywhere, Category = Particle)
		class UParticleSystemComponent* ArrowParticle;
	UPROPERTY(VisibleAnywhere, Category = Particle)
		class UParticleSystem* FireParticle;
	UPROPERTY(VisibleAnywhere, Category = Particle)
		class UParticleSystem* PoisionParticle;
	
	//Poision
	UPROPERTY(VisibleAnywhere, Category = Poision)
		float PoisionTime;
	UPROPERTY(VisibleAnywhere, Category = Poision)
		float RemainTime;
	UPROPERTY(VisibleAnywhere, Category = Poision)
		AActor* PoisionActor;
	//머티리얼
	UPROPERTY(VisibleAnywhere, Category = Material)
		UMaterial* LightingArrowMat;
	UPROPERTY(VisibleAnywhere, Category = Material)
		UMaterial* DefaultArrowMat;
};
