#pragma once

#include "GPInteractable.h"
#include "GPJournalWriteOnInteract.generated.h"
class GPJournal;

UCLASS(BlueprintType, meta = (BlueprintSpawnableComponent))
class UGPJournalWriteOnInteract : public UGPInteractable
{
	GENERATED_BODY()
public:
	void Interact(UGPInteractionComponent* InteractionComponent) override;

	
};