#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "GPNavMovementComponent.generated.h"

UCLASS(BlueprintType)
class UGPNavMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
public:
	UGPNavMovementComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;
	virtual void RequestPathMove(const FVector& MoveInput) override;
	virtual bool CanStartPathFollowing() const override;
	virtual bool CanStopPathFollowing() const override;
	virtual void StopActiveMovement() override;
	virtual float GetPathFollowingBrakingDistance(float MaxSpeed) const override;
	const INavigationDataInterface* GetNavData() const;
	bool FindNavFloor(const FVector& TestLocation, FNavLocation& NavFloorLocation) const;

	UFUNCTION(BlueprintPure)
	bool IsMoving() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float MovementSpeed = 400.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float VisualRotationSpeed = 90.0f;

	bool bHasRequestedVelocity = false;
};
