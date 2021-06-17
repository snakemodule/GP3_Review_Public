#include "GPCreatureController.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Components/SplineComponent.h"

AGPCreatureController::AGPCreatureController() : Super()
{
	PathSmoothingSpline = CreateDefaultSubobject<USplineComponent>(TEXT("PathSpline"));
	PathSmoothingSpline->SetupAttachment(RootComponent);
	PathSmoothingSpline->SetAbsolute(true, true, true);
}

void AGPCreatureController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const FNavPathSharedPtr NavPath = GetPathFollowingComponent()->GetPath();
	if(NavPath.IsValid())
	{
		FNavigationPath* PathPtr = NavPath.Get();
		
		const TArray<FNavPathPoint>& PathPoints = PathPtr->GetPathPoints();
		if (PathPoints.Num() > 1)
		{
			for (int i = 1; i < PathPoints.Num(); i++)
			{
				//UKismetSystemLibrary::DrawDebugLine(this, PathPoints[i - 1], PathPoints[i], FLinearColor::Green, 0.0f, 5.0f);
			}
		}
	}

	/*
	if (PathSmoothingSpline->GetNumberOfSplinePoints() > 1)
	{
		const float SplineLength = PathSmoothingSpline->GetSplineLength();
		const float StepDistance = 10.0f;
		float DistanceAlongSpline = 0.0f;
		FVector PrevLocation;
		FVector CurrentLocation = PathSmoothingSpline->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);

		while (DistanceAlongSpline < SplineLength)
		{
			PrevLocation = CurrentLocation;
			DistanceAlongSpline += StepDistance;
			CurrentLocation = PathSmoothingSpline->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
			UKismetSystemLibrary::DrawDebugLine(this, PrevLocation, CurrentLocation, FLinearColor::Red, 0.0f, 5.0f);
		}
	}
	*/
}


FAIRequestID AGPCreatureController::RequestMove(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr Path)
{
	// TODO make path smoother by adding more waypoints
	// TODO have the angle limit be based on values (movement speed, turn rate) fetched from the creature
	FNavigationPath* PathPtr = Path.Get();
	TArray<FNavPathPoint>& PathPoints = PathPtr->GetPathPoints();
	ANavigationData* NavData = PathPtr->GetNavigationDataUsed();

	if (PathPoints.Num() > 2)
	{
		TArray<FVector> SplinePointsToAdd;

		const float SubdivisionMaxSize = 350.0f; // TODO make this tweakable
		
		SplinePointsToAdd.Add(PathPoints[0].Location);
		
		for (int i = 1; i < PathPoints.Num(); i++)
		{
			const FVector PrevLocation = PathPoints[i - 1].Location;
			const FVector CurrentLocation = PathPoints[i].Location;
			const FVector LocationDiff = CurrentLocation - PrevLocation;
			const float DistBetweenLocations = LocationDiff.Size();

			const int SubdivisionCount = (DistBetweenLocations / SubdivisionMaxSize);

			if (SubdivisionCount > 0)
			{
				const FVector PrevToCurrDirection = LocationDiff / DistBetweenLocations;
				const float SubdivisionSize = DistBetweenLocations / (SubdivisionCount + 1);
				
				for (int CurrentSubdivisionIndex = 0; CurrentSubdivisionIndex < SubdivisionCount; CurrentSubdivisionIndex++)
				{
					const float CurrentSubdivisionDistance = SubdivisionSize * (CurrentSubdivisionIndex + 1);
					SplinePointsToAdd.Add(PrevLocation + PrevToCurrDirection * CurrentSubdivisionDistance);
				}
			}
			
			SplinePointsToAdd.Add(CurrentLocation);
		}
		
		PathSmoothingSpline->ClearSplinePoints(true);
		PathSmoothingSpline->SetSplinePoints(SplinePointsToAdd, ESplineCoordinateSpace::World);
		

		const float SplineLength = PathSmoothingSpline->GetSplineLength();
		const float DistancePerSample = 10.0f; // TODO make this tweakable?
		float DistanceAlongSpline = -DistancePerSample;

		PathPoints.Empty();
		
		while (DistanceAlongSpline < SplineLength)
		{
			DistanceAlongSpline += DistancePerSample;
			FVector CurrentLocation = PathSmoothingSpline->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);

			FNavPathPoint SubstepNavPathPoint;
			NavData->ProjectPoint(CurrentLocation, SubstepNavPathPoint, FVector(100.0f, 100.0f, 100.0f)); // arbitrary extents, change this?
			// TODO add some sort of check for failure
			PathPoints.Add(SubstepNavPathPoint);
		}
	}
	
	/*
	if(PathPoints.Num() > 2)
	{
		//this is wonky smoothing

		
		// most of these const floats will be replaced
		const float SubstepTime = .05f; // TODO make this tweakable
		const float RotSpeed = 20.0f; // TODO make this a tweakable value on the creature movement component // 100.0f worked fine in my test level
		const float MoveSpeed = 400.0f; // TODO get this value from the creature movement component

		const float SubstepAngle = FMath::Min(SubstepTime * RotSpeed, 180.0f); // remove this variable
		const float SubstepMovementDistance = SubstepTime * MoveSpeed;
		const float SubstepAngleCos = FMath::Cos(FMath::DegreesToRadians(SubstepAngle));
		
		int CurrentIndex = PathPoints.Num() - 2;
		
		UE_LOG(LogTemp, Log, TEXT("SubstepAngleCos: %f"), SubstepAngleCos);

		int counter = 0;
		while (CurrentIndex > 0)
		{
			counter++;
			const FNavPathPoint PreviousPathPoint = PathPoints[CurrentIndex + 1];
			const FNavPathPoint CurrentPathPoint = PathPoints[CurrentIndex];
			const FNavPathPoint NextPathPoint = PathPoints[CurrentIndex - 1];

			FVector PrevToCurr = CurrentPathPoint.Location - PreviousPathPoint.Location;
			PrevToCurr.Z = 0;
			PrevToCurr.Normalize();
			FVector CurrToNext = NextPathPoint.Location - CurrentPathPoint.Location;
			CurrToNext.Z = 0;
			CurrToNext.Normalize();
			
			const float DirectionDot = PrevToCurr | CurrToNext;

			UE_LOG(LogTemp, Log, TEXT("x: %f, y: %f, z: %f"), CurrentPathPoint.Location.X, CurrentPathPoint.Location.Y, CurrentPathPoint.Location.Z);
			UE_LOG(LogTemp, Log, TEXT("DirectionDot: %f"), DirectionDot);

			if (DirectionDot < SubstepAngleCos)
			{
				// TODO add "smoothed" path point
				const FVector SubstepDirection = FMath::VInterpNormalRotationTo(PrevToCurr, CurrToNext, SubstepTime, RotSpeed);
				const FVector SubstepLocation = SubstepDirection * SubstepMovementDistance + CurrentPathPoint.Location;
				
				// TODO project SubstepLocation onto navmesh
				FNavPathPoint SubstepNavPathPoint;
				NavData->ProjectPoint(SubstepLocation, SubstepNavPathPoint, FVector(SubstepMovementDistance, SubstepMovementDistance, SubstepMovementDistance)); // arbitrary extents, change this?

				
				for(int i = CurrentIndex - 2; i >= 0; i--)
				{
					FVector HitLocation = FVector::ZeroVector;
					const bool bRaycastObstructed = NavData->Raycast(SubstepNavPathPoint.Location, PathPoints[i].Location, HitLocation, PathPtr->GetQueryData().QueryFilter);
					if (!bRaycastObstructed)
					{
						// TODO remove all points between PathPoints[i] and PathPoints[CurrentIndex]
						const int RemoveRangeStart = i + 1;
						const int CountToRemove = CurrentIndex - RemoveRangeStart;
						PathPoints.RemoveAt(RemoveRangeStart, CountToRemove);
						CurrentIndex = RemoveRangeStart;
					}
				}
				
				
				//FNavPathPoint SubstepNavPathPoint(SubstepLocation);
				PathPoints.Insert(SubstepNavPathPoint, CurrentIndex);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("index: %d, counter: %d"), CurrentIndex, counter);
				counter = 0;
				--CurrentIndex;
			}
			
			
		}
	}
	*/
	//PathPtr->GetNavigationDataUsed()->DrawDebugPath(PathPtr);
	return Super::RequestMove(MoveRequest, Path);
}

