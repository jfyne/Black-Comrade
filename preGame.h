#ifndef PRE_GAME_H
#define PRE_GAME_H

#include <OGRE/Ogre.h>

#include "menuSystem.h"
#include "networkingManager.h"
#include "networkRole.h"
#include "gameRole.h"
#include "stateUpdate.h"
#include "ITickable.h"

#include "storyMenu.h"
#include "networkRoleMenu.h"
#include "gameRoleMenu.h"
#include "loadingScreen.h"

class PreGame : public ITickable, protected MenuSystem {
private:

    NetworkingManager *networkingMgr;

    void loadNextMenu();

    IMenuScreen *storyMenu;
    IMenuScreen *networkRoleMenu;
    IMenuScreen *gameRoleMenu;
    LoadingScreen *loadingScreen;

public:

    PreGame(SceneManager *sceneMgr, Ogre::RenderWindow *window,
            InputState *inputState,GuiManager *guiMgr,
            NetworkingManager *networkingMgr);

    CollaborationInfo *showMenus();

    void waitForPlayers(CollaborationInfo *myInfo,
                        CollaborationInfo *pilotInfo,
                        CollaborationInfo *engineerInfo,
                        CollaborationInfo *navigatorInfo);
    
    LoadingScreen *getLoadingScreen();

    void tick();
};

#endif
