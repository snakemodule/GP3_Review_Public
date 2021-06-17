// Fill out your copyright notice in the Description page of Project Settings.

#define LOG(string) UE_LOG(LogTemp, Log, TEXT(string))

#include "GPSpyglass.h"

#include "Camera/CameraComponent.h"
#include "FG20FT_GP3_Team6/Player/GPPlayerPawn.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AGPSpyglass::AGPSpyglass()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGPSpyglass::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGPSpyglass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bLooking)
		return;
	
	if (CurrentState == Equipping)
	{
		InterpolateTransform(DeltaTime, EquipStartPosition, RestingPosition,
		                FQuat(EqipStartRotation), FQuat(RestingRotation), EquipTime);
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
	else if (CurrentState == Using)
	{
		InterpolateTransform(DeltaTime, RestingPosition, UsePosition,
		                FQuat(RestingRotation), FQuat(UseRotation), UseTime);
		if (AnimTimer >= UseTime)
		{
			auto Player = Cast<AGPPlayerPawn>(GetParentActor());
			Player->SpyglassEnabled(true);
			Player->FirstPersonCameraComponent->SetFieldOfView(ZoomFOV);
			Player->DiscoveryRange = SpyglassDiscoveryRange;

			bLooking = true;
			SetActorHiddenInGame(bLooking);			
		}
		else if (AnimTimer <= 0)
		{
			CurrentState = Idle;
		}
	}
	else if (CurrentState == Idle)
	{
	}
}

void AGPSpyglass::Press()
{
	if (CurrentState == Idle)
	{
		AnimTimer = 0;
		bReverse = false;
		CurrentState = Using;
	}
}

void AGPSpyglass::HandleZoom(float Value)
{
	if (bLooking)
	{
		ZoomFOV -= Value * ZoomSpeed;
		ZoomFOV = FMath::Clamp(ZoomFOV, MinZoomFOV, MaxZoomFOV);
		
		auto Player = Cast<AGPPlayerPawn>(GetParentActor());
		Player->FirstPersonCameraComponent->SetFieldOfView(ZoomFOV);
	}
}

void AGPSpyglass::Release()
{
	if (bLooking)
	{
		bLooking = false;
		SetActorHiddenInGame(bLooking);

		auto Player = Cast<AGPPlayerPawn>(GetParentActor());
		Player->SpyglassEnabled(false);
		Player->FirstPersonCameraComponent->SetFieldOfView(Player->DesiredFOV);
		Player->DiscoveryRange = Player->DefaultDiscoveryRange;

		AnimTimer = UseTime;
		bReverse = true;
		CurrentState = Using;
	}
	else if (CurrentState == Using)
	{
		bReverse = true;
	}
}

void AGPSpyglass::InitToolBindings()
{
	auto PlayerInputComponent = UGameplayStatics::GetPlayerController(this, 0)->InputComponent;
	PlayerInputComponent->BindAction("M1", IE_Pressed, this, &AGPSpyglass::Press);
	PlayerInputComponent->BindAction("M1", IE_Released, this, &AGPSpyglass::Release);

	
	PlayerInputComponent->BindAxis("MouseWheelAxis", this, &AGPSpyglass::HandleZoom);
}

void AGPSpyglass::ResetToolBindings()
{
	auto PlayerInputComponent = UGameplayStatics::GetPlayerController(this, 0)->InputComponent;
	PlayerInputComponent->RemoveActionBinding("M1", IE_Pressed);
	PlayerInputComponent->RemoveActionBinding("M1", IE_Released);

	for (int i = 0; i < PlayerInputComponent->AxisBindings.Num(); i++)
	{
		if (PlayerInputComponent->AxisBindings[i].AxisName == "MouseWheelAxis")
		{
			PlayerInputComponent->AxisBindings.RemoveAt(i);
			break;
		}	
	}
}

void AGPSpyglass::Equip()
{
	if (CurrentState == Idle)
	{
		AnimTimer = 0;
		bReverse = false;
		CurrentState = State::Equipping;
	}
}

void AGPSpyglass::StartSwap(AGPToolBase* Tool, void (AGPPlayerPawn::*OnSwapEvent)())
{
	if (CurrentState == Idle)
	{
		AnimTimer = EquipTime;
		bReverse = true;
		CurrentState = State::Equipping;
		Super::StartSwap(Tool, OnSwapEvent);
	}
}
