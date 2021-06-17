// Fill out your copyright notice in the Description page of Project Settings.


#include "GPBPEventOnInteract.h"

void UGPBPEventOnInteract::Interact(UGPInteractionComponent* InteractionComponent)
{
	OnTargetInteracted.Broadcast();
}
