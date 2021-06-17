// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/DecalComponent.h"
#include "FG20FT_GP3_Team6/AI/ActivitySpots/GPActivitySpotBase.h"

#include "GPJournalDecal.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FG20FT_GP3_TEAM6_API UGPJournalDecal : public UDecalComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGPJournalDecal();
	// Called when the game starts
	virtual void BeginPlay() override;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	                           
	void MakeEntryVisible();

	UPROPERTY(EditDefaultsOnly)
	bool IsActivity = false;
	
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "IsActivity == true", EditConditionHides))
	TSubclassOf<AGPActivitySpotBase> ActivityType;
	
	bool bShouldBeVisible = false;
	bool bCurrentlyFading = true;
	float CurrentOpacity = 0.0f;
	float FadeSpeed = 1.0f;

	UPROPERTY(Transient)
	UMaterialInstanceDynamic* DecalDynamicMaterial;
};
