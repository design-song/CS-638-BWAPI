#pragma once
/*
 * CombatManager.h
 *  
 * Used to defend our base and attack other bases
 */
#include "Manager.h"
#include "Common.h"

#include <BWAPI.h>


class CombatManager : public Manager
{
private:
    AgentSet unassignedAgents;
    AgentSet assignedAgents;
    SquadVector squads;
    BWAPI::Position enemyBase;

public: 
    virtual void onMatchStart();
	virtual void update();
    
    virtual const std::string& getName() const 
    { 
        static const std::string name("CombatMgr");
        return name;
    }
};
