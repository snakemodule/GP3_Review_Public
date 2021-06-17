// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"



#include "FG20FT_GP3_Team6/Player/Tools/Spyglass/GPDiscoveryWidget.h"
#include "FG20FT_GP3_Team6/Player/Tools/Journal/GPJournal.h"
#include "GameFramework/Character.h"
#include "Tools/GPDummyTool.h"
#include "Tools/Spyglass/GPSpyglass.h"
#include "Tools/GPToolBase.h"

#include "GPPlayerPawn.generated.h"

class USoundCue;
class AGPMap;
class UGPMovementComponent;
class UGPInteractionComponent;
class UCapsuleComponent;
class UCameraComponent;

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class FG20FT_GP3_TEAM6_API AGPPlayerPawn : public ACharacter
{
	GENERATED_BODY()

public:
		
	AGPPlayerPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SkipTutorial();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAudioComponent* VoiceOverComponent;	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAudioComponent* AudioComponent;
	UPROPERTY(EditDefaultsOnly)
    USoundCue* SoundCue;

	UFUNCTION(BlueprintImplementableEvent)
	void OnMapFragmentDiscovered(bool bRunestoneMapFragment);
	UFUNCTION(BlueprintImplementableEvent)
	void OnJournalEntryDiscovered();

	UFUNCTION(BlueprintImplementableEvent)
    void OnSpyglassEquip();
	UFUNCTION(BlueprintImplementableEvent)
    void OnJournalEquip();
	UFUNCTION(BlueprintImplementableEvent)
    void OnMapEquip();
	UFUNCTION(BlueprintImplementableEvent)
	void OnUnequip();

	
	TArray<FHitResult> VisionLineTraceHits;

	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	float DefaultDiscoveryRange = 10000;
		
	float DiscoveryRange = 10000;

	UPROPERTY(EditDefaultsOnly)
	float DesiredFOV = 100;

	UPROPERTY(Transient)
	AGPToolBase* CurrentTool = nullptr;

	UFUNCTION(BlueprintImplementableEvent)
    void SetDiscoveryPercent(float Percent);
	UFUNCTION(BlueprintImplementableEvent)
    void BeginDiscover();	
	UFUNCTION(BlueprintImplementableEvent)
    void EndDiscover();
	UFUNCTION(BlueprintImplementableEvent)
    void SpyglassEnabled(bool bEnabled);
	UFUNCTION(BlueprintImplementableEvent)
    void DiscoveryGracePeriodStart();
	UFUNCTION(BlueprintImplementableEvent)
    void DiscoveryGracePeriodReset();

	UFUNCTION(BlueprintImplementableEvent)
    void InputForwarding_AxisForward();
	UFUNCTION(BlueprintImplementableEvent)
    void InputForwarding_AxisRight();
	UFUNCTION(BlueprintImplementableEvent)
    void OnSprint();
	UFUNCTION(BlueprintImplementableEvent)
    void OnSprintEnd();

	void UnlockSpyglass(); 
	void UnlockJournal();  
	void UnlockMap();      
protected:
	
	virtual void BeginPlay() override;

private:	
	bool bSlowedDown = false;
	float JumpSlowTimer = 0.f;

	UPROPERTY(VisibleAnywhere, Category = Interaction)
	UGPInteractionComponent* InteractionComponent;
	
	
	UPROPERTY(Transient)                 
	AGPSpyglass* Spyglass = nullptr;	 
	UPROPERTY(Transient)                 
	AGPJournal* Journal = nullptr;	     
	UPROPERTY(Transient)                 
	AGPMap* Map = nullptr;               
                                         
	UPROPERTY(Transient)          
	UChildActorComponent* SpyglassActorComponent = nullptr;
	UPROPERTY(Transient)
	UChildActorComponent* JournalActorComponent = nullptr;
	UPROPERTY(Transient)
	UChildActorComponent* MapActorComponent = nullptr;          	

	bool bSprinting = false;

	UPROPERTY(EditDefaultsOnly)
	bool bTutorialSequence = false;
	
	UPROPERTY(EditAnywhere, Category = Movement)
	float JumpSpeed = 2000;
	UPROPERTY(EditAnywhere, Category = Movement)
	float Speed = 500;

	UPROPERTY(EditAnywhere, Category = Movement)
	float SlowDownMultiplier = 0.5;
	UPROPERTY(EditAnywhere, Category = Movement)
	float SlowedDownDuration = 2;	
	UPROPERTY(EditAnywhere, Category = Movement)
	float Acceleration = 1600;
	UPROPERTY(EditAnywhere, Category = Movement)
	float SprintSpeed = 1000;
	UPROPERTY(EditAnywhere, Category = Movement)
	float SprintAcceleration = 2000;
	
	UPROPERTY(EditAnywhere, Category = Movement)
	float LookSensitivity = 1;
		
	UPROPERTY(Transient)
	UGPDiscoverableComponent* Watching = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	float DiscoveryTime = 3.f;
	float DiscoveryInProgress = 0.f;
	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	float DiscoveryGracePeriod = 0.6f;
	float GraceTimer = 0.f;

	void MoveForward(float Value);
	void MoveRight(float Value);
	
	void MouseHorizontal(float Value);
	void MouseVertical(float Value);

	void Interact();
	void SwapToTool(AGPToolBase* Tool, void (AGPPlayerPawn::*OnSwapEvent)());

	void SwapToTool(AGPToolBase* Tool, void (*OnSwapEvent)());
	void UnlockMovement();
	void UpdateMovementSpeed(float DeltaTime);

	static void ToolActive(AActor* ToolActor, bool Active);
		
	void EquipSlot1();
	void EquipSlot2();
	void EquipSlot3();
	void EquipSlot4();
	
	void virtual Jump() override;
	void Duck();
	void Stand();

	void SprintOn();
	void SprintOff();

	

};
