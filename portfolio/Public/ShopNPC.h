// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interact.h"
#include "ShopNPC.generated.h"

UCLASS()
class PORTFOLIO_API AShopNPC : public ACharacter, public IInteract
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShopNPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Activate_Implementation() override;
	virtual void SetWidget_Implementation(bool IsVisible) override;
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void ShowShopWidget();
private:
	class USphereComponent* Sphere;
	UPROPERTY(VisibleAnyWhere, Category = UI)
		class UWidgetComponent* InteractWidget;
	UPROPERTY()
		class UShopWidget* ShopWidget;
	UPROPERTY()
		TSubclassOf<class UShopWidget> ShopWidgetClass;


};
