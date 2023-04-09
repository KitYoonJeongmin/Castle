// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interact.h"
#include "ItemGen.generated.h"

UCLASS()
class PORTFOLIO_API AItemGen : public AActor, public IInteract
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemGen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Activate_Implementation() override;
	virtual void SetWidget_Implementation(bool IsVisible) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void GenerateItem();
private:
	UPROPERTY(VisibleAnyWhere, Category = UI)
		class UWidgetComponent* InteractWidget;
	UPROPERTY(VisibleAnyWhere, Category = COLLISION)
		class USphereComponent* Sphere;
};
