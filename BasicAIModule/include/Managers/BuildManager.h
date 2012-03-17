#pragma once
/*
 *  BuildManager.h
 *  
 *  Used to produce new units, buildings and add-ons
 *  Passes tasks down to the Construction or Production Managers
 */
#include "Manager.h"

// States a req can be in
enum BuildReqState {
	NEW,
	ISSUED,
	STARTED,
	COMPLETE
};

// Information needed to complete a build request
typedef struct BuildReq {
	// External
	BWAPI::UnitType type;
	BWAPI::TilePosition goalPosition;
	int count;

	// Internal
	BuildReqState state;
	BWAPI::Unit *builder; // SCV or Structure
} BuildReq;


class BuildManager : public Manager
{
private:
    queue<BuildReq> buildQueue;
    stack<BuildReq> buildStack;  // Active requests can be recursive (dependencies)

    void drawDebugText();

public: 
	void update();

	void build(BWAPI::UnitType type, bool immediate=false);
	void build(BWAPI::UnitType type, BWAPI::TilePosition goalPosition, bool immediate=false);

    virtual const std::string& getName() const 
    {
        static const std::string name("BuildMgr");
        return name; 
    }
};
