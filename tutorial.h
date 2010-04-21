#ifndef TUTORIAL
#define TUTORIAL

#include "ITickable.h"
#include "guiManager.h"
#include "hud.h"
#include "miniGameManager.h"
#include "collaborationInfo.h"
#include "door.h"
#include "tutorial.h"

#include <iostream>

enum TutorialState { TS_PRE, TS_START,
                     TS_SHOW_CONTROLS,
                     TS_SHOW_MAP, TS_SHOW_RADAR,
                     TS_OPEN_CONSOLE, TS_CLOSE_CONSOLE,
                     TS_REPAIR_SYSTEMS,
                     TS_WAITING_FOR_OTHERS, TS_END };

class Tutorial : public ITickable {
    private:

        GuiManager *guiMgr;
        HUD *hud;
        TutorialState state;
        MiniGameManager *miniGameMgr;
        DamageState *damageState;
        InputState *inputState;

        CollaborationInfo *tutee;

        CollaborationInfo *tutee1;
        CollaborationInfo *tutee2;
        CollaborationInfo *tutee3;

        Door *door;

        int pauseProgress;

        void tickCommonTutorial();
        void tickPilotTutorial();
        void tickEngineerTutorial();
        void tickNavigatorTutorial();

        void checkForCompletion();

        void changeWithPause(TutorialState newState, int pause = 150);

    public:
        TutorialState getState();
        Tutorial(CollaborationInfo *tutee,
                 CollaborationInfo *tutee1, CollaborationInfo *tutee2, CollaborationInfo *tutee3,
                 GuiManager *guiMgr, HUD *hud, MiniGameManager *miniGameMgr, DamageState *damageState,
                 Door *door, InputState *inputState);
	virtual void tick();
};

#endif
