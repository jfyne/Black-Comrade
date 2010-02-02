#ifndef _FLYING_H 
#define _FLYING_H

#include <OGRE/Ogre.h>
#include <math.h>
#include "ITickable.h"
#include "shipControls.h"
#include "collisionManager.h"

using namespace Ogre;
using namespace std;

class Flying : public ITickable
{
    private:
        CollisionManager *colMgr;
        ShipControls *sc;
        
        double zVel;
        double xVel;
        double yVel;
        
        double addPitch;
        double addRoll;
        //double addYaw;
        
        double flyPitch;
        double flyYaw;
        double flyRoll;
        
        static const double EngineForce = 0.8;
        static const double SideForce = 0.3;

        
        
        void updateAngels();
        
        // double getDrag(string dir);
        void updatePosition();
        // void changeVector();
    public:
        Vector3 *position; //
        // Vector3 *velocity; // ( Units per tick )
        //Vector3 *angularVelocity; // pitch yaw roll ( Radians per tick )
        //Vector3 *orientation; // pitch yaw roll ( Radians )
        double pitch;
        double yaw;
        double roll;
        
        Flying(ShipControls *sc,CollisionManager *colMgr);

        ~Flying();

        virtual void tick();
};

#endif

