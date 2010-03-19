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
#include "damageState.h"

#include <boost/random/normal_distribution.hpp>

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
        DamageState *damageState;
        ParticleSystemEffectManager *particleSystemEffectManager;
       
        bool fire(Vector3 origin, Vector3 direction, ColourValue c, Vector3 trailOrigin);
        bool fire(Vector3 origin, Vector3 direction, ColourValue c, Vector3 trailOrigin, PlayerStats *stats);

        void handleGun(GunState *gun);
        void handleEnemies(std::vector<Enemy*> enemies);
        void updateBullets(); 
        
    public:
        bool playerFire;
        bool enemyFire;
        SceneNode *enemyNode;

        BulletManager(ShipState *shipState, SceneManager *sceneMgr,
            GunState *pilotGunState, GunState *engineerGunState,
            GunState *navigatorGunState, CollisionManager *colMgr,
            SwarmManager *swarmMgr, SceneNodeManager *sceneNodeMgr,
            DamageState *damageState, ParticleSystemEffectManager *particleSystemEffectManager);

        ~BulletManager();
            
        virtual void tick();
        std::vector<Bullet*>* getActiveBullets();
};


#endif

