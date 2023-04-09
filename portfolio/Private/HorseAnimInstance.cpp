// Fill out your copyright notice in the Description page of Project Settings.


#include "HorseAnimInstance.h"
#include "RidingHorse.h"
UHorseAnimInstance::UHorseAnimInstance()
{
	CurrentPawnForwardSpeed = 0.f;
	CurrentPawnRotation = 0.f;
}

void UHorseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	auto Character = Cast<ARidingHorse>(TryGetPawnOwner());
	if (!::IsValid(Character)) { return; }
	CurrentPawnForwardSpeed = Character->ForwardSpeed;
	CurrentPawnRotation = Character->RightSpeed/7.f;
}
