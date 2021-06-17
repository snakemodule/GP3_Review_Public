#pragma once
#include "Components/ActorComponent.h"
#include "GPInteractable.generated.h"

UCLASS(Abstract, BlueprintType, Blueprintable)
class UGPInteractable : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnabled = true;
	
	virtual void Interact(class UGPInteractionComponent* InteractionComponent) PURE_VIRTUAL(UGPInteractable);
};