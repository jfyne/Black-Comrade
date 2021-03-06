#ifndef LOBBY_H
#define LOBBY_H

// raknet required includes
#include "StringTable.h"
#include "RakPeerInterface.h"
#include "RakNetworkFactory.h"
#include <stdio.h>
#include "BitStream.h"
#include "MessageIdentifiers.h"
#include "ReplicaManager3.h"
#include "NetworkIDManager.h"
#include "RakSleep.h"
#include "FormatString.h"
#include "RakString.h"
#include "GetTime.h"
#include "SocketLayer.h"

#include "discoveryAgent.h"
#include "networkRole.h"
#include "gameRole.h"

#include <iostream>
#include <string>
#include <cstdio>

const unsigned char START_GAME = 100;
const unsigned char GAME_ROLE_CHOICES = 101;
const unsigned char GAME_ROLE_CHOICE = 102;
const unsigned char GAME_INFO_REQUEST = 103;
const unsigned char GAME_INFO = 104;
const unsigned char END_GAME = 222;

using namespace std;
using namespace RakNet;

class Lobby {
    private:
    RakPeerInterface *rakPeer;
    Packet *packet;
    DiscoveryAgent *discoveryAgent;

    GameRole chosenGameRole;
    GameRole gameRole;
    NetworkRole networkRole;
    void checkForRoleChoice();

    void process();

    void sendGameRoleChoice(GameRole chosenRole);
    void offerGameRoleChoices();
    void sendGameRoleChoices(SystemAddress recipient);
    void sendGameRoleChoices();

    void sendGameInfo(SystemAddress recipient);

    string nick;
    string gameName;

    public:
    Lobby(RakPeerInterface *rp, DiscoveryAgent *da, NetworkRole nr, std::string nick);
    Lobby(RakPeerInterface *rp, DiscoveryAgent *da, NetworkRole nr, std::string nick, std::string gameName);

    bool roleOptionsChanged;
    bool pilotTaken;
    bool navTaken;
    bool engTaken;

    void enter();
    bool wait();
    void chooseGameRole(GameRole role);

    bool hasChosenRole();

    bool connect(string address, int port);
    GameRole getChosenGameRole();
    string getChosenNick();
};

#endif
