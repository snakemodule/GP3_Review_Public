#pragma once

#include "GameFramework/Actor.h"
#include "GPActivityExitPoint.h"

#include "GPActivitySpotBase.generated.h"

class AGPCreatureBase;
class UBehaviorTree;
class USoundCue;

UCLASS(Abstract, BlueprintType)
class AGPActivitySpotBase : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* ActivityAnimMontage;
	
	UPROPERTY(EditAnywhere)
	TArray<FGPActivityExitPoint> ExitPoints;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetExitPointLocationFarthestFromPlayer() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetActivityLocationWorldSpace() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetActivityLookTargetWorldSpace() const;

	/* A voice line that is meant to be played when the this activity has successfully been observed */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	
	USoundCue* ActivityDiscoveryCompletionVoiceCue;

	UFUNCTION(BlueprintImplementableEvent)
	void CreatureSpawned();

private:
	UPROPERTY(EditAnywhere, meta=(MakeEditWidget))
	FVector ActivityLocation;

	UPROPERTY(EditAnywhere, meta=(MakeEditWidget))
	FVector ActivityLookTarget;
};