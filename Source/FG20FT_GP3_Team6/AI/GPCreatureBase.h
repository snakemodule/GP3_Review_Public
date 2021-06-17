#pragma once

#include "FG20FT_GP3_Team6/Player/Tools/Spyglass/GPBPEventDiscoverableComponent.h"
#include "GameFramework/Pawn.h"
#include "Sound/SoundCue.h"

#include "GPCreatureBase.generated.h"

class UCapsuleComponent;
class UGPNavMovementComponent;
class UGPVisionSenseComponent;
class UGPSonarSenseComponent;
class AGPActivitySpotBase;

UCLASS()
class AGPCreatureBase : public APawn
{
	GENERATED_BODY()

public:
	AGPCreatureBase();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaSeconds) override;

public:
	UPROPERTY(VisibleDefaultsOnly, Category=Collision)
	UCapsuleComponent* Capsule;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Mesh)
	UStaticMeshComponent* SonarSphere;

	UPROPERTY(VisibleDefaultsOnly, Category = Effects)
	UAudioComponent* PhasingAudioComponent;
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	USoundCue* PhasingSound;
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	float PhasingNoiseFadeInDistance = 300;
	
	UPROPERTY(VisibleDefaultsOnly, Category=Effects)
	UAudioComponent* AudioComponent;
	UPROPERTY(EditDefaultsOnly, Category=Effects)
	USceneComponent* DissolvePlane;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Movement)
	UGPNavMovementComponent* NavMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Sensing)
	UGPVisionSenseComponent* VisionSenseComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Sensing)
	UGPSonarSenseComponent* SonarSenseComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Discovery)
	UGPBPEventDiscoverableComponent* DiscoverableComponent;

	UPROPERTY(Transient)
	UMaterialInstanceDynamic* AntlerDynamicMaterial;

	UFUNCTION()
	virtual void SetActivitySpot(AGPActivitySpotBase* ActivitySpot);

	UFUNCTION(BlueprintCallable)
	void BecomeDiscovered();

	UFUNCTION(BlueprintCallable)
	void SetDiscoverability(bool bDiscoverability);

	UFUNCTION(BlueprintCallable)
	void LockDiscoveryDelayed(float DelaySeconds);

	UFUNCTION(BlueprintImplementableEvent)
	void SetDissolvePlaneValues(FVector PlanePosition, FVector PlaneNormal);	
	void SetDissolvePlane(FVector PlanePosition, FVector PlaneNormal);

	UFUNCTION(BlueprintCallable)
	FVector SelectExitPointFarthestFromPlayer();

	UPROPERTY(EditDefaultsOnly)
	float PhasePlaneSpawnOffset = 400.0f;

	UPROPERTY(BlueprintReadOnly)
	AGPActivitySpotBase* CurrentActivitySpot;

private:
	bool bDiscoveryLockTimerActive = false;
	float TimeUntilDiscoveryLock = 0.0f;
};
