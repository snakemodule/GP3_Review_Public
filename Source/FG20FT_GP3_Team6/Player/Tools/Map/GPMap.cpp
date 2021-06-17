#include "GPMap.h"

#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "FG20FT_GP3_Team6/GPGameInstance.h"
#include "FG20FT_GP3_Team6/GPGameModeBase.h"
#include "FG20FT_GP3_Team6/Player/GPPlayerPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AGPMap::AGPMap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<UStaticMeshComponent>("Root");
	Compass = CreateDefaultSubobject<UStaticMeshComponent>("Compass");
	CompassNeedle = CreateDefaultSubobject<UStaticMeshComponent>("Compass needle");
	CompassNeedle->SetupAttachment(Compass);
	RootComponent = Root;
	Compass->SetupAttachment(Root);
}

void AGPMap::BeginPlay()
{
	Super::BeginPlay();
	TArray<UActorComponent*> DecalComponents;
	GetComponents(UDecalComponent::StaticClass(), DecalComponents, true);
	UGPGameInstance* MyInstance = Cast<UGPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	for(int i = 0; i < DecalComponents.Num(); i++)
	{
		MyInstance->FragmentStatesMap.Add(DecalComponents[i]->GetName(), Cast<UDecalComponent>(DecalComponents[i]));
		Cast<UDecalComponent>(DecalComponents[i])->SetVisibility(false);
	}
	
	
}

void AGPMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AGPPlayerPawn* Player = Cast<AGPPlayerPawn>(GetParentActor());
	
	auto const Camera = Player->FirstPersonCameraComponent;
	auto CameraForward = Camera->GetForwardVector();
	auto Upness = FVector::DotProduct(FVector::UpVector, CameraForward);
	
	float t = UKismetMathLibrary::NormalizeToRange(Upness, LowerThreshold, UpperThreshold);
	t = FMath::Clamp(t,0.f,1.f);
		
	InterpolatedPosition = FMath::Lerp(LookingPosition,RestingPosition, t);
			
	if (CurrentState == Equipping)
    	{
    		InterpolatePosition(DeltaTime, EquipStartPosition, InterpolatedPosition, EquipTime);
    		if (AnimTimer >= EquipTime && bReverse == false)
    		{
    			CurrentState = Idle;			
    		}
    		else if(AnimTimer <= 0 && bReverse == true)
    		{			
    			CurrentState = Idle;
    			FinalizeSwap(SwappingTo);
    		}
    }
	else 
	{
		SetActorRelativeLocation(InterpolatedPosition);
	}
	
	RotateNeedleToNorth();
}

void AGPMap::StartSwap(AGPToolBase* Tool,void (AGPPlayerPawn::*OnSwapEvent)())
{
	if (CurrentState == Idle)
	{
		AnimTimer = EquipTime;
		bReverse = true;
		CurrentState = State::Equipping;
		Super::StartSwap(Tool, OnSwapEvent);				
	}
}

void AGPMap::Equip()
{
	if (CurrentState == Idle)
	{
		AnimTimer = 0;
		bReverse = false;
		CurrentState = State::Equipping;
		
	}
}

void AGPMap::RotateNeedleToNorth()
{
	FVector Direction = FVector::RightVector * -100000;
	FRotator Rotation = Direction.Rotation();
	FRotator TargetRotation = Rotation - CompassNeedle->GetForwardVector().Rotation();
	TargetRotation += FRotator(0, 14.f, 0);
	if (FMath::Abs(TargetRotation.Yaw) < 180) 
	{
		RotateNeedle(TargetRotation.Yaw);
	}
	else 
	{
		RotateNeedle(-TargetRotation.Yaw);
	}
}

void AGPMap::RotateNeedle(float RelativeSpeed)
{
	float RotationChange = RelativeSpeed * CompassNeedleSpeed * GetWorld()->DeltaTimeSeconds;
	float NewRotation = CompassNeedle->GetRelativeRotation().Yaw + RotationChange;
	CompassNeedle->SetRelativeRotation(FRotator(0, NewRotation, 0));
}
