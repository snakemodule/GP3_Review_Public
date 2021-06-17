#pragma once

#include "GPInteractable.h"
#include "FG20FT_GP3_Team6/AI/ActivitySpots/GPActivitySpotBase.h"

#include "GPPrintOnInteract.generated.h"
class USoundCue;
class AGPJournal;

UCLASS(BlueprintType, meta = (BlueprintSpawnableComponent))
class UGPPrintOnInteract : public UGPInteractable
{
	GENERATED_BODY()

protected:
	UGPPrintOnInteract();
	virtual void Interact(class UGPInteractionComponent* InteractionComponent) override;

	
	UPROPERTY(EditInstanceOnly)
	FString FragmentName;
	UPROPERTY(EditInstanceOnly)
	TSubclassOf<AGPActivitySpotBase> ActivityType;
	
};