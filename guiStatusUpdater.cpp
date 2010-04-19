#include "guiStatusUpdater.h"

GuiStatusUpdater::GuiStatusUpdater(GuiManager *guiMgr, StateUpdate *stateUpdate, DamageState *damageState,
                                   GunnerControls *playerControls, GameRole gameRole,
                                   SystemManager *systemManager, HUD *hud, Flying *flying,
                                   NotificationManager *notificationMgr,
                                   GameStateMachine *gameStateMachine, Objective *objective,
                                   CollaborationInfo *pilotInfo, CollaborationInfo *navInfo, CollaborationInfo *engInfo) :
    guiMgr(guiMgr),
    stateUpdate(stateUpdate),
    damageState(damageState),
    playerControls(playerControls),
    gameRole(gameRole),
    systemManager(systemManager),
    hud(hud),
    flying(flying),
    notificationMgr(notificationMgr),
    gameStateMachine(gameStateMachine),
    objective(objective),
    pilotInfo(pilotInfo),
    engInfo(engInfo),
    navInfo(navInfo)
{}

GuiStatusUpdater::~GuiStatusUpdater() {}

void GuiStatusUpdater::tick() {

    // Display the slack in the game time
    long slack = stateUpdate->getSlack();
    std::string s;
    std::stringstream out;
    out << slack << "ms";
    s = out.str();
    hud->setStatus(s);

    // Ship speed
    if(gameRole==PILOT) {
        double speed = flying->getSpeed();
        std::string t;
        std::stringstream outt;
        outt << int(speed) << " KM/H";
        t = outt.str();
        hud->setSpeedIndicator(t);
    }

    // Deal with the damage of various systems
    float sensorHealth = (float)(damageState->getSensorHealth()/100.0);
    float weaponHealth = (float)(damageState->getWeaponHealth()/100.0);
    float engineHealth = (float)(damageState->getEngineHealth()/100.0);
    float hullHealth = (float)(damageState->getHullHealth()/100.0); 

    hud->setSensors(sensorHealth);
    hud->setWeapons(weaponHealth);
    hud->setEngines(engineHealth);
    hud->setHull(hullHealth);
    
    // get game state and set HUD accordingly
    
    GameState gameState = gameStateMachine->currentGameState();
    if (gameStateMachine->isNewState()) {
        switch(gameState) {
            case GS_STEALTH:
                hud->switchStatus(1);
                break;
            case GS_ATTACK:
                hud->switchStatus(4);
                break;
            case GS_FLEE:
                hud->switchStatus(5);
                break;
         }
    }

    // Toggle whether the controls are shown or not
    hud->toggleControls(playerControls->showControls());


    // If player is navigator they can toggle the full screen map here
    if(gameRole==NAVIGATOR) {
        if(((NavigatorControls*) playerControls)->isMap()) {
            hud->toggleMap(true);
        } else {
            hud->toggleMap(false);
        }
        // Updates the images on the minimap
        hud->updateMiniMap();
   }

    // Update the state of the power system bars
    if(gameRole==ENGINEER) {
        float shieldRate = (float)(systemManager->getShieldRate());
        float weaponRate = (float)(systemManager->getWeaponRate());
        float engineRate = (float)(systemManager->getEngineRate());

        
        hud->setShieldRate(shieldRate);
        hud->setWeaponRate(weaponRate);
        hud->setEngineRate(engineRate);
       }

    // TODO: Show player status / stats
    bool pilotRepairing = pilotInfo->repairing;
    bool engineerRepairing = engInfo->repairing;
    bool navigatorRepairing = navInfo->repairing;
    int pilKillCount = pilotInfo->getPlayerStats()->enemiesDestroyed;
    int navKillCount = navInfo->getPlayerStats()->enemiesDestroyed;
    int engKillCount = engInfo->getPlayerStats()->enemiesDestroyed;
    
    std::string status1 = "Fighting";
    std::string status2 = "Fighting";
    int kills1 = 0;
    int kills2 = 0;
    std::string rpr = "Repairing";
    
    
    if (gameRole==PILOT) {
    	if (navigatorRepairing) { status1 = rpr; }
    	if (engineerRepairing) { status2 = rpr; }
    	kills1 = navKillCount;
    	kills2 = engKillCount;
	}
	if (gameRole==ENGINEER) {
		if (pilotRepairing) { status1 = rpr; }
		if (navigatorRepairing) { status2 = rpr; }
		kills1 = pilKillCount;
		kills2 = navKillCount;
		
	}
	if (gameRole==NAVIGATOR) {
		if (pilotRepairing) { status1 = rpr; }
    	if (engineerRepairing) { status2 = rpr; }
		kills1 = pilKillCount;
    	kills2 = engKillCount;
	}
      
    hud->setTeamInfo(status1,status2,kills1,kills2);
    /**/ 
    float weaponCharge = (float)(systemManager->getWeaponCharge());
    float shieldCharge = (float)(systemManager->getShieldCharge());
    hud->setWeaponCharge(weaponCharge/100.0);
    hud->setShieldCharge(shieldCharge/100.0);

    float bossHealth = objective->getHealthPercentage();
    hud->setBossHealthbar(bossHealth);

    int t = objective->getEscapeTime();
    std::stringstream count;
    count << " " << t << " seconds";
    hud->setCountdown(count.str());

    // Update transmission log

    if (notificationMgr->hasNewNotification()) {
        Notification *notification = notificationMgr->getCurrentNotification();
        if (notification->getType() != NT_NONE) {
            hud->setLog(notification->getConsoleText());
        }
    }
}
