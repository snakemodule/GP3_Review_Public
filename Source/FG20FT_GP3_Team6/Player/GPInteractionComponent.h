#pragma once
#include "Components/ActorComponent.h"
#include "GPInteractionComponent.generated.h"

class UGPInteractable;

UCLASS(BlueprintType, meta = (BlueprintSpawnableComponent))
class UGPInteractionComponent : public UActorComponent 
{
	GENERATED_BODY()

public:
	UGPInteractionComponent();
	void Interact();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	bool bEnabled = true;
private:
	UPROPERTY(EditDefaultsOnly)
	float InteractRange = 200.f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> InteractWidget;
	
	UPROPERTY(Transient)
	UUserWidget* WidgetInstance;

	UPROPERTY(Transient)
	AActor* DetectedInteractableActor = nullptr;
};