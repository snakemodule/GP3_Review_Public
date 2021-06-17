#pragma once

#include "Engine/DataAsset.h"
#include "GPVisionSenseSettings.generated.h"

UCLASS()
class UGPVisionSenseSettings : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category=Sensing)
	float VisionHalfAngle = 90.0f;

	UPROPERTY(EditAnywhere, Category=Sensing)
	float MaxDistance = 2000.0f;
};