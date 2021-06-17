// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GPBPEventOnInteract.h"
#include "Engine/SpotLight.h"

#include "GPClueInteractable.generated.h"

class AGPTrail;

UCLASS(meta = (BlueprintSpawnableComponent))
class FG20FT_GP3_TEAM6_API UGPClueInteractable : public UGPBPEventOnInteract
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UGPClueInteractable();

	virtual void BeginPlay() override;

	virtual void Interact(UGPInteractionComponent* InteractionComponent) override;

	void EnableClue();
	void DisableClue();
	
	UPROPERTY(EditInstanceOnly)
	AGPTrail* Trail = nullptr;

	UPROPERTY(EditInstanceOnly)
	FString MapFragmentName;

	/* This is the name of the journal decal that will be revealed on the first interaction with this clue, intended for a picture of the clue */
	UPROPERTY(EditInstanceOnly) 
	FString JournalCluePictureDecalName = "";

	/* This is the name of the journal decal that will be revealed on the first clue interaction */
	UPROPERTY(EditDefaultsOnly) 
	FString TrailJournalEntryName = "Footprint";
	
	UParticleSystemComponent* Particles;
	USpotLightComponent* SpotLight;
};
