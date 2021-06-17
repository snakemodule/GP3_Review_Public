// Fill out your copyright notice in the Description page of Project Settings.


#include "GPMovementComponent.h"


// Called every frame
void UGPMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FacingRotationCurrent = FQuat::Slerp(FacingRotationCurrent.Quaternion(), FacingRotationTarget.Quaternion(),
	                                     FacingRotationSpeed * DeltaTime).Rotator();

	if (FacingRotationCurrent.Equals(FacingRotationTarget))
	{
		FacingRotationCurrent = FacingRotationTarget;
		SetComponentTickEnabled(false);
	}

	FacingRotationSpeed = 1.0f;
}

GPFrameMovement UGPMovementComponent::CreateFrameMovement() const
{
	return GPFrameMovement(UpdatedComponent);
}

bool UGPMovementComponent::Fall(GPFrameMovement& FrameMovement)
{
	bool falling = false;
	FHitResult Hit;
	FVector Delta = FrameMovement.GetGravity();
	//if (FrameMovement.ShouldApplyGravity())
	{
		SafeMoveUpdatedComponent(Delta, UpdatedComponent->GetComponentQuat(), true, Hit);

		if (!Hit.bBlockingHit)
		{
			falling = true;
		}
		else
		{
			auto SlopeDeg = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Hit.ImpactNormal, FVector::UpVector)));
			if ( SlopeDeg > SlopeLimitDeg)
			{
				Delta = FrameMovement.GetGravity();
				SlideAlongSurface(Delta, 1.0f - Hit.Time, Hit.Normal, Hit);
				falling = true;
			}
		}
	}

	return falling;
}


void UGPMovementComponent::Move(GPFrameMovement& FrameMovement)
{
	FHitResult Hit;

	// First perform movement with gravity
	//if (FrameMovement.ShouldApplyGravity())
	//{
	//	SafeMoveUpdatedComponent(FrameMovement.GetGravity(), UpdatedComponent->GetComponentQuat(), true, Hit);
	//	Hit.Reset();
	//}

	FVector Delta = FrameMovement.GetMovementDelta();

	if (!Delta.IsNearlyZero())
	{
		SafeMoveUpdatedComponent(Delta, UpdatedComponent->GetComponentQuat(), true, Hit);

		if (Hit.bBlockingHit)
		{
			Delta = FrameMovement.GetMovementDelta();			
			SlideAlongSurfaceLimited(Delta, 1.0f - Hit.Time, Hit.Normal, Hit);
		}

	}
	FrameMovement.FinalLocation = UpdatedComponent->GetComponentLocation();
}

float UGPMovementComponent::SlideAlongSurfaceLimited(const FVector& Delta, float Time, const FVector& Normal,
                                                     FHitResult& Hit, bool bHandleImpact)
{
	if (!Hit.bBlockingHit)
	{
		return 0.f;
	}

	float PercentTimeApplied = 0.f;
	const FVector OldHitNormal = Normal;

	FVector SlideDelta = ComputeSlideVector(Delta, Time, Normal, Hit);

	if (FMath::RadiansToDegrees(FMath::Asin(SlideDelta.GetSafeNormal().Z)) > 45)
	{
		SlideDelta.Z = 0; //don't try to move up if sliding angle is too great
	}

	if ((SlideDelta | Delta) > 0.f)
	{
		const FQuat Rotation = UpdatedComponent->GetComponentQuat();
		SafeMoveUpdatedComponent(SlideDelta, Rotation, true, Hit);

		const float FirstHitPercent = Hit.Time;
		PercentTimeApplied = FirstHitPercent;
		if (Hit.IsValidBlockingHit())
		{
			// Notify first impact
			if (bHandleImpact)
			{
				HandleImpact(Hit, FirstHitPercent * Time, SlideDelta);
			}

			// Compute new slide normal when hitting multiple surfaces.
			TwoWallAdjust(SlideDelta, Hit, OldHitNormal);

			// Only proceed if the new direction is of significant length and not in reverse of original attempted move.
			if (!SlideDelta.IsNearlyZero(1e-3f) && (SlideDelta | Delta) > 0.f)
			{
				// Perform second move
				SafeMoveUpdatedComponent(SlideDelta, Rotation, true, Hit);
				const float SecondHitPercent = Hit.Time * (1.f - FirstHitPercent);
				PercentTimeApplied += SecondHitPercent;

				// Notify second impact
				if (bHandleImpact && Hit.bBlockingHit)
				{
					HandleImpact(Hit, SecondHitPercent * Time, SlideDelta);
				}
			}
		}

		return FMath::Clamp(PercentTimeApplied, 0.f, 1.f);
	}

	return 0.f;
}

void UGPMovementComponent::SetFacingRotation(const FRotator& InFacingRotation, float InRotationSpeed)
{
	Internal_SetFacingRotation(InFacingRotation, InRotationSpeed);
}

void UGPMovementComponent::SetFacingRotation(const FQuat& InFacingRotation, float InRotationSpeed)
{
	Internal_SetFacingRotation(InFacingRotation.Rotator(), InRotationSpeed);
}

void UGPMovementComponent::SetFacingDirection(const FVector& InFacingDirection, float InRotationSpeed)
{
	Internal_SetFacingRotation(InFacingDirection.Rotation(), InRotationSpeed);
}

void UGPMovementComponent::Internal_SetFacingRotation(const FRotator& InFacingRotation, float InRotationSpeed)
{
	FRotator NewRotation = InFacingRotation;
	NewRotation.Roll = 0.0f;
	NewRotation.Pitch = 0.0f;
	FacingRotationTarget = NewRotation;

	if (InRotationSpeed < 0.0f)
	{
		FacingRotationCurrent = NewRotation;
		SetComponentTickEnabled(false);
	}
	else
	{
		SetComponentTickEnabled(true);
		FacingRotationSpeed = InRotationSpeed;
	}
}
