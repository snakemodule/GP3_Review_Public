#pragma once

#include "Engine/DataAsset.h"
#include "GPSonarSenseSettings.generated.h"

UCLASS()
class UGPSonarSenseSettings : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category=Sensing)
	float MaxRange = 500.0f;

	UPROPERTY(EditAnywhere, Category=Sensing)
	float ExpansionDuration = 2.0f;

	UPROPERTY(EditAnywhere, Category=Sensing)
	float MaxRangeDuration = 1.0f;

	UPROPERTY(EditAnywhere, Category=Sensing)
	float TimeBetweenPulses = 1.0f;
};