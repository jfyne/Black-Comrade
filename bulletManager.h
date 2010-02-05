#ifndef _BULLETMANAGER_H
#define	_BULLETMANAGER_H

#include <OGRE/Ogre.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "gunState.h"
#include "ITickable.h"
#include "bullet.h"
#include "collisionManager.h"
#include "swarmManager.h"
#include "shipState.h"

using namespace Ogre;
using namespace std;

class BulletManager : public ITickable
{
    private:
        std::vector<Bullet*> *activeBullets;
        ShipState *shipState;
        SceneManager *sceneMgr;
        SceneNodeManager *sceneNodeMgr;
        int bnum;
        GunState *pilotGunState;
        GunState *navigatorGunState;
        GunState *engineerGunState;
        CollisionManager *colMgr;
        SwarmManager *swarmMgr;
        
        void fire(Vector3 origin, Vector3 direction, ColourValue c);
        void handleGun(GunState *gun);
        void updateBullets(); 
        
    public:
        BulletManager(ShipState *shipState, SceneManager *sceneMgr,
            GunState *pilotGunState, GunState *engineerGunState,
            GunState *navigatorGunState, CollisionManager *colMgr,
            SwarmManager *swarmMgr, SceneNodeManager *sceneNodeMgr);

        ~BulletManager();
            
        virtual void tick();
};


#endif

