#include "GPSonarSenseComponent.h"


#include "Components/AudioComponent.h"
#include "FG20FT_GP3_Team6/AI/GPCreatureBase.h"
#include "FG20FT_GP3_Team6/Player/GPPlayerPawn.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SenseSettings/GPSonarSenseSettings.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

UGPSonarSenseComponent::UGPSonarSenseComponent() : Super()
{
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UGPSonarSenseComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateSonar(DeltaTime);
}

void UGPSonarSenseComponent::BeginPlay()
{
	Super::BeginPlay();
	SonarSphere = Cast<AGPCreatureBase>(GetOwner())->SonarSphere;

	
}

void UGPSonarSenseComponent::UpdateSonar(float DeltaTime)
{
	if (SenseSettings == nullptr)
	{
		return;
	}
	auto Creature = Cast<AGPCreatureBase>(GetOwner());
	

	const float DistanceToDissolvePlane = FVector::Distance(Creature->GetActorLocation(), Creature->DissolvePlane->GetComponentLocation());

	if (DistanceToDissolvePlane < PhasingThresholdDistance && !bIsSonarActive)
	{
		return;
	}
	
	SonarTimer += DeltaTime;
	

	
	if (!bIsSonarActive)
	{
		if (SonarTimer <= SenseSettings->TimeBetweenPulses)
		{
			float Alpha = SonarTimer / SenseSettings->TimeBetweenPulses;
			Alpha = FMath::Clamp(2.0f * (Alpha - 0.5f), 0.0f, 1.0f);
			float Value;			
			{
				//Value = FMath::Clamp((1/Alpha)*50.0f, 0.f, 10000.f);
				//Value = FMath::SmoothStep(.0f, 1.f, Alpha - 0.5f);
				//Value *= 800.f;
				Value = Alpha * Alpha * 100.0f;
				Value = FMath::Max(0.8f, Value);
			}
			//GEngine->AddOnScreenDebugMessage(1337, 30.0f, FColor::Red, FString::SanitizeFloat(Value));
			Creature->AntlerDynamicMaterial->SetScalarParameterValue("EmissiveColorMultiplier", Value);
			return;
		}
		
		SonarTimer -= SenseSettings->TimeBetweenPulses;
		bIsSonarActive = true;

		SonarSphere->SetVisibility(true);
		SonarSphere->SetWorldLocation(GetOwner()->GetActorLocation());
		//Creature->AntlerDynamicMaterial->SetScalarParameterValue("EmissiveColorMultiplier", 0.8f);
		
		if (SonarSound != nullptr)
		{
			auto AudioComponent = Cast<AGPCreatureBase>(GetOwner())->AudioComponent;
			AudioComponent->SetSound(SonarSound);
			AudioComponent->Play();
		}
	}

	float Alpha = 1.0f - FMath::Clamp(10.0f * SonarTimer / SenseSettings->ExpansionDuration, 0.0f, 1.0f);
	float Value = FMath::SmoothStep(.0f, 1.f, Alpha - 0.5f);
	Value *= 800.f;
	Value = FMath::Max(0.8f, Value);
	Creature->AntlerDynamicMaterial->SetScalarParameterValue("EmissiveColorMultiplier", Value);

	if (SonarTimer >= SenseSettings->ExpansionDuration + SenseSettings->MaxRangeDuration)
	{
		SonarTimer -= SenseSettings->ExpansionDuration + SenseSettings->MaxRangeDuration;
		bIsSonarActive = false;

		SonarSphere->SetVisibility(false);
		return; // the wrap-around isn't seamless, but it'll do for now
	}

	const float RangeControlT = FMath::Clamp(SonarTimer / SenseSettings->ExpansionDuration, 0.0f, 1.0f);
	const float CurrentRange = RangeControlT * SenseSettings->MaxRange;

	if (SonarSphere->IsVisible())
	{
		SonarSphere->SetWorldScale3D(FVector(CurrentRange / SphereMeshRadius));		
	}

	if (bDebugDrawPulse)
	{
		UKismetSystemLibrary::DrawDebugSphere(GetOwner(), SonarSphere->GetComponentLocation(), CurrentRange, 12,
		                                      FLinearColor::Blue, 0, 10.0f);
	}

	const AGPPlayerPawn* PlayerPawn = Cast<AGPPlayerPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (PlayerPawn != nullptr)
	{
		const FVector& PlayerVelocity = PlayerPawn->GetCharacterMovement()->Velocity;
		const float DistanceToSonarOrigin = FVector::Distance(PlayerPawn->GetActorLocation(), SonarSphere->GetComponentLocation());// not using squared, since we're passing this value to blueprints			
		if (DistanceToSonarOrigin < CurrentRange && !PlayerVelocity.IsNearlyZero())
		{
			FGPSonarSenseResults SenseResults;
			SenseResults.PlayerLocation = PlayerPawn->GetActorLocation();
			SenseResults.DistanceToPlayer = DistanceToSonarOrigin;
			OnSonarTargetFound.Broadcast(SenseResults);
		}
	}
}
