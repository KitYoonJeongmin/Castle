// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interact.h"
#include "NPCCharacter.generated.h"

UCLASS()
class PORTFOLIO_API ANPCCharacter : public ACharacter, public IInteract
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPCCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void NextDiagolue();
	virtual void Activate_Implementation() override;
	virtual void SetWidget_Implementation(bool IsVisible) override;
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:
	class USphereComponent* Sphere;

	TArray<FString> DialogueArr;
	int32 CurrentDia = -1;

	UPROPERTY()
		class UDialogueUserWidget* DialogueWidget;
	UPROPERTY()
		TSubclassOf<UUserWidget> DialogueWidgetClass;

	UPROPERTY(VisibleAnyWhere, Category = UI)
		class UWidgetComponent* InteractWidget;
};
