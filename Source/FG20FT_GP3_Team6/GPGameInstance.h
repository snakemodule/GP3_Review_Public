#pragma once

#include "AI/ActivitySpots/GPActivitySpotBase.h"
#include "Engine/GameInstance.h"
#include "GPGameInstance.generated.h"

class UGPJournalDecal;

UCLASS()
class UGPGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<AGPActivitySpotBase>, UGPJournalDecal*> ActivityEntryMap;
	UPROPERTY(EditDefaultsOnly)
	TMap<FString, UGPJournalDecal*> NamedEntryMap;
	
	TMap<FString, UDecalComponent*> FragmentStatesMap;


	void UpdateFragments(FString FragmentName, bool bRunestoneFragment);
	UFUNCTION(BlueprintCallable)
	void UpdateActivityEntry(TSubclassOf<AGPActivitySpotBase> ActivityType);
	
	UFUNCTION(BlueprintCallable)
    void UpdateNamedEntry(FString EntryName);
	
	void PlayDiscoverySound();

};