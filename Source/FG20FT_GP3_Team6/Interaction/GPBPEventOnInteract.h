// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GPInteractable.h"
#include "GPBPEventOnInteract.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGPInteractionDelegate);

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class FG20FT_GP3_TEAM6_API UGPBPEventOnInteract : public UGPInteractable
{
	GENERATED_BODY()
public:

    	
	virtual void Interact(UGPInteractionComponent* InteractionComponent) override;

	UPROPERTY(BlueprintAssignable)
	FGPInteractionDelegate OnTargetInteracted;
	
};
