#ifndef COLLISION_LIGHTMGR_H
#define COLLISION_LIGHTMGR_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>
#include <vector>
#include <math.h>
#include "ITickable.h"

using namespace Ogre;
using namespace std;

class LightManager : public ITickable
{
  private:
    std::vector<Light*> connPieceMainSPLight;
    double angle;
    SceneManager *sceneManager;


  public:
    LightManager(SceneManager *sceneManager);
    void addConnPieceSPLight( Vector3 pos );
    virtual void tick();
    ~LightManager(){}
};

#endif
