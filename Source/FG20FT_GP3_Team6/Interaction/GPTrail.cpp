// Fill out your copyright notice in the Description page of Project Settings.


#include "GPTrail.h"

#include "Components/SphereComponent.h"
#include "FG20FT_GP3_Team6/GPGameModeBase.h"
#include "FG20FT_GP3_Team6/AI/Manager/GPCreatureSpawnParameters.h"
#include "FG20FT_GP3_Team6/Player/GPPlayerPawn.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
AGPTrail::AGPTrail()
{
	PrimaryActorTick.bCanEverTick = true;	

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;
	
	CreatureSpawnTrigger =  CreateDefaultSubobject<USphereComponent>(TEXT("CreatureSpawnTrigger"));
	CreatureSpawnTrigger->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGPTrail::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);	
}

// Called every frame
void AGPTrail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ActiveTimer += DeltaTime;
	if (ActiveTimer >= SecondsToStayActive)
	{
		Deactivate();
		Cast<AGPGameModeBase>(UGameplayStatics::GetGameMode(this))->EnableClues();
	}

}

void AGPTrail::Activate()
{
	//register overlap	
	CreatureSpawnTrigger->OnComponentBeginOverlap.AddDynamic(this, &AGPTrail::SpawnCreature);
	TrailBecomeActive();

	//timer tick on
	SetActorTickEnabled(true);
	ActiveTimer = 0;
}

void AGPTrail::Deactivate()
{
	//timer tick off
	SetActorTickEnabled(false);
	//deregister overlap
	CreatureSpawnTrigger->OnComponentBeginOverlap.RemoveDynamic(this, &AGPTrail::SpawnCreature);
	TrailBecomeInactive();
}

void AGPTrail::SpawnCreature(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{	
	AGPPlayerPawn* PlayerPawn = Cast<AGPPlayerPawn>(OtherActor);
	if (PlayerPawn == nullptr)
	{
		return;
	}
	
	FGPCreatureSpawnParameters SpawnParams;
	SpawnParams.ActivitySpot = Activity;
	SpawnParams.SpawnLocation = CreatureSpawnTrigger->GetComponentLocation();
	AGPGameModeBase* GameMode = Cast<AGPGameModeBase>(UGameplayStatics::GetGameMode(this));
	GameMode->CreatureManager.SpawnCreatureForActivity(SpawnParams);

	Deactivate();
}

