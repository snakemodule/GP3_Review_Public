#pragma once

#include "FG20FT_GP3_Team6/Player/Tools/GPToolBase.h"
#include "GPMap.generated.h"

class UBoxComponent;


UCLASS()
class AGPMap : public AGPToolBase
{
	GENERATED_BODY()

public:
	AGPMap();

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* CompassNeedle;
	UPROPERTY(EditDefaultsOnly)
	float CompassNeedleSpeed = 2.f;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Compass;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Root;
	FVector NorthDirection;
	FRotator LastRotation;
	FRotator PlayerRotationOffset;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;


	virtual void StartSwap(AGPToolBase* Tool, void (AGPPlayerPawn::*OnSwapEvent)()) override;
	virtual void Equip() override;

	void RotateNeedleToNorth();
	void RotateNeedle(float RelativeSpeed);
};