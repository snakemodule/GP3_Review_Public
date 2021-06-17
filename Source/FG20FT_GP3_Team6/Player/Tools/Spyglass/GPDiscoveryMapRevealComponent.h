// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GPBPEventDiscoverableComponent.h"
#include "Components/ActorComponent.h"
#include "GPDiscoveryMapRevealComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FG20FT_GP3_TEAM6_API UGPDiscoveryMapRevealComponent : public UGPBPEventDiscoverableComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGPDiscoveryMapRevealComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	                     
	virtual void Discover(AGPPlayerPawn* Discoverer) override;

	UPROPERTY(EditInstanceOnly)
	FString MapFragmentName = "";

	/* Name of the journal entry decal that should be revealed the first time you interact with a waystone */
	UPROPERTY(EditDefaultsOnly)
	FString WaystoneJournalEntryName = "GlowingRunes";	                           
};
