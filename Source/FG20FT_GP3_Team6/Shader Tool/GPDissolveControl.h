#pragma once
#include "FG20FT_GP3_Team6/Interaction/GPInteractable.h"
#include "GPDissolveControl.generated.h"

UCLASS(BlueprintType, meta = (BlueprintSpawnableComponent))
class UGPDissolveControl : public UGPInteractable
{
	GENERATED_BODY()
public:
	UGPDissolveControl();
	UPROPERTY(EditAnywhere)
    float OldValue;
    UPROPERTY(EditAnywhere)
    float OldMin;
   	UPROPERTY(EditAnywhere)
   	float OldMax;
   	UPROPERTY(EditAnywhere)
   	float NewMin;
	UPROPERTY(EditAnywhere)
   	float NewMax;
   	float NewValue;
	float t = 0.01f;
	float TAmount = 0.f;
	
	UMaterialInstanceDynamic* MaterialDynamic;
	void SetDissolve(float Amount);
	void Interact(UGPInteractionComponent* InteractionComponent) override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};