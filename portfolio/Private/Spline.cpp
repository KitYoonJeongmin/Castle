// Fill out your copyright notice in the Description page of Project Settings.


#include "Spline.h"
#include "Components/SplineComponent.h"
// Sets default values
ASpline::ASpline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PatrolPath = CreateDefaultSubobject<USplineComponent>(TEXT("PatrolPath"));
	
}

float ASpline::GetSplineLength()
{
	return PatrolPath->GetSplineLength();
}

FVector ASpline::GetLocationAtDistanceAlongSpline(float Distance, ESplineCoordinateSpace::Type CoordinateSpace)
{
	return PatrolPath->GetLocationAtDistanceAlongSpline(Distance, CoordinateSpace);
}

// Called when the game starts or when spawned
void ASpline::BeginPlay()
{
	Super::BeginPlay();
	
}




