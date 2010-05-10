#include "tutorial.h"

Tutorial::Tutorial(CollaborationInfo *tutee,
                   CollaborationInfo *tutee1, CollaborationInfo *tutee2, CollaborationInfo *tutee3,
                   GuiManager *guiMgr, HUD *hud, MiniGameManager *miniGameMgr, DamageState *damageState,
                   SystemManager *systemMgr, ShipState *shipState, Door *door, InputState *inputState)
    : tutee(tutee)
    , tutee1(tutee1)
    , tutee2(tutee2)
    , tutee3(tutee3)
    , guiMgr(guiMgr)
    , hud(hud)
    , inputState(inputState)
    , miniGameMgr(miniGameMgr)
    , damageState(damageState)
    , systemMgr(systemMgr)
    , shipState(shipState)
    , state(TS_PRE)
    , door(door)
    , pauseProgress(0)
{}

TutorialState Tutorial::getState() { return state; }

void Tutorial::tick() {
    if (state == TS_END) return;

    tickCommonTutorial();
    switch(tutee->getGameRole()) {
        case PILOT:
            tickPilotTutorial();
            break;
        case ENGINEER:
            tickEngineerTutorial();
            break;
        case NAVIGATOR:
            tickNavigatorTutorial();
            break;
    }
    checkForCompletion();
    if (state == TS_INDIVIDUAL)
        door->open();
}

void Tutorial::tickCommonTutorial() {
    if (inputState->isKeyDown(OIS::KC_END)) {
        // Skip the tutorial
        state = TS_WAITING_FOR_OTHERS;
    }

    switch(state) {
        case(TS_PRE) :
            changeWithPause(TS_START,3);
            break;
        case(TS_START) :
            changeWithPause(TS_MISSION_LOG,9);
            break;
        case(TS_MISSION_LOG) :
            // Highlight the mission log to the players
            changeWithPause(TS_HEALTH_BARS,8);
            break;
        case(TS_HEALTH_BARS) :
            changeWithPause(TS_CHARGE_BARS,13);
            break;
        case(TS_CHARGE_BARS) :
            changeWithPause(TS_AVATARS,12);
            break;
        case(TS_AVATARS) :
            changeWithPause(TS_OPEN_CONSOLE,7);
            break;
        case(TS_OPEN_CONSOLE) :
            // We want the player to open the console
            //if (miniGameMgr->hasConsoleBeenOpened())
                changeWithPause(TS_REPAIR_SYSTEMS,0);
            //break;
        case(TS_CLOSE_CONSOLE) :
            // We want the player to close the console
            if (miniGameMgr->hasConsoleBeenClosed())
               state = TS_WAITING_FOR_OTHERS;
            break;
    }
}

void Tutorial::tickPilotTutorial() {
    switch(state) {
        case(TS_REPAIR_SYSTEMS) :
            state = TS_REPAIR_ENGINES;
            break;
        case(TS_REPAIR_ENGINES) :
            if (damageState->getEngineHealth()>=95)
               changeWithPause(TS_CLOSE_CONSOLE);
            break;
        case(TS_INDIVIDUAL) :
            changeWithPause(TS_PILOT_ROLE);
            break;
        case(TS_PILOT_ROLE) :
            changeWithPause(TS_MOVE_SHIP);
            break;
        case(TS_MOVE_SHIP) :
            // TODO: Check if the ship has been moved
            if (shipState->getSpeed() > 50)
                changeWithPause(TS_PILOT_END);
            break;
    }
}

void Tutorial::tickEngineerTutorial() {
    switch(state) {
        case(TS_REPAIR_SYSTEMS) :
            state = TS_REPAIR_WEAPONS;
            break;
        case(TS_REPAIR_WEAPONS) :
            if (damageState->getWeaponHealth()>=95)
               changeWithPause(TS_CLOSE_CONSOLE);
            break;
        case(TS_INDIVIDUAL) :
            changeWithPause(TS_ENGINEER_ROLE);
            break;
        case(TS_ENGINEER_ROLE) :
            changeWithPause(TS_POWER_BARS,1);
            break;
        case(TS_POWER_BARS) :
            changeWithPause(TS_MINI_RADAR,11);
            break;
        case(TS_MINI_RADAR) :
            changeWithPause(TS_SHOW_RADAR,11);
            break;
        case(TS_SHOW_RADAR) :
            // TODO: Check if the large radar has been shown
            changeWithPause(TS_CLOSE_RADAR,3);
            break;
        case(TS_CLOSE_RADAR) :
            // TODO: Check if the large radar has been closed
            changeWithPause(TS_CHANGE_POWERS,3);
            break;
        case(TS_CHANGE_POWERS) :
            // TODO: Check if the powers have been changed
            changeWithPause(TS_ENGINEER_END);
            break;
    }
}

void Tutorial::tickNavigatorTutorial() {
    switch(state) {
        case(TS_REPAIR_SYSTEMS) :
            state = TS_REPAIR_SENSORS;
            break;
        case(TS_REPAIR_SENSORS) :
            if (damageState->getSensorHealth()>=95)
               changeWithPause(TS_CLOSE_CONSOLE);
            break;
        case(TS_INDIVIDUAL) :
            changeWithPause(TS_NAVIGATOR_ROLE);
            break;
        case(TS_NAVIGATOR_ROLE) :
            changeWithPause(TS_MINI_MAP,500);
            break;
        case(TS_MINI_MAP) :
            changeWithPause(TS_SHOW_MAP,100);
            break;
        case(TS_SHOW_MAP) :
            // TODO: Check if the large map has been shown
            changeWithPause(TS_CLOSE_MAP,200);
            break;
        case(TS_CLOSE_MAP) :
            // TODO: Check if the large map has been closed
            changeWithPause(TS_NAVIGATOR_END);
            break;
    }
}

void Tutorial::changeWithPause(TutorialState newState, int pause) {
    if (pauseProgress > pause / ConstManager::getFloat("tick_period")) {
        state = newState;
        pauseProgress = 0;
    } else {
        pauseProgress ++;
    }
}

void Tutorial::checkForCompletion() {
    if (state == TS_WAITING_FOR_OTHERS) {
        tutee->hasCompletedTutorial = true;
        bool completed = (tutee1->hasCompletedTutorial &&
                          tutee2->hasCompletedTutorial &&
                          tutee3->hasCompletedTutorial);
        if (completed) state = TS_INDIVIDUAL;
    }
}

void Tutorial::end() {
    state = TS_END;
}
