#pragma once

#include "GPVisionSenseResults.generated.h"

USTRUCT(BlueprintType)
struct FGPVisionSenseResults
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	AActor* SpottedActor;
};