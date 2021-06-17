#pragma once

#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "GPCreatureSpawnerSettings.generated.h"

class AGPCreatureBase;
class UBehaviorTree;

UCLASS()
class UGPCreatureSpawnerSettings : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AGPCreatureBase> CreatureClass; // TODO give this a "safe" default value

	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* CreatureBehaviorTreeBase;

	UPROPERTY(EditAnywhere)
	FGameplayTag ActivityBehaviorNodeTag;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "1"))
	int MaxIncorrectActivities = 1;
	
};
