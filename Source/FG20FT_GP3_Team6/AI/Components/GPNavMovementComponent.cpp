#include "GPNavMovementComponent.h"

#include "AI/Navigation/NavigationDataInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

UGPNavMovementComponent::UGPNavMovementComponent() : Super()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UGPNavMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!bHasRequestedVelocity)
		return;

	{
		const FVector CurrentForward = GetOwner()->GetActorForwardVector();
		const FVector DesiredForward = FVector(Velocity.X, Velocity.Y, 0.0f).GetSafeNormal();
		const FVector NewForward = FMath::VInterpNormalRotationTo(CurrentForward, DesiredForward, DeltaTime, VisualRotationSpeed);
		GetOwner()->SetActorRotation(UKismetMathLibrary::MakeRotFromX(NewForward));
	}
	
	GetOwner()->AddActorWorldOffset(Velocity * DeltaTime);
	
	
	FNavLocation NavFloorLocation;
	if (FindNavFloor(GetOwner()->GetActorLocation(), NavFloorLocation))
	{		
		float FloorHeight = NavFloorLocation.Location.Z;
		FVector CurrentLocation = GetOwner()->GetActorLocation();
		CurrentLocation.Z = FloorHeight + NavAgentProps.AgentHeight * 0.5f;
		GetOwner()->SetActorLocation(CurrentLocation);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("No navfloor found"));
	}
}

void UGPNavMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	Velocity = MoveVelocity.GetSafeNormal() * MovementSpeed;
	bHasRequestedVelocity = true;
}

void UGPNavMovementComponent::RequestPathMove(const FVector& MoveInput)
{
	// do nothing apparently?
}

bool UGPNavMovementComponent::CanStartPathFollowing() const
{
	return true;
}

bool UGPNavMovementComponent::CanStopPathFollowing() const
{
	return true;
}

void UGPNavMovementComponent::StopActiveMovement()
{
	Super::StopActiveMovement();

	Velocity = FVector::ZeroVector;
	bHasRequestedVelocity = false;
}

float UGPNavMovementComponent::GetPathFollowingBrakingDistance(float MaxSpeed) const
{
	return 1.0f;
}

const INavigationDataInterface* UGPNavMovementComponent::GetNavData() const
{
	const UWorld* World = GetWorld();
	if (World == nullptr || World->GetNavigationSystem() == nullptr)
	{
		return nullptr;
	}

	const INavigationDataInterface* NavData = FNavigationSystem::GetNavDataForActor(*GetOwner());

	return NavData;
}

bool UGPNavMovementComponent::FindNavFloor(const FVector& TestLocation, FNavLocation& NavFloorLocation) const
{
	const INavigationDataInterface* NavData = GetNavData();
	if (NavData == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("No nav data found"));
		return false;
	}

	const FNavAgentProperties& AgentProps = GetPawnOwner()->GetNavAgentPropertiesRef();
	const float SearchRadius = AgentProps.AgentRadius * 2.0f;
	const float SearchHeight = AgentProps.AgentHeight * AgentProps.NavWalkingSearchHeightScale;

	return NavData->ProjectPoint(TestLocation - FVector(0.0f, 0.0f, AgentProps.AgentHeight * 0.5f), NavFloorLocation, FVector(SearchRadius, SearchRadius, SearchHeight));
}

bool UGPNavMovementComponent::IsMoving() const
{
	return bHasRequestedVelocity;
}













