#pragma once

#include "GPFrameMovement.h"

#include "GameFramework/MovementComponent.h"
#include "GPMovementComponent.generated.h"



UCLASS()
class UGPMovementComponent : public UMovementComponent
{
	GENERATED_BODY()
public:

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	GPFrameMovement CreateFrameMovement() const;

	bool Fall(GPFrameMovement& FrameMovement);
	void Move(GPFrameMovement& FrameMovement);

	float SlideAlongSurfaceLimited(const FVector& Delta, float Time, const FVector& Normal, FHitResult& Hit,
	                               bool bHandleImpact = false);

	FRotator GetFacingRotation() const { return FacingRotationCurrent; }
	FVector GetFacingDirection() const { return FacingRotationCurrent.Vector(); }

	void SetFacingRotation(const FRotator& InFacingRotation, float InRotationSpeed = -1.0f);
	void SetFacingRotation(const FQuat& InFacingRotation, float InRotationSpeed = -1.0f);
	void SetFacingDirection(const FVector& InFacingDirection, float InRotationSpeed = -1.0f);



private:
	void Internal_SetFacingRotation(const FRotator& InFacingRotation, float InRotationSpeed);	

	FRotator FacingRotationCurrent;
	FRotator FacingRotationTarget;
	float FacingRotationSpeed;

	UPROPERTY(EditDefaultsOnly)
	float SlopeLimitDeg = 45;
};


