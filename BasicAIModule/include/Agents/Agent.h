/*
 *  Agent.h 
 */
#pragma once
#include "State.h"
#include <Common.h>

#include <BWAPI.h>

class Manager;


class Agent 
{
protected:
	BWAPI::Unit &unit;                      // The Unit this Agent controls
	State state;                            // The State this Agent is in

	BWAPI::Unit *unitTarget;                // Targeted unit (for gather, attack, etc.)
	BWAPI::Position positionTarget;         // Targeted position (for moving, evade, etc.)
	BWAPI::UnitType unitTypeTarget;         // Targeted unit type (for production of units, buildings, etc.)

	bool buildingReserved;                  // Have we reserved space for a building yet?
	BWAPI::TilePosition buildingLocation;	// Location to build target building at

    Manager *parentManager;

public:  
	/*
	 * update - Called on each frame to update this agent's state and unit.
	 */
	virtual void update();
 
	bool operator==(const Agent& other);

	// attr_accessors
    inline void setState(State state)              { this->state = state; }
    inline void setUnitTarget(BWAPI::Unit *target) { unitTarget = target; }
	inline void setPositionTarget(const BWAPI::Position& target) { positionTarget = target; }
	inline void setUnitTypeTarget(const BWAPI::UnitType& target) { unitTypeTarget = target; }

	bool unitTypeTargetValid() { 
		int typeTargetID = unitTypeTarget.getID();
		return typeTargetID >= 0 && 
			typeTargetID < NUM_UNIT_TYPES && 
			typeTargetID != BWAPI::UnitTypes::None; 
	}

	inline const State getState()       const { return state; }
	inline const BWAPI::Unit& getUnit() const { return unit; }
	inline BWAPI::Unit& getUnit() { return unit; }  
    inline const BWAPI::Position& getPositionTarget() const { return positionTarget; }
    inline const BWAPI::UnitType& getUnitTypeTarget() const { return unitTypeTarget; }
    inline BWAPI::Unit* getUnitTarget() const { return unitTarget; }

    void setParentManager(Manager *manager);
    const std::string getParentManagerName() const;

protected:
	// C'tor (must be called from subclass)
	Agent(BWAPI::Unit &u);

	virtual void printState();
};
