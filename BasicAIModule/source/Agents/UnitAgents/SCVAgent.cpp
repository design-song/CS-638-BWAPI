/*
 * SCVAgent.cpp
 */
#include "UnitAgents/SCVAgent.h"
#include "GroundAgent.h"
#include <TacticalBuildingPlacer.h>

#include <BWAPI.h>

using BWAPI::Unit;


SCVAgent::SCVAgent(Unit &u) 
	: GroundAgent(u)
{}

void SCVAgent::update()
{
	if (state) {
		switch (state) {
			case GatherState:
				// Gather minerals
				if (unitTarget->getType().isMineralField()) 
				{
					if (!unit.isGatheringMinerals())
						unit.gather(unitTarget);
				// Gather gas
				} else if (unitTarget->getType().isRefinery()) {
					if (!unit.isGatheringGas())
						unit.gather(unitTarget);
				}
				break;

			case BuildState:
				if (!unit.isConstructing())
				{
					BWAPI::TilePosition loc = TacticalBuildingPlacer::instance().reserveBuildLocation(unitTypeTarget, BWAPI::Broodwar->self()->getStartLocation(), &unit);
					if (loc != BWAPI::TilePositions::None)
					{
						unit.build(loc, unitTypeTarget);
					}
				}

			default:
				GroundAgent::update();
		}
	}
}
