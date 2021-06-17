#include "GPJournalWriteOnInteract.h"
#include "FG20FT_GP3_Team6/Player/GPInteractionComponent.h"
#include "FG20FT_GP3_Team6/Player/GPPlayerPawn.h"

#define LOG(string) UE_LOG(LogTemp, Log, TEXT(string))
void UGPJournalWriteOnInteract::Interact(UGPInteractionComponent* InteractionComponent)
{
	AGPPlayerPawn* Player = Cast<AGPPlayerPawn>(InteractionComponent->GetOwner()); 

	
}
