// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "Components/ActorComponent.h"

#include "GPDiscoverableComponent.generated.h"

class AGPSpyglass;
class AGPPlayerPawn;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FG20FT_GP3_TEAM6_API UGPDiscoverableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGPDiscoverableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	bool bDiscoveryLocked = false;

	virtual void Discover(AGPPlayerPawn* Discoverer);
};
