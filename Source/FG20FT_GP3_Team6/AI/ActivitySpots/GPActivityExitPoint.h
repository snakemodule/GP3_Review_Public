#pragma once

#include "GPActivityExitPoint.generated.h"

class AGPActivitySpotBase;

USTRUCT()
struct FGPActivityExitPoint
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta=(MakeEditWidget))
	FVector Location;
};