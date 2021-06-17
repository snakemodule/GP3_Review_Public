#include "GPPrintOnInteract.h"
#include "Components/AudioComponent.h"
#include "Components/DecalComponent.h"
#include "FG20FT_GP3_Team6/GPGameInstance.h"
#include "FG20FT_GP3_Team6/GPGameModeBase.h"
#include "FG20FT_GP3_Team6/Player/GPInteractionComponent.h"
#include "FG20FT_GP3_Team6/Player/GPPlayerPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#define LOG(string) UE_LOG(LogTemp, Log, TEXT(string))

UGPPrintOnInteract::UGPPrintOnInteract()
{
	// AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	// AudioComponent->bAutoActivate = false;
	
}


void UGPPrintOnInteract::Interact(class UGPInteractionComponent* InteractionComponent)
{
	UGPGameInstance* MyInstance = Cast<UGPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	AGPPlayerPawn* Player = Cast<AGPPlayerPawn>(InteractionComponent->GetOwner());


	MyInstance->UpdateFragments(FragmentName,true);
	MyInstance->UpdateActivityEntry(ActivityType);

}