#include "GPGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "Interaction/GPClueInteractable.h"
#include "Interaction/GPTrail.h"
#include "Particles/ParticleSystemComponent.h"

void AGPGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	CreatureManager.CreatureSpawnerSettings = CreatureSpawnerSettings;
}

void AGPGameModeBase::SpawnCreatureForActivity(const FGPCreatureSpawnParameters& SpawnParams)
{
	CreatureManager.SpawnCreatureForActivity(SpawnParams);
}

void AGPGameModeBase::DisableClues()
{
	for (auto Clue : AllClues)
	{
		Clue->DisableClue();
	}
}


void AGPGameModeBase::EnableClues()
{
	for (UGPClueInteractable* Clue : AllClues)
	{
		const AGPTrail* ClueTrail = Clue->Trail;
		const AGPActivitySpotBase* TrailActivity = ClueTrail->Activity;
		UClass* ActivityType = TrailActivity->GetClass();
		const bool bIsCompleted = CompletedActivities.Contains(ActivityType);
		if (!bIsCompleted)
		{
			Clue->EnableClue();
		}
	}
}




