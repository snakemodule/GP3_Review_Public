#include "FG20FT_GP3_Team6/Player/Tools/GPToolBase.h"

#include "FG20FT_GP3_Team6/Player/GPPlayerPawn.h"


void AGPToolBase::StartSwap(AGPToolBase* Tool, void (AGPPlayerPawn::* OnSwapEvent)())
{
	SwappingTo = Tool;
	AGPPlayerPawn* Player = Cast<AGPPlayerPawn>(GetParentActor()); 
	(Player->*OnSwapEvent)();
}

inline void AGPToolBase::FinalizeSwap(AGPToolBase* Tool)
{
	ToolActive(this, false);
	this->ResetTool();
	this->ResetToolBindings();

	if (Tool != nullptr)
	{
		ToolActive(Tool, true);
		Tool->InitToolBindings();
		Tool->Equip();		
	}
	Cast<AGPPlayerPawn>(GetParentActor())->CurrentTool = Tool;
	
}


inline void AGPToolBase::ToolActive(AActor* ToolActor, bool Active)
{
	ToolActor->SetActorTickEnabled(Active);
	ToolActor->SetActorHiddenInGame(!Active);
}

inline void AGPToolBase::InterpolateTransform(float DeltaTime, FVector FromPos, FVector ToPos,
    FQuat FromRot, FQuat ToRot,	float Duration)
{
	if (bReverse)
		AnimTimer -= DeltaTime;
	else
		AnimTimer += DeltaTime;

	AnimTimer = FMath::Clamp(AnimTimer, 0.f, Duration);
	float Alpha = AnimTimer / Duration;
	SetActorRelativeLocation(FMath::Lerp(FromPos, ToPos, Alpha));
	SetActorRelativeRotation(FQuat::Slerp(FromRot, ToRot, Alpha));	
}

void AGPToolBase::InterpolatePosition(float DeltaTime, FVector FromPos, FVector ToPos, float Duration)
{
	if (bReverse)
		AnimTimer -= DeltaTime;
	else
		AnimTimer += DeltaTime;

	AnimTimer = FMath::Clamp(AnimTimer, 0.f, Duration);
	float Alpha = AnimTimer / Duration;
	SetActorRelativeLocation(FMath::Lerp(FromPos, ToPos, Alpha));
}
