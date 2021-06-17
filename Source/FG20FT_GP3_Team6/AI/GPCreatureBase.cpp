#include "GPCreatureBase.h"

#include "FG20FT_GP3_Team6/AI/Components/GPNavMovementComponent.h"
#include "FG20FT_GP3_Team6/GPGameModeBase.h"
#include "FG20FT_GP3_Team6/AI/Components/GPVisionSenseComponent.h"
#include "FG20FT_GP3_Team6/AI/Components/GPSonarSenseComponent.h"
#include "FG20FT_GP3_Team6/GPGameInstance.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Kismet/KismetMathLibrary.h"


AGPCreatureBase::AGPCreatureBase() : Super()
{
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = Capsule;
	Capsule->InitCapsuleSize(50.0f, 100.0f);
	Capsule->bDynamicObstacle = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	Mesh->SetupAttachment(RootComponent);

	SonarSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SonarSphereMesh"));
	SonarSphere->SetupAttachment(RootComponent);
	
	SonarSphere->SetAbsolute(true, true, true);
	SonarSphere->SetVisibility(false);	
	SonarSphere->SetCollisionProfileName(TEXT("NoCollision"));

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);

	PhasingAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Phasing Audio Component"));
	PhasingAudioComponent->SetupAttachment(RootComponent);

	NavMovementComponent = CreateDefaultSubobject<UGPNavMovementComponent>(TEXT("NavMovementComponent"));
	VisionSenseComponent = CreateDefaultSubobject<UGPVisionSenseComponent>(TEXT("VisionSense"));
	SonarSenseComponent = CreateDefaultSubobject<UGPSonarSenseComponent>(TEXT("SonarSense"));
	DiscoverableComponent = CreateDefaultSubobject<UGPBPEventDiscoverableComponent>(TEXT("DiscoverableComponent"));

	DissolvePlane = CreateDefaultSubobject<USceneComponent>(TEXT("Dissolve Plane"));
	DissolvePlane->SetAbsolute(true, true, true);
	DissolvePlane->SetupAttachment(RootComponent);
}


void AGPCreatureBase::BeginPlay()
{
	Super::BeginPlay();
	DiscoverableComponent->bDiscoveryLocked = true;

	if (PhasingSound != nullptr)
	{
		PhasingAudioComponent->SetSound(PhasingSound);
		PhasingAudioComponent->Play();
	}

	UMaterialInterface* AntlerMaterial =  Mesh->GetMaterial(2);
	AntlerDynamicMaterial = UMaterialInstanceDynamic::Create(AntlerMaterial, this);
	AntlerDynamicMaterial->SetScalarParameterValue("EmissiveColorMultiplier", 0.8f);
	Mesh->SetMaterial(2,AntlerDynamicMaterial);
	
}

void AGPCreatureBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	AGPGameModeBase* GameMode = Cast<AGPGameModeBase>(UGameplayStatics::GetGameMode(this));
	GameMode->CreatureManager.CurrentCreature = nullptr;

	//Cast<AGPGameModeBase>(UGameplayStatics::GetGameMode(this))->EnableClues();
	GameMode->EnableClues();

	Super::EndPlay(EndPlayReason);
}

void AGPCreatureBase::Tick(float DeltaSeconds)
{	
	if (bDiscoveryLockTimerActive)
	{
		if (TimeUntilDiscoveryLock < 0.0f)
		{
			bDiscoveryLockTimerActive = false;
			DiscoverableComponent->bDiscoveryLocked = true;
		}
		else
		{
			TimeUntilDiscoveryLock -= DeltaSeconds;
		}
	}

	float DistanceToDissolvePlane = FMath::Abs(FVector::PointPlaneDist(GetActorLocation(),
		DissolvePlane->GetComponentLocation(), DissolvePlane->GetForwardVector()));	
	PhasingAudioComponent->SetVolumeMultiplier(1 - FMath::SmoothStep(0, PhasingNoiseFadeInDistance,
                                                                        DistanceToDissolvePlane));	
}

void AGPCreatureBase::SetActivitySpot(AGPActivitySpotBase* ActivitySpot)
{
	CurrentActivitySpot = ActivitySpot;
}

void AGPCreatureBase::BecomeDiscovered()
{
	UGPGameInstance* Instance = Cast<UGPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	Instance->UpdateActivityEntry(CurrentActivitySpot->GetClass());

	AGPGameModeBase* GameMode = Cast<AGPGameModeBase>(UGameplayStatics::GetGameMode(this));
	GameMode->CompletedActivities.Add(CurrentActivitySpot->GetClass());
}

void AGPCreatureBase::SetDiscoverability(bool bDiscoverability)
{
	DiscoverableComponent->bDiscoveryLocked = !bDiscoverability;
}

void AGPCreatureBase::LockDiscoveryDelayed(float DelaySeconds)
{
	TimeUntilDiscoveryLock = DelaySeconds;
	bDiscoveryLockTimerActive = true;
}

void AGPCreatureBase::SetDissolvePlane(FVector PlanePosition, FVector PlaneNormal)
{
	FRotator Rotation = FRotationMatrix::MakeFromX(PlaneNormal).Rotator();

	DissolvePlane->SetWorldLocation(PlanePosition);
	DissolvePlane->SetWorldRotation(Rotation);

	SetDissolvePlaneValues(PlanePosition, DissolvePlane->GetForwardVector());
}

FVector AGPCreatureBase::SelectExitPointFarthestFromPlayer()
{
	FVector ExitPoint;
	if (CurrentActivitySpot == nullptr)
	{
		ExitPoint = GetActorLocation() + GetActorForwardVector() * (1000.0f + PhasePlaneSpawnOffset);
	}
	else
	{
		ExitPoint = CurrentActivitySpot->GetExitPointLocationFarthestFromPlayer();
	}
	
	FVector DirectionFromExitToCreature = (GetActorLocation() - ExitPoint);
	DirectionFromExitToCreature.Z = 0;
	DirectionFromExitToCreature.Normalize();	
	const FVector PlanePos = ExitPoint + (DirectionFromExitToCreature * PhasePlaneSpawnOffset);
	SetDissolvePlane(PlanePos, -DirectionFromExitToCreature);

	
	
	return ExitPoint;
}
