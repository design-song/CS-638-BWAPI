/*
 * SCVAgent.cpp
 */
#include "SCVAgent.h"
#include "GroundAgent.h"

#include "TacticalBuildingPlacer.h"
#include "ResourceAdvisor.h"

#include <BWAPI.h>

using namespace BWAPI;


SCVAgent::SCVAgent(Unit &u) 
	: GroundAgent(u)
    , wasConstructing(false)
    , constructingStructure(NULL)
{ }

void SCVAgent::update()
{
	switch (state) {

		case IdleState:
			// be idle
			break;

		case GatherState:
			// Gather minerals
			// Exhausted mineral fields become Unknown type, so we need to check against that type
			if (unitTarget->getType().isMineralField() || 
				unitTarget->getType().getID() == UnitTypes::Unknown.getID()) 
			{
				// Get a new mineral patch if this one is exhausted
				if (unitTarget->getResources() <= 0) {
					Unit* mineralPatch = ResourceAdvisor::getClosestMineralPatch(*this);
					if (mineralPatch != NULL)
						unitTarget = mineralPatch;
					unit.gather(unitTarget);
				}
				// Initiate gathering action
				if (!unit.isGatheringMinerals())
					unit.gather(unitTarget);
			}

			// Gather gas
			else if (unitTarget->getType().isRefinery()) 
			{
				if (!unit.isGatheringGas())
					unit.gather(unitTarget);
			}
			break;

		case BuildState:

			// Done?
			if (!unit.isConstructing() && constructingStructure != NULL)
			{
				if (constructingStructure->isCompleted())
				{
					constructingStructure = NULL;
					buildingReserved = false;
					setUnitTypeTarget(UnitTypes::None);
				}
			}

			// Need new target?
			else if (unitTypeTarget == UnitTypes::None &&
				!unit.isTraining() &&
				!unit.isBeingConstructed() && 
				!unit.isConstructing()) {
				if (!buildQueue.empty()) {
					unitTypeTarget = buildQueue.front();
					buildQueue.pop();
				} else {
					setState(IdleState);
				}
			}

			// Reserve a build location
			else if (!buildingReserved)
			{
				TacticalBuildingPlacer &tbp = TacticalBuildingPlacer::instance();
				buildingLocation = tbp.instance().reserveBuildLocation(unitTypeTarget, Broodwar->self()->getStartLocation(), &unit);
				if (buildingLocation != TilePositions::None)
				{
					buildingReserved = true;
				}
				unit.move(Position(buildingLocation));
			}

			// Build it
			else if (buildingReserved && !unit.isConstructing())
			{
				unit.build(buildingLocation, unitTypeTarget);
			}

			// Building...
			else if (unit.isConstructing())
			{
				constructingStructure = unit.getBuildUnit();
			}
			break;
	}

    GroundAgent::update();
}

bool SCVAgent::gatherMinerals()
{
	bool success = false;
	Unit *closest = ResourceAdvisor::getClosestMineralPatch(*this);
	if( closest != NULL )
	{
		setState(GatherState);
		setUnitTarget(closest);
		setUnitTypeTarget(UnitTypes::None);
		setPositionTarget(closest->getPosition());
		success = true;
	}

	return success;
}