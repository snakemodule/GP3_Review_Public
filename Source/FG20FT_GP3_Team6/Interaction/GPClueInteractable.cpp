// Fill out your copyright notice in the Description page of Project Settings.


#include "GPClueInteractable.h"
#include "FG20FT_GP3_Team6/Player/GPInteractionComponent.h"
#include "GPTrail.h"
#include "Components/SpotLightComponent.h"
#include "FG20FT_GP3_Team6/GPGameInstance.h"
#include "FG20FT_GP3_Team6/GPGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

UGPClueInteractable::UGPClueInteractable()
{
	
}

void UGPClueInteractable::BeginPlay()
{
	Super::BeginPlay();
	
	Particles = GetOwner()->FindComponentByClass<UParticleSystemComponent>();
	SpotLight = GetOwner()->FindComponentByClass<USpotLightComponent>();
	if (Trail == nullptr)
	{
		const FString MessageString = "Error: no trail assigned to: " + GetOwner()->GetName();
		GEngine->AddOnScreenDebugMessage((uint64)-1, 30.0f, FColor::Red, MessageString);
		DisableClue();
	}
	else if (Trail->Activity == nullptr)
	{
		const FString MessageString = "Error: no activity assigned to: " + Trail->GetName();
		GEngine->AddOnScreenDebugMessage((uint64)-1, 30.0f, FColor::Red, MessageString);
		DisableClue();
	}
	else
	{
		auto GameMode = Cast<AGPGameModeBase>(UGameplayStatics::GetGameMode(this));
		GameMode->AllClues.Add(this);
	}
}

void UGPClueInteractable::Interact(UGPInteractionComponent* InteractionComponent)
{
	Super::Interact(InteractionComponent);
	if (Trail!=nullptr)
	{
		Trail->Activate();		
	}

	UGPGameInstance* Instance = Cast<UGPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	Instance->UpdateFragments(MapFragmentName, false);

	Instance->UpdateNamedEntry(TrailJournalEntryName);
	Instance->UpdateNamedEntry(JournalCluePictureDecalName);
		
	Cast<AGPGameModeBase>(UGameplayStatics::GetGameMode(this))->DisableClues();
}

void UGPClueInteractable::EnableClue()
{
	bEnabled = true;
	Particles->Activate();
	Particles->SetVisibility(true);
	SpotLight->SetVisibility(false);
}

void UGPClueInteractable::DisableClue()
{
	bEnabled = false;
	Particles->Deactivate();
	Particles->SetVisibility(false);
	SpotLight->SetVisibility(false);
}

