#include "motionState.h"
#include <iostream>
#include <math.h>
using namespace std;

MotionState::MotionState(IEngineState *es):
    engineState(es),
    xMotion(0.0),
    yMotion(0.0),
    zMotion(0.0),
    mPitch(0.0),
    mRoll(0.0),
    mYaw(0.0)
{}

double MotionState::xVelocity()
{
    return xMotion;
}

double MotionState::yVelocity()
{
    return yMotion;
}

double MotionState::zVelocity()
{
    return zMotion;
}

double MotionState::pitch()
{
    return mPitch;
}

double MotionState::yaw()
{
    return mYaw;
}

double MotionState::roll()
{
    return mRoll;
}

void MotionState::tick()
{
    
    mYaw += engineState->turnPower() * 0.01;
    
    //mRoll -= engineState->turnPower() * 0.004;
    //if(mRoll > 0.01) mRoll = 0.01;
    //else if(mRoll < -0.01) mRoll = 0.01;
    
    zMotion = cos(mYaw) * engineState->enginePower() + sin(mYaw) * (engineState->sideThrusterPower());
    xMotion = cos(mYaw) * (-engineState->sideThrusterPower()) + sin(mYaw) * engineState->enginePower();
    yMotion = -(engineState->upThrusterPower());

    //double xPowerFrac = engineState->sideThrusterPower();
    //yMotion = (xMotion + xPowerFrac*SIDETHURSTERPOWER / MASS) * yFRIC;
    //cout << "xMotion: " << xMotion << endl;
    
    //mYaw = engineState->turnPower() * PI * TURNPERTICK;
    //cout << "yawAngleS: " << yawAngleS << endl;
    //pitchAngleS = engineState->pitchPower() * PI * TURNPERTICK ;
    //cout << "pitchAngleS: " << pitchAngleS << endl;
}
