// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FG20FT_GP3_Team6/Player/Tools/Spyglass/GPDiscoverableComponent.h"

#include "GPBPEventDiscoverableComponent.generated.h"

class AGPSpyglass;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGPDiscoveryDelegate, const AGPPlayerPawn*, Discoverer);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FG20FT_GP3_TEAM6_API UGPBPEventDiscoverableComponent : public UGPDiscoverableComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGPBPEventDiscoverableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UPROPERTY(BlueprintAssignable)
	FGPDiscoveryDelegate OnDiscovered;
	
	virtual void Discover(AGPPlayerPawn* Discoverer) override;
};
