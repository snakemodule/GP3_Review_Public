#include "GPInteractionComponent.h"


#include "Algo/ForEach.h"
#include "Blueprint/UserWidget.h"
#include "FG20FT_GP3_Team6/Interaction/GPInteractable.h"
#include "FG20FT_GP3_Team6/Player/GPPlayerPawn.h"

UGPInteractionComponent::UGPInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGPInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	WidgetInstance = CreateWidget(GetWorld(), InteractWidget, TEXT("InteractPopup"));
}

void UGPInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	if (!bEnabled)
		return;

	auto Player = Cast<AGPPlayerPawn>(GetOwner());

	if (Player->VisionLineTraceHits.Num() < 1)
	{
		DetectedInteractableActor = nullptr;
		if (WidgetInstance->IsInViewport())
			WidgetInstance->RemoveFromViewport();
		return;
	}

	for (int i = 0; i < Player->VisionLineTraceHits.Num(); i++)
	{
		if (Player->VisionLineTraceHits[i].Distance <= InteractRange
			&& Player->VisionLineTraceHits[i].Actor != nullptr)
		{			
			TArray<UActorComponent*> Components;
			Player->VisionLineTraceHits[i].Actor->GetComponents(UGPInteractable::StaticClass(), Components);
			for (auto Component : Components)
			{
				UGPInteractable* Interactable = Cast<UGPInteractable>(Component);
				if (Interactable->bEnabled)
				{
					DetectedInteractableActor = Interactable->GetOwner();
					if (!WidgetInstance->IsInViewport())
						WidgetInstance->AddToViewport();
				}
			}			
		}
		else
		{
			DetectedInteractableActor = nullptr;
			if (WidgetInstance->IsInViewport())
				WidgetInstance->RemoveFromViewport();
			return;
		}
	}
}


void UGPInteractionComponent::Interact()
{
	if (!bEnabled)
		return;
	if (DetectedInteractableActor != nullptr)
	{
		TArray<UActorComponent*> Components;
		DetectedInteractableActor->GetComponents(UGPInteractable::StaticClass(), Components);

		for (auto Component : Components)
		{
			UGPInteractable* Interactable = Cast<UGPInteractable>(Component);
			if (Interactable->bEnabled)
			{
				Interactable->Interact(this);
				if(WidgetInstance->IsInViewport())
					WidgetInstance->RemoveFromViewport();
			}
		}		
	}	
}
