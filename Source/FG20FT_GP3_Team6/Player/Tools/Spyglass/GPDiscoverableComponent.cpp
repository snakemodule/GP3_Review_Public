#include "GPDiscoverableComponent.h"

UGPDiscoverableComponent::UGPDiscoverableComponent()
{	
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UGPDiscoverableComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UGPDiscoverableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGPDiscoverableComponent::Discover(AGPPlayerPawn* Discoverer)
{
	UE_LOG(LogTemp, Log, TEXT("Discovery Response"))
}

