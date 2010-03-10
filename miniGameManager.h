
#ifndef MINI_GAME_MANAGER_H
#define MINI_GAME_MANAGER_H

// Includes all Ogre classes
#include <OGRE/Ogre.h>

#include "ITickable.h"
#include "inputState.h"
#include "IPlayerControls.h"
#include "IMiniGame.h"
#include "IConsoleInputReceiver.h"
#include "console.h"
#include "consoleShell.h"
#include "quickTimeMiniGame.h"
#include "collaborationInfo.h"
#include "IExit.h"

using namespace Ogre;
using namespace std;

class MiniGameManager : public ITickable, public OIS::KeyListener {
private:
    IPlayerControls *playerControls;
    InputState *inputState;
    IConsoleInputReceiver *inputReceiver;
    SceneManager *sceneManager;
    Console *console;
    ConsoleShell *consoleShell;
    CollaborationInfo *player;
    IExit *exit;
    
    IMiniGame *currentMiniGame;

    void setConsoleState(bool isOpen);
    
public:
    bool aKeyPressed;

    void tick();
    
    MiniGameManager(Console *console,
                    InputState *inputState, IPlayerControls *playerControls,
                    SceneManager *sceneManager, CollaborationInfo *player,
                    IExit *exit);

    bool keyPressed(const OIS::KeyEvent &arg);
    bool keyReleased(const OIS::KeyEvent &arg);
};

#endif
