// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "GameFramework/Actor.h"
#include "GPTrail.generated.h"

class AGPActivitySpotBase;
class USphereComponent;



UCLASS()
class AGPTrail : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGPTrail();

	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* SceneRoot;

	UPROPERTY(EditInstanceOnly)
	USphereComponent* CreatureSpawnTrigger;

	UPROPERTY(EditInstanceOnly)
	AGPActivitySpotBase* Activity = nullptr;

	UPROPERTY(EditAnywhere)
	float SecondsToStayActive = 75.f;

	UFUNCTION(BlueprintImplementableEvent)
	void TrailBecomeActive();
	
	UFUNCTION(BlueprintImplementableEvent)
	void TrailBecomeInactive();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Activate();
	void Deactivate();
	
private:
	UFUNCTION()
	void SpawnCreature(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	float ActiveTimer = 0;
};
   