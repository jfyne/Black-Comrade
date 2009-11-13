
#include "shipState.h"

#include <iostream>

ShipState::ShipState(SceneNode *shipSceneNode, IMotionState *motionState) :
    position(new Vector3(0.0,0.0,-500.0)),
    roll(0.0),
    pitch(0.0),
    yaw(0.0),
    shipSceneNode(shipSceneNode),
    motionState(motionState)
{}

void ShipState::tick()
{
    position->x += motionState->xVelocity();
    position->y += motionState->yVelocity();
    position->z += motionState->zVelocity();
    
    roll = motionState->roll();
    pitch = motionState->pitch();
    yaw = motionState->yaw();
    
    updateOgre();
}

void ShipState::updateOgre()
{
    //std::cout << position->x << "," << position->y << "," << position->z << std::endl;

	shipSceneNode->resetOrientation();

    shipSceneNode->setPosition(*position);
    
    Radian roll(this->roll);
    Radian pitch(this->pitch);
    Radian yaw(this->yaw);
    
    shipSceneNode->yaw(yaw);
    shipSceneNode->roll(roll);
    shipSceneNode->pitch(pitch);
        
    //std::cout << shipSceneNode->getPosition().z << std::endl;
}
