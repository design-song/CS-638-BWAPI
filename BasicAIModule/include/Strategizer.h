#pragma once
/* 
 * Strategizer.h
 */
#include "GameEvent.h"
// #include "GameStateDB.h"
#include "IncludeAllManagers.h"
#include "Common.h"

#include "TacticalBuildingPlacer.h"

#include <BWAPI.h>


class Strategizer
{
public:
	BuildManager      buildManager;
	CombatManager     combatManager;
	GasManager        gasManager;
	ProductionManager productionManager;    // TODO: remove this once BuildMgr is more complete
	ResourceManager   resourceManager;
	ScoutManager      scoutManager;
	SupplyManager     supplyManager;

private:
	UnitAgentMap      unitAgentMap;
	AgentManagerMap   agentManagerMap;

	// Game State analysis
	// GameStateDB gsdb;

public:
	
	/* Strategizer is a Singleton class, access is through it's instance method */
	static Strategizer& instance() { static Strategizer s; return s; }

	/* update - Called by AI Module for each frame */
	void update();

	/* onMatchStart - Called by AI Module when a new match begins */
	void onMatchStart();

    /* onMatchEnd - Called by AI Module whena  new match ends */
    void onMatchEnd(bool isWinner);

	/* onEvent - Called by EventProducer when a new event is detected */
	void onEvent(GameEvent& e);
  
private:  
	// Strategizer is singleton, hence private ctors/assignment
	Strategizer() { }
	Strategizer(const Strategizer& other);
	void operator=(const Strategizer& other);

	// Core update() functions
	/* updateUnitAgentMap() - Gives an Agent to new Units, removes Agents of inactive Units */
	void updateUnitAgentMap();

	/* updateAgentManagerMap() - Reassigns Agents to Managers based on bid values */
	void updateAgentManagerMap();

	/* redistributeAgents() - Reallocate Agents to Managers, and make reassignments
	   if they more efficiently reallocate */
	void redistributeAgents();

	/* updateManagers() - Calls each Manager's update() method */
	void updateManagers();

	// Utility
	/* remap - Remap a unit of type @type from @src to @dst */
	bool remap(BWAPI::UnitType type, Manager &src, Manager &dst);
};


