#pragma once

#include <string>

#include "AI/ActivitySpots/GPActivitySpotBase.h"

#include "GameFramework/GameModeBase.h"
#include "FG20FT_GP3_Team6/AI/Manager/GPCreatureSpawnParameters.h" // const ref as parameter requires this
#include "FG20FT_GP3_Team6/AI/Manager/GPCreatureManager.h"
#include "GPGameModeBase.generated.h"

//struct FGPCreatureSpawnParameters;
class UGPCreatureSpawnerSettings;

class UGPClueInteractable;

UCLASS()
class AGPGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void SpawnCreatureForActivity(const FGPCreatureSpawnParameters& SpawnParams);

	UPROPERTY(EditAnywhere, Category = CreatureSpawning)
	UGPCreatureSpawnerSettings* CreatureSpawnerSettings;
	
	GPCreatureManager CreatureManager;

	TArray<UGPClueInteractable*> AllClues;

	UPROPERTY(BlueprintReadOnly)
	TSet<TSubclassOf<AGPActivitySpotBase>> CompletedActivities;

	// marks all clues disabled
	void DisableClues();

	// marks all uncompleted clues as enabled.
	void EnableClues();
	
};