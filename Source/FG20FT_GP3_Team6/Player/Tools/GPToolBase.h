// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "GPToolBase.generated.h"

class AGPPlayerPawn;

UCLASS(Abstract)
class FG20FT_GP3_TEAM6_API AGPToolBase : public AActor
{
	GENERATED_BODY()

public:
	virtual void ResetTool() {}

	virtual void StartSwap(AGPToolBase* Tool, void (AGPPlayerPawn::*OnSwapEvent)());
	
	virtual void Equip() {};
	virtual void InitToolBindings()	{ };
	virtual void ResetToolBindings() { };

	virtual void FinalizeSwap(AGPToolBase* Tool);

	static void ToolActive(AActor* ToolActor, bool Active);

protected:
	UPROPERTY(Transient)
	AGPToolBase* SwappingTo = nullptr;

	float AnimTimer;
	bool bReverse = false;	

	void InterpolateTransform(float DeltaTime, FVector FromPos, FVector ToPos, FQuat FromRot, FQuat ToRot, float Duration);
	void InterpolatePosition(float DeltaTime, FVector FromPos, FVector ToPos, float Duration);
	
	enum State
	{
		Equipping,
        Using,
        Idle
    };
	State CurrentState = Idle;

	
	FVector InterpolatedPosition;

	/* How far up you have to look to move the map to RestingPosition. 1 = up, 0 = forward, -1 down */
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	float UpperThreshold = -0.1;
	/* How far down you have to look to move the map to LookingPosition. 1 = up, 0 = forward, -1 = down */
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	float LowerThreshold = -0.5;
	
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	FVector LookingPosition = FVector(30.f,0.f,20.f);
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	FVector RestingPosition = FVector(45.0f, 0.0f, -10.0f);
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	FVector EquipStartPosition = FVector(-45.f,0.f,-10.f);
	
	UPROPERTY(EditDefaultsOnly, Category = Animation)
    float EquipTime;
};



