// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GPToolBase.h"
#include "GPDummyTool.generated.h"

UCLASS()
class FG20FT_GP3_TEAM6_API AGPDummyTool : public AGPToolBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGPDummyTool();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
