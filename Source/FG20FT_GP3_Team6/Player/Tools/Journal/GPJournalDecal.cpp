// Fill out your copyright notice in the Description page of Project Settings.


#include "GPJournalDecal.h"


// Sets default values for this component's properties
UGPJournalDecal::UGPJournalDecal()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGPJournalDecal::BeginPlay()
{
	Super::BeginPlay();
	
	DecalDynamicMaterial = CreateDynamicMaterialInstance();
	DecalDynamicMaterial->SetScalarParameterValue("Opacity", CurrentOpacity);
}


// Called every frame
void UGPJournalDecal::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bShouldBeVisible && !bCurrentlyFading)
	{
		CurrentOpacity += DeltaTime * FadeSpeed;
		if (CurrentOpacity < 1.0f)
		{
			DecalDynamicMaterial->SetScalarParameterValue("Opacity", CurrentOpacity);
		}
		else
		{
			DecalDynamicMaterial->SetScalarParameterValue("Opacity", 1.0f);
			bCurrentlyFading = true;
		}
	}
}

void UGPJournalDecal::MakeEntryVisible()
{
	SetVisibility(true);
	bCurrentlyFading = !(CurrentOpacity < 1.0f);
}

