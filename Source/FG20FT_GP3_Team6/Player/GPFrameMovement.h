#pragma once

#include "CoreMinimal.h"

class AActor;
class USceneComponent;

//todo maybe move this
class GPFrameMovement
{
public:
	GPFrameMovement(const FVector& InStartLocation) :
		StartLocation(InStartLocation)
	{
	}

	GPFrameMovement(AActor* InActor) { StartLocation = InActor->GetActorLocation(); };
	GPFrameMovement(USceneComponent* InSceneComponent) { StartLocation = InSceneComponent->GetComponentLocation(); };

	void AddDelta(const FVector& InDelta) { MovementDelta += InDelta; };
	void AddGravity(const float GravityToApply) { AccumulatedGravity -= GravityToApply; }

	bool ShouldApplyGravity() const { return !FMath::IsNearlyZero(AccumulatedGravity); }

	FVector GetMovementDelta() const { return MovementDelta; }

	FVector FinalLocation = FVector::ZeroVector;

	FVector GetStartLocation() const { return StartLocation; }
	FVector GetGravity() const { return FVector(0.0f, 0.0f, AccumulatedGravity); }

private:
	FVector MovementDelta = FVector::ZeroVector;
	FVector StartLocation = FVector::ZeroVector;
	float AccumulatedGravity = 0.0f;
};
