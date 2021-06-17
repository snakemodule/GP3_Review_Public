// Fill out your copyright notice in the Description page of Project Settings.


#include "GPBPEventDiscoverableComponent.h"


// Sets default values for this component's properties
UGPBPEventDiscoverableComponent::UGPBPEventDiscoverableComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGPBPEventDiscoverableComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);	
}




void UGPBPEventDiscoverableComponent::Discover(AGPPlayerPawn* Discoverer)
{
	OnDiscovered.Broadcast(Discoverer);
}

