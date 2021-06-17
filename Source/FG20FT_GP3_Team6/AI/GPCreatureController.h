#pragma once

#include "AIController.h"
#include "GPCreatureController.generated.h"

class USplineComponent;

UCLASS()
class AGPCreatureController : public AAIController
{
	GENERATED_BODY()

public:
	AGPCreatureController();

	virtual void Tick(float DeltaSeconds) override;
	virtual FAIRequestID RequestMove(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr Path) override;

	UPROPERTY(VisibleDefaultsOnly)
	USplineComponent* PathSmoothingSpline; // make sure its settings cant be changed in editor?
};
