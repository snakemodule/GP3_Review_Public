#include "GPPage.h"

#include "GPJournalDecal.h"
#include "Components/DecalComponent.h"
#include "FG20FT_GP3_Team6/GPGameInstance.h"
#include "Kismet/GameplayStatics.h"

void AGPPage::BeginPlay()
{
	Super::BeginPlay();
	TArray<UActorComponent*> DecalComponents;
	GetComponents(UGPJournalDecal::StaticClass(), DecalComponents, true);
	UGPGameInstance* MyInstance = Cast<UGPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	for(int i = 0; i < DecalComponents.Num(); i++)
	{
		UGPJournalDecal* Decal = Cast<UGPJournalDecal>(DecalComponents[i]);
		PageEntries.Add(Decal);
		Decal->FadeSpeed = 1.0f / EntryFadeInDuration;
		
		if(Decal->IsActivity)
		{
			MyInstance->ActivityEntryMap.Add(Decal->ActivityType, Decal);
		}
		else
		{
			MyInstance->NamedEntryMap.Add(Decal->GetName(), Decal);
		}
		
		Decal->SetVisibility(false);
	}
	
}

void AGPPage::MakePageVisible(bool bShouldBeVisible)
{
	RootComponent->SetVisibility(bShouldBeVisible);
	for (UGPJournalDecal* Entry : PageEntries)
	{
		Entry->bShouldBeVisible = bShouldBeVisible;
	}
}
