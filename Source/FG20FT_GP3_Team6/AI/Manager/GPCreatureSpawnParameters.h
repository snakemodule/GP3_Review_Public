#pragma once

#include "GPCreatureSpawnParameters.generated.h"

class AGPActivitySpotBase;

USTRUCT(BlueprintType)
struct FGPCreatureSpawnParameters
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	AGPActivitySpotBase* ActivitySpot;

	UPROPERTY(BlueprintReadWrite)
	FVector SpawnLocation;
};