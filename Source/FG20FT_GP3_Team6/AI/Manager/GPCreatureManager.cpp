#include "GPCreatureManager.h"

#include "FG20FT_GP3_Team6/AI/ActivitySpots/GPActivitySpotBase.h"
#include "FG20FT_GP3_Team6/AI/GPCreatureBase.h"
#include "FG20FT_GP3_Team6/AI/Manager/GPCreatureSpawnerSettings.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

void GPCreatureManager::SpawnCreatureForActivity(const FGPCreatureSpawnParameters& SpawnParams)
{
	if(CreatureSpawnerSettings == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Received request to spawn creature but creature spawner settings is null, aborting"))
		return;
	}
	
	if (CurrentCreature != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Received request to spawn creature when a creature already exists, aborting"))
		return;
	}
	
	if (SpawnParams.ActivitySpot == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Received request to spawn creature with null activity, aborting"))
		return;
	}	
	
	UWorld* WorldPtr = SpawnParams.ActivitySpot->GetWorld();
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnParams.SpawnLocation);
	
	CurrentCreature = WorldPtr->SpawnActor<AGPCreatureBase>(CreatureSpawnerSettings->CreatureClass, SpawnTransform);
	CurrentCreature->SetActivitySpot(SpawnParams.ActivitySpot);
	SpawnParams.ActivitySpot->CreatureSpawned();
	
	PhasePlaneSpawnOffset = 400; //todo don't set here
	FVector ActivityDirection = SpawnParams.ActivitySpot->GetActivityLocationWorldSpace() - CurrentCreature->GetActorLocation();	
	ActivityDirection.Z = 0;
	ActivityDirection.Normalize(); 
	FVector PlanePos = CurrentCreature->GetActorLocation()+(ActivityDirection*PhasePlaneSpawnOffset);	
	CurrentCreature->SetDissolvePlane(PlanePos, -ActivityDirection);


	AAIController* CreatureController = Cast<AAIController>(CurrentCreature->GetController());
	if (CreatureController)
	{		
		if (CreatureController->RunBehaviorTree(CreatureSpawnerSettings->CreatureBehaviorTreeBase))
		{			
			UBlackboardComponent* CreatureBlackboard = CreatureController->GetBlackboardComponent();
			CreatureBlackboard->SetValueAsObject("ActivityActor", SpawnParams.ActivitySpot); // TODO hard-coded key names are not good.
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Creature BT start failed"))
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("creature does not have ai controller"))
	}
}
