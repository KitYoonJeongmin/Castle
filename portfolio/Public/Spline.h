// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "Spline.generated.h"

UCLASS()
class PORTFOLIO_API ASpline : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpline();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	float GetSplineLength();
	FVector GetLocationAtDistanceAlongSpline(float Distance, ESplineCoordinateSpace::Type CoordinateSpace);
private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Path", Meta = (AllowPrivateAccess = true))
	class USplineComponent* PatrolPath;

};
