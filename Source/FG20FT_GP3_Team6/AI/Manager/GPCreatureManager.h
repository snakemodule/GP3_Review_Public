#pragma once

//#include "GPCreatureSpawner.h"
#include "FG20FT_GP3_Team6/AI/Manager/GPCreatureSpawnParameters.h"

class AGPCreatureBase;
class UGPCreatureSpawnerSettings;
class AGPActivitySpotBase;

class GPCreatureManager
{
public:
	void SpawnCreatureForActivity(const FGPCreatureSpawnParameters& SpawnParams);
	
	

	UGPCreatureSpawnerSettings* CreatureSpawnerSettings;
	AGPCreatureBase* CurrentCreature;
	float PhasePlaneSpawnOffset;
};