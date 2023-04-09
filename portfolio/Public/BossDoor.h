// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interact.h"
#include "BossDoor.generated.h"

UCLASS()
class PORTFOLIO_API ABossDoor : public AActor, public IInteract
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void GoBossMap();
	virtual void Activate_Implementation() override;
	virtual void SetWidget_Implementation(bool IsVisible) override;
private:
	UPROPERTY(VisibleAnywhere, Category = Mesh, Meta = (AllowPrivateAccess = true))
		UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnyWhere, Category = UI)
		class UWidgetComponent* InteractWidget;
	class USphereComponent* Sphere;
};
