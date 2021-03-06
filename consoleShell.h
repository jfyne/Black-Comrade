#ifndef CONSOLE_SHELL_H
#define CONSOLE_SHELL_H

#include <iostream>
#include <string>
#include <vector>
#include "IConsoleInputReceiver.h"
#include "IMiniGame.h"
#include "ITickable.h"
#include "console.h"
#include "inputState.h"
#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>
#include <boost/algorithm/string/trim.hpp>

#include "quickTimeMiniGame.h"
#include "magicWordMiniGame.h"
#include "buttonMashMiniGame.h"
#include "testMiniGame.h"
#include "fixMiniGame.h"
#include "sequenceMiniGame.h"
#include "sheildMiniGame.h"
#include "sensorMiniGame.h"

#include "IExit.h"

class ConsoleShell : public IConsoleInputReceiver
{
    private:
        InputState *inputState;
        Console *console;
        IExit *exit;

        IMiniGame *gameToPlay;

        std::string defaultPrompt;
        std::string fullCommand;
        std::string command;
        std::vector<std::string> commands;
        int commandIndex;

        int difficulty;
        std::map <std::string,int> *difficulties;

        void historyBack();
        void historyForward();
        void showCommand(int index);

        void processCommand();
        void tabComplete();
        int getDifficulty(std::string name);
        
    public:
        void tick();
        IMiniGame* getGameToPlay();

        void alphaNumKeyPressed (const OIS::KeyEvent &arg);
        void backspaceKeyPressed();
        void returnKeyPressed();
        void otherKeyPressed (const OIS::KeyEvent &arg);

        void showPrompt();
        
        // Null unless a command has been entered
        std::string getCommand();
    
        ConsoleShell(Console *console, InputState *inputState,
                     std::map <std::string,int> *difficulties,
                     IExit *exit);
        ~ConsoleShell();
};

#endif
