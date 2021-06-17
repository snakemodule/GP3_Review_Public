// Fill out your copyright notice in the Description page of Project Settings.


#include "GPToolPickup.h"
#include "FG20FT_GP3_Team6/Player/GPInteractionComponent.h"
#include "FG20FT_GP3_Team6/Player/GPPlayerPawn.h"

void UGPToolPickup::Interact(UGPInteractionComponent* InteractionComponent)
{
	auto Player = Cast<AGPPlayerPawn>(InteractionComponent->GetOwner());
	ensureMsgf(Player != nullptr, TEXT("The interacting component was not on the player"));

	switch (ToolPickup)
	{
	case EToolPickupType::MAP:
		Player->UnlockMap();
		break;
	case EToolPickupType::JOURNAL:
		Player->UnlockJournal();
		break;
	case EToolPickupType::SPYGLASS:
		Player->UnlockSpyglass();
		break;
	default:
		ensureMsgf(false, TEXT("Unhandled case, unkown tool pickup enum"));
	}
	bEnabled = false;
	Super::Interact(InteractionComponent);
}
