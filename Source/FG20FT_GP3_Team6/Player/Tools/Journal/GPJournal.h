#pragma once

#include "FG20FT_GP3_Team6/Player/Tools/GPToolBase.h"
#include "FG20FT_GP3_Team6/GPGameModeBase.h"
#include "GPJournal.generated.h"
class UBoxComponent;

UCLASS()
class AGPJournal : public AGPToolBase
{
	GENERATED_BODY()
public:
	AGPJournal();

	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly)
	UMaterial* EmptyPageMaterial;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* JournalBodyMesh;

	UPROPERTY(Transient)
	TArray<UActorComponent*> Pages;
	UPROPERTY(Transient)
	UChildActorComponent* CurrentPage;
	UPROPERTY(Transient)
	AGPGameModeBase* GPGameMode;
	
	int PageIndex;
	void OpenJournal();
	void NextPage();
	void PreviousPage();
	
	virtual void ResetTool() override;
	virtual void InitToolBindings() override;
	virtual void ResetToolBindings() override;


	virtual void Tick(float DeltaTime) override;

	virtual void StartSwap(AGPToolBase* Tool, void (AGPPlayerPawn::*OnSwapEvent)()) override;

	virtual void Equip() override;
};