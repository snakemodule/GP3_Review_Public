#pragma once

#include "GPSonarSenseResults.generated.h"

USTRUCT(BlueprintType)
struct FGPSonarSenseResults
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	FVector PlayerLocation;

	UPROPERTY(BlueprintReadOnly)
	float DistanceToPlayer;
};