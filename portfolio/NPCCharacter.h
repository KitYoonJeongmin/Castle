// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPCCharacter.generated.h"

UCLASS()
class PORTFOLIO_API ANPCCharacter : public ACharacter
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
private:
	class USphereComponent* Sphere;

	TArray<FString> DialogueArr;
	int32 CurrentDia = -1;

	UPROPERTY()
		class UDialogueUserWidget* DialogueWidget;
	UPROPERTY()
		TSubclassOf<UUserWidget> DialogueWidgetClass;
};
