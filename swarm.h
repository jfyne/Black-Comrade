#ifndef _SWARM_H 
#define _SWARM_H

#include <OGRE/Ogre.h>
#include <string>
#include <sstream>
#include <iostream>
#include <math.h>
#include "enemy.h"
#include "rayQuery.h"
#include "shipState.h"
#include "sceneNodeManager.h"
#include "lines.h"
#include "collisionManager.h"
#include "constManager.h"
#include "mapManager.h"
#include "pathFinder.h"
#include "gameParameterMap.h"

using namespace Ogre;
using namespace std;

enum SwarmState { SS_PATROL, SS_ATTACK };

class Swarm
{
    private:
        SceneManager *sceneMgr;
        SceneNodeManager *sceneNodeMgr;
        std::vector<Enemy*> members;
        std::vector<MapTile*> path;
        int id;
        Vector3 location;
        Vector3 target;
        MapTile* currentSwarmTile;
        MapTile* oldShipTile;
        MapTile* oldSwarmTile;
    	Real roll;
    	Real pitch;
    	Real yaw;
    	RayQuery *rRayQuery;
    	float speed;
    	SwarmState state;
    	ShipState *shipState;
    	Lines *lines;
    	CollisionManager *collisionMgr;
        MapManager *mapMgr;
        PathFinder *pathFinder;
        GameParameterMap *gameParameterMap;
    
    	void updateSwarmLocation();
    	void updateEnemyLocations();
        void updateTargetLocation();

    	void removeDeadEnemies();
    	void shootAtShip();
    	
    	void turnEnemy(Enemy *e);
    	
    	float calcNewAngle(float old, float target, float step);
    	
    public:
        int size;

        Swarm(int size, int id, Vector3 location, SceneManager *sceneMgr,
			Real roll, Real pitch, Real yaw, ShipState *shipState, SceneNodeManager *sceneNodeMgr, Lines *lines,
            CollisionManager *collisionMgr, MapManager *mapMgr, GameParameterMap *gameParameterMap);
        ~Swarm();

        Vector3 getAverageAlignment();
        Vector3 getAveragePosition();
    	bool isShipInSight();

        std::vector<Enemy*> getAllEnemies();
        
        void tick();

};

#endif
