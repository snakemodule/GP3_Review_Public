// Fill out your copyright notice in the Description page of Project Settings.

#define LOG(string) UE_LOG(LogTemp, Log, TEXT(string))

#include "GPPlayerPawn.h"

#include "GPMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "Tools/Map/GPMap.h"
#include "FG20FT_GP3_Team6/Player/GPInteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Tools/Spyglass/GPDiscoveryMapRevealComponent.h"


AGPPlayerPawn::AGPPlayerPawn() : Super()
{
	PrimaryActorTick.bCanEverTick = true;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	InteractionComponent = CreateDefaultSubobject<UGPInteractionComponent>(TEXT("InteractionComponent"));

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioComponent->bAutoActivate = false;

	VoiceOverComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("VoiceOver Component"));
	VoiceOverComponent->bAutoActivate = false;
}


void AGPPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = Speed;
	GetCharacterMovement()->MaxAcceleration = Acceleration;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	DiscoveryRange = DefaultDiscoveryRange;

	TArray<UActorComponent*> ToolActorComponents;
	GetComponents(UChildActorComponent::StaticClass(), ToolActorComponents);

	for (auto ToolActorComponent : ToolActorComponents)
	{
		UChildActorComponent* Component = Cast<UChildActorComponent>(ToolActorComponent);
		auto Actor = Component->GetChildActor();

		if (Cast<AGPSpyglass>(Actor) != nullptr)
		{
			SpyglassActorComponent = Component;
			const auto SpyglassActor = Cast<AGPSpyglass>(Actor);
			ensure(SpyglassActor != nullptr);
			ToolActive(SpyglassActor, false);
		}
		else if (Cast<AGPJournal>(Actor) != nullptr)
		{
			JournalActorComponent = Component;
			const auto JournalActor = Cast<AGPJournal>(Actor);
			ensure(JournalActor != nullptr);
			ToolActive(JournalActor, false);
		}
		else if (Cast<AGPMap>(Actor) != nullptr)
		{
			MapActorComponent = Component;
			const auto MapActor = Cast<AGPMap>(Actor);
			ensure(MapActor != nullptr);
			ToolActive(MapActor, false);
		}
	}
	if (!bTutorialSequence)
	{
		Spyglass = Cast<AGPSpyglass>(SpyglassActorComponent->GetChildActor());
		Journal = Cast<AGPJournal>(JournalActorComponent->GetChildActor());
		Map = Cast<AGPMap>(MapActorComponent->GetChildActor());
	}
}


void AGPPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateMovementSpeed(DeltaTime);

	if (VoiceOverComponent->IsPlaying())
		InteractionComponent->bEnabled = false;
	else
		InteractionComponent->bEnabled = true;

	auto Origin = FirstPersonCameraComponent->GetComponentLocation();
	auto End = Origin + FirstPersonCameraComponent->GetForwardVector() * DiscoveryRange;
	GetWorld()->LineTraceMultiByChannel(VisionLineTraceHits, Origin, End, ECollisionChannel::ECC_Visibility);
	VisionLineTraceHits.Sort([](const FHitResult& LHS, const FHitResult& RHS)
	{
		return LHS.Distance > RHS.Distance;
	});

	UGPDiscoverableComponent* Discoverable = nullptr;
	UGPDiscoveryMapRevealComponent* MapReveal = nullptr;
	for (int i = 0; i < VisionLineTraceHits.Num(); ++i)
	{
		if (VisionLineTraceHits[i].Actor == nullptr)
			continue;

		UActorComponent* Component = VisionLineTraceHits[i].Actor->FindComponentByClass(
			UGPDiscoverableComponent::StaticClass());
		Discoverable = Cast<UGPDiscoverableComponent>(Component);

		if (Discoverable != nullptr && Discoverable != Watching && !Discoverable->bDiscoveryLocked)
		{
			MapReveal = Cast<UGPDiscoveryMapRevealComponent>(Discoverable);
			if (MapReveal)
			{
				auto DistanceToMapReveal = FVector::Distance(MapReveal->GetOwner()->GetActorLocation(), GetActorLocation());
				if (DistanceToMapReveal > DefaultDiscoveryRange)
					continue;
			}

			Watching = Discoverable;

			DiscoveryInProgress = 0;
			GraceTimer = 0;
			SetDiscoveryPercent(0);
			BeginDiscover();
			break;
		}
	}

	if (Discoverable == nullptr && Watching != nullptr)
	{
		if (GraceTimer > DiscoveryGracePeriod)
		{
			Watching = nullptr;
			EndDiscover();
		}
		else
		{
			if (GraceTimer <= 0)
				DiscoveryGracePeriodStart();
			GraceTimer += DeltaTime;
			DiscoveryInProgress -= DeltaTime;
			SetDiscoveryPercent(FMath::Clamp(DiscoveryInProgress / DiscoveryTime, 0.f, 1.f));
		}
	}
	else if (Watching != nullptr)
	{
		if (Watching->bDiscoveryLocked)
		{
			Watching = nullptr;
			EndDiscover();
		}
		else
		{
			DiscoveryInProgress += DeltaTime;
			if (GraceTimer > 0)
				DiscoveryGracePeriodReset();
			GraceTimer = 0;
			SetDiscoveryPercent(FMath::Clamp(DiscoveryInProgress / DiscoveryTime, 0.f, 1.f));
			if (DiscoveryInProgress >= DiscoveryTime)
			{
				Watching->Discover(this);
				Watching->bDiscoveryLocked = true;
				EndDiscover();
			}
		}
	}
}


void AGPPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	PlayerInputComponent->BindAxis("MoveForward", this, &AGPPlayerPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGPPlayerPawn::MoveRight);

	PlayerInputComponent->BindAxis("MouseHorizontal", this, &AGPPlayerPawn::MouseHorizontal);
	PlayerInputComponent->BindAxis("MouseVertical", this, &AGPPlayerPawn::MouseVertical);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AGPPlayerPawn::Interact);

	PlayerInputComponent->BindAction("Slot 1", IE_Pressed, this, &AGPPlayerPawn::EquipSlot1);
	PlayerInputComponent->BindAction("Slot 2", IE_Pressed, this, &AGPPlayerPawn::EquipSlot2);
	PlayerInputComponent->BindAction("Slot 3", IE_Pressed, this, &AGPPlayerPawn::EquipSlot3);
	PlayerInputComponent->BindAction("Slot 4", IE_Pressed, this, &AGPPlayerPawn::EquipSlot4);

	PlayerInputComponent->BindAction("Space", IE_Pressed, this, &AGPPlayerPawn::Jump);

	PlayerInputComponent->BindAction("LShift", IE_Pressed, this, &AGPPlayerPawn::SprintOn);
	PlayerInputComponent->BindAction("LShift", IE_Released, this, &AGPPlayerPawn::SprintOff);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AGPPlayerPawn::Duck);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AGPPlayerPawn::Stand);

	PlayerInputComponent->BindAction("SkipTutorial", IE_Released, this, &AGPPlayerPawn::SkipTutorial);
}

void AGPPlayerPawn::SkipTutorial()
{
	Spyglass = Cast<AGPSpyglass>(SpyglassActorComponent->GetChildActor());
	Journal = Cast<AGPJournal>(JournalActorComponent->GetChildActor());
	Map = Cast<AGPMap>(MapActorComponent->GetChildActor());
	bTutorialSequence = false;
}

void AGPPlayerPawn::Jump()
{
	if (!bSlowedDown)
		Super::Jump();
}

void AGPPlayerPawn::Duck()
{
	Crouch();
}

void AGPPlayerPawn::Stand()
{
	UnCrouch();
}

void AGPPlayerPawn::SprintOff()
{
	bSprinting = false;
	OnSprintEnd();
}

void AGPPlayerPawn::SprintOn()
{
	if (!bIsCrouched)
	{
		bSprinting = true;
		OnSprint();
	}
}

void AGPPlayerPawn::MoveForward(float Value)
{
	if (bTutorialSequence)
		return;

	InputForwarding_AxisForward();
	if (Value < KINDA_SMALL_NUMBER)
	{
		SprintOff();
	}

	AddMovementInput(GetActorForwardVector() * Value);
}

void AGPPlayerPawn::MoveRight(float Value)
{
	if (bTutorialSequence)
		return;
	InputForwarding_AxisRight();
	AddMovementInput(GetActorRightVector() * Value);
}

void AGPPlayerPawn::MouseHorizontal(float Value)
{
	AddControllerYawInput(Value * LookSensitivity);
}

void AGPPlayerPawn::MouseVertical(float Value)
{
	AddControllerPitchInput(Value * LookSensitivity);
}

void AGPPlayerPawn::Interact()
{
	InteractionComponent->Interact();
}

void AGPPlayerPawn::SwapToTool(AGPToolBase* Tool, void (AGPPlayerPawn::*OnSwapEvent)())
{
	if (Tool == nullptr && CurrentTool == nullptr)
		return;

	if (Tool == CurrentTool)
		CurrentTool->StartSwap(nullptr, &AGPPlayerPawn::OnUnequip);

	if (CurrentTool == nullptr)
	{
		ToolActive(Tool, true);
		Tool->Equip();
		(this->*OnSwapEvent)();
		Tool->InitToolBindings();
		CurrentTool = Tool;
	}
	else
	{
		CurrentTool->StartSwap(Tool, OnSwapEvent);
	}
}


void AGPPlayerPawn::UnlockMovement()
{
	if (Spyglass != nullptr && Journal != nullptr && Map != nullptr)
	{
		bTutorialSequence = false;
	}
};

void AGPPlayerPawn::UpdateMovementSpeed(float DeltaTime)
{
	if (bSprinting && bIsCrouched)
	{
		SprintOff();
	}

	if (GetCharacterMovement()->Velocity.Z < 0)
	{
		bSlowedDown = true;
		JumpSlowTimer = 0.f;
	}

	const float SprintMulti = SprintSpeed / Speed;
	const float SprintAccMulti = SprintAcceleration / Acceleration;
	const float SlowMulti = 0.5;

	float CurrentSpeed = Speed;
	float CurrentAcceleration = Acceleration;
	if (bSprinting)
	{
		CurrentSpeed *= SprintMulti;
		CurrentAcceleration *= SprintAccMulti;
	}
	if (bSlowedDown)
	{
		const float t = JumpSlowTimer / SlowedDownDuration;
		const float LerpMulti = FMath::Lerp(SlowMulti, 1.f, t);
		CurrentSpeed *= LerpMulti;
		CurrentAcceleration *= LerpMulti;
	}

	//apply movement modifiers
	GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
	GetCharacterMovement()->MaxAcceleration = CurrentAcceleration;

	if (!GetCharacterMovement()->IsFalling() && JumpSlowTimer <= SlowedDownDuration)
	{
		JumpSlowTimer += DeltaTime;
		if (JumpSlowTimer >= SlowedDownDuration)
		{
			bSlowedDown = false;
		}
	}
}


void AGPPlayerPawn::UnlockSpyglass()
{
	Spyglass = Cast<AGPSpyglass>(SpyglassActorComponent->GetChildActor());
	UnlockMovement();
}

void AGPPlayerPawn::UnlockJournal()
{
	Journal = Cast<AGPJournal>(JournalActorComponent->GetChildActor());
	UnlockMovement();
}

void AGPPlayerPawn::UnlockMap()
{
	Map = Cast<AGPMap>(MapActorComponent->GetChildActor());
	UnlockMovement();
}

void AGPPlayerPawn::EquipSlot4()
{
	SwapToTool(nullptr, &AGPPlayerPawn::OnUnequip);
}

void AGPPlayerPawn::EquipSlot3()
{
	SwapToTool(Map, &AGPPlayerPawn::OnMapEquip);
}

void AGPPlayerPawn::EquipSlot2()
{
	SwapToTool(Spyglass, &AGPPlayerPawn::OnSpyglassEquip);
}

void AGPPlayerPawn::EquipSlot1()
{
	SwapToTool(Journal, &AGPPlayerPawn::OnJournalEquip);
}


auto AGPPlayerPawn::ToolActive(AActor* ToolActor, bool Active) -> void
{
	ToolActor->SetActorTickEnabled(Active);
	ToolActor->SetActorHiddenInGame(!Active);
}
