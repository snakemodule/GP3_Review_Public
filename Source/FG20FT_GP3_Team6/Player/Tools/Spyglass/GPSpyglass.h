// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "GPDiscoverableComponent.h"
#include "../GPToolBase.h"
#include "GPSpyglass.generated.h"

class UGPDiscoverableComponent;

UCLASS()
class FG20FT_GP3_TEAM6_API AGPSpyglass : public AGPToolBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGPSpyglass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Press();
	void HandleZoom(float Value);
	virtual void Release();
	
	virtual void InitToolBindings() override;
	virtual void ResetToolBindings() override;

protected:
	virtual void Equip() override;
	virtual void StartSwap(AGPToolBase* Tool, void (AGPPlayerPawn::*OnSwapEvent)()) override;
	
private:	
	bool bLooking = false;

	UPROPERTY(EditDefaultsOnly)
	float ZoomSpeed = 8;
	UPROPERTY(EditDefaultsOnly)
	float MaxZoomFOV = 60;
	UPROPERTY(EditDefaultsOnly)
	float MinZoomFOV = 20;
	UPROPERTY(EditDefaultsOnly)
	float ZoomFOV = 20;	
	UPROPERTY(EditDefaultsOnly)
	float SpyglassDiscoveryRange = 10000;
	
	
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	FRotator RestingRotation;	
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	FRotator EqipStartRotation;
	UPROPERTY(EditDefaultsOnly, Category = Animation)

	FVector UsePosition;
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	FRotator UseRotation;
	
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	float UseTime;

};
