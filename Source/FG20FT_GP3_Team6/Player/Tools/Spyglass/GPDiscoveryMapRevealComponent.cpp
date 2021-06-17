// Fill out your copyright notice in the Description page of Project Settings.


#include "GPDiscoveryMapRevealComponent.h"

#include "FG20FT_GP3_Team6/GPGameInstance.h"
#include "FG20FT_GP3_Team6/GPGameModeBase.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UGPDiscoveryMapRevealComponent::UGPDiscoveryMapRevealComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGPDiscoveryMapRevealComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGPDiscoveryMapRevealComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGPDiscoveryMapRevealComponent::Discover(AGPPlayerPawn* Discoverer)
{
	UGPGameInstance* Instance = Cast<UGPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	Instance->UpdateFragments(MapFragmentName,true);
	
	Instance->UpdateNamedEntry(WaystoneJournalEntryName);
	
	Super::Discover(Discoverer);
}

