
#ifndef ENEMY_STATE_H
#define ENEMY_STATE_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>

#include "ITickable.h"
#include "rayQuery.h"

// include Raknet classes
#include "ReplicaObject.h"
#include "ReplicaManager3.h"

using namespace Ogre;
using namespace RakNet;

class EnemyState : public ITickable, public ReplicaObject {
    
private:
    SceneManager *mSceneMgr;
    RayQuery *rRayQuery;
    
    static const float FVEL = 0.3;
    //lookahead in ticks
    static const float LOOKA = 30;

    
public:
    SceneNode *eSceneNode;
    Vector3 *position;
    Real roll;
    Real pitch;
    Real yaw;
    
    EnemyState();
    EnemyState(SceneNode *neSceneNode, SceneManager *mSceneMgr );
    
    void tick();
    
    void updateOgre();

    virtual RakNet::RakString GetName(void) const;
    virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters);
    virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);

    void print();

    
};

#endif