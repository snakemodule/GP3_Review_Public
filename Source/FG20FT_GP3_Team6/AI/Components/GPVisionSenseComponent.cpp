#include "GPVisionSenseComponent.h"


#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SenseSettings/GPVisionSenseSettings.h"

UGPVisionSenseComponent::UGPVisionSenseComponent() : Super()
{
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UGPVisionSenseComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (SenseSettings == nullptr)
	{
		return;
	}

	// TODO change this to use socket on skeletal mesh
	const FVector Origin = GetOwner()->GetActorLocation();
	const FVector Direction = GetOwner()->GetActorForwardVector();

	if (bDebugDrawVision)
	{
		const FVector Right = Direction.RotateAngleAxis(SenseSettings->VisionHalfAngle, FVector::UpVector);
		const FVector Left = Direction.RotateAngleAxis(-SenseSettings->VisionHalfAngle, FVector::UpVector);
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Origin, Origin + Right * SenseSettings->MaxDistance, FLinearColor::Red);
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Origin, Origin + Left * SenseSettings->MaxDistance, FLinearColor::Green);
		//UKismetSystemLibrary::DrawDebugConeInDegrees(GetWorld(), Origin, Direction, SenseSettings->MaxDistance, SenseSettings->VisionHalfAngle, SenseSettings->VisionHalfAngle);
	}
	
	if (IsPlayerVisible(Origin, Direction))
	{
		if (!bCanSeePlayer)
		{
			bCanSeePlayer = true;
			FGPVisionSenseResults Results;
			Results.SpottedActor = UGameplayStatics::GetPlayerPawn(this, 0);
			OnVisionTargetSensed.Broadcast(Results);
		}
	}
	else if (bCanSeePlayer)
	{
		bCanSeePlayer = false;
		FGPVisionSenseResults Results;
		Results.SpottedActor = nullptr;
		OnVisionTargetLost.Broadcast(Results);
	}

	
}

bool UGPVisionSenseComponent::IsPlayerVisible(const FVector& VisionOrigin, const FVector& VisionDirection)
{
	if (SenseSettings == nullptr)
	{
		return false;
	}
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	if (PlayerPawn == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Vision sense component got nullptr when getting player pawn"));
		return false;
	}

	const FVector PlayerLocation = PlayerPawn->GetActorLocation();
	const float MaxDistanceSq = FMath::Square(SenseSettings->MaxDistance);

	if (FVector::DistSquared(VisionOrigin, PlayerLocation) > MaxDistanceSq)
	{
		return false;
	}

	const FVector DirectionToTarget = (PlayerLocation - VisionOrigin).GetSafeNormal();
	const float Dot = FVector::DotProduct(VisionDirection, DirectionToTarget);
	const float CosOfVisionHalfAngle = FMath::Cos(FMath::DegreesToRadians(SenseSettings->VisionHalfAngle));

	if (CosOfVisionHalfAngle > Dot)
	{
		return false;
	}

	UWorld* World = GetWorld();
	FHitResult TraceHit;
	FCollisionQueryParams QueryParams;
	QueryParams.TraceTag = TEXT("LineTraceSingle");
	QueryParams.AddIgnoredActor(GetOwner());
	QueryParams.AddIgnoredActor(PlayerPawn);
	const bool bHit = World->LineTraceSingleByChannel(TraceHit, VisionOrigin, PlayerLocation, ECollisionChannel::ECC_Visibility, QueryParams);
	
	return TraceHit.GetActor() == nullptr;
}
