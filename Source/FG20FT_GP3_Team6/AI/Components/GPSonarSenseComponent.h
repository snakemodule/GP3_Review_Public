#pragma once

#include "Components/ActorComponent.h"
#include "FG20FT_GP3_Team6/AI/GPCreatureBase.h"
#include "FG20FT_GP3_Team6/AI/Components/SenseResults/GPSonarSenseResults.h"
#include "Sound/SoundCue.h"

#include "GPSonarSenseComponent.generated.h"

class UGPSonarSenseSettings;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGPSonarSenseDelegate, const FGPSonarSenseResults&, Results);

UCLASS(BlueprintType)
class UGPSonarSenseComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGPSonarSenseComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable)
	FGPSonarSenseDelegate OnSonarTargetFound;

	UPROPERTY(EditDefaultsOnly, Category = Sensing)
	UGPSonarSenseSettings* SenseSettings;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* SonarSound;
	
	UPROPERTY(EditAnywhere, Category = Debug)
	bool bDebugDrawPulse = true;
	
	bool bIsSonarActive = false;
	float SonarTimer = 0.0f;

	UPROPERTY(Transient)
	UStaticMeshComponent* SonarSphere;

	UPROPERTY(EditDefaultsOnly)
	float SphereMeshRadius = 50.f;
	/* Sonar pings should not happen when the creature is within this distance of the dissolve plane */
	UPROPERTY(EditDefaultsOnly)
	float PhasingThresholdDistance = 400.f;

private:
	void UpdateSonar(float DeltaTime);
};