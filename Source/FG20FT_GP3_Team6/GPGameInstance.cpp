#include "GPGameInstance.h"

#include "Player/GPPlayerPawn.h"
#include "Components/AudioComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "FG20FT_GP3_Team6/Player/Tools/Journal/GPJournalDecal.h"



void UGPGameInstance::UpdateActivityEntry(TSubclassOf<AGPActivitySpotBase> ActivityType)
{
	AGPPlayerPawn* Player = Cast<AGPPlayerPawn>(UGameplayStatics::GetPlayerPawn(this, 0));

	if(ActivityEntryMap.Contains(ActivityType) && !ActivityEntryMap[ActivityType]->IsVisible())
	{
		ActivityEntryMap[ActivityType]->MakeEntryVisible();
		Player->OnJournalEntryDiscovered();
		PlayDiscoverySound();
	}
	
}

void UGPGameInstance::UpdateNamedEntry(FString EntryName)
{
	AGPPlayerPawn* Player = Cast<AGPPlayerPawn>(UGameplayStatics::GetPlayerPawn(this, 0));

	if(NamedEntryMap.Contains(EntryName) && !NamedEntryMap[EntryName]->IsVisible())
	{
		NamedEntryMap[EntryName]->MakeEntryVisible();
		Player->OnJournalEntryDiscovered();
		PlayDiscoverySound();
	}
}


void UGPGameInstance::UpdateFragments(FString FragmentName, bool bRunestoneFragment)
{
	AGPPlayerPawn* Player = Cast<AGPPlayerPawn>(UGameplayStatics::GetPlayerPawn(this, 0));

	if(FragmentStatesMap.Contains(FragmentName) && !FragmentStatesMap[FragmentName]->IsVisible())
	{
		FragmentStatesMap[FragmentName]->SetVisibility(true);
		Player->OnMapFragmentDiscovered(bRunestoneFragment);
		PlayDiscoverySound();
	}
	
}

//TODO Cleanup this, no longer needed?
void UGPGameInstance::PlayDiscoverySound()
{
	AGPPlayerPawn* Player = Cast<AGPPlayerPawn>(UGameplayStatics::GetPlayerPawn(this, 0));

	if(Player->SoundCue != nullptr)
	{
		
		Player->AudioComponent->SetSound(Player->SoundCue);
		Player->AudioComponent->Play();
		
	}
}
