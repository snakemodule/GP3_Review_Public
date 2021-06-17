// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GPBPEventOnInteract.h"
#include "Components/ActorComponent.h"
#include "GPToolPickup.generated.h"

UENUM(BlueprintType)
enum class EToolPickupType: uint8
{
	SPYGLASS,
	JOURNAL,
	MAP
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FG20FT_GP3_TEAM6_API UGPToolPickup : public UGPBPEventOnInteract
{
	GENERATED_BODY()

public:
	virtual void Interact(UGPInteractionComponent* InteractionComponent) override;
	
	UPROPERTY(EditDefaultsOnly)
	EToolPickupType ToolPickup;
};
