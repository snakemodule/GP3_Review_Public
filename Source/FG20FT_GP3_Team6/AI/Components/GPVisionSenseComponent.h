#pragma once

#include "Components/ActorComponent.h"
#include "FG20FT_GP3_Team6/AI/Components/SenseResults/GPVisionSenseResults.h"
#include "GPVisionSenseComponent.generated.h"

class UGPVisionSenseSettings;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGPVisionSenseDelegate, const FGPVisionSenseResults&, Results);

UCLASS(BlueprintType)
class UGPVisionSenseComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	//TODO vision functionality
	UGPVisionSenseComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool IsPlayerVisible(const FVector& VisionOrigin, const FVector& VisionDirection);
	
	UPROPERTY(BlueprintAssignable)
	FGPVisionSenseDelegate OnVisionTargetSensed;

	UPROPERTY(BlueprintAssignable)
	FGPVisionSenseDelegate OnVisionTargetLost;
	
	UPROPERTY(EditAnywhere, Category = Sensing)
	UGPVisionSenseSettings* SenseSettings = nullptr;

	UPROPERTY(EditAnywhere, Category = Debug)
	bool bDebugDrawVision = false;

	bool bCanSeePlayer = false;
};

