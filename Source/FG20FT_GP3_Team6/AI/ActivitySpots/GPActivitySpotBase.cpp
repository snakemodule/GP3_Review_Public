#include "GPActivitySpotBase.h"
#include "FG20FT_GP3_Team6/GPGameModeBase.h"
#include "Kismet/GameplayStatics.h"

FVector AGPActivitySpotBase::GetExitPointLocationFarthestFromPlayer() const
{
	const FTransform LocalToWorld = GetActorTransform();

	if (ExitPoints.Num() == 0)
	{
		return LocalToWorld.GetLocation();
	}
	
	const APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	const FVector PlayerLocation = Player->GetActorLocation();
	
	FVector FurthestExitLocation = LocalToWorld.TransformPosition(ExitPoints[0].Location);
	
	for (int i = 1; i < ExitPoints.Num(); i++)
	{
		const FVector CurrentExitLocation = LocalToWorld.TransformPosition(ExitPoints[i].Location);
		if (FVector::DistSquared(PlayerLocation, CurrentExitLocation) >
            FVector::DistSquared(PlayerLocation, FurthestExitLocation))
		{
			FurthestExitLocation = CurrentExitLocation;
		}
	}
	
	return FurthestExitLocation;
}

FVector AGPActivitySpotBase::GetActivityLocationWorldSpace() const
{
	return GetActorTransform().TransformPosition(ActivityLocation);
}

FVector AGPActivitySpotBase::GetActivityLookTargetWorldSpace() const
{
	return GetActorTransform().TransformPosition(ActivityLookTarget);
}

