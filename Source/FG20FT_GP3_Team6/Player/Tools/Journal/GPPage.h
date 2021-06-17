#pragma once

#include "GameFramework/Actor.h"
#include "GPPage.generated.h"

class UGPJournalDecal;

UCLASS()
class AGPPage : public AActor
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0"))
	float EntryFadeInDuration = 1.0f;

	UPROPERTY()
	TArray<UGPJournalDecal*> PageEntries;

	UFUNCTION()
	void MakePageVisible(bool bShouldBeVisible);
};