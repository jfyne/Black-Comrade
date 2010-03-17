#include "networkingManager.h"
#include <iostream>  // I/O 
#include "networkRole.h"
#include "collaborationInfo.h"
#include "gameRole.h"
#include <vector>
#include <stdio.h>
#include "lobby.h"
#include "const.h"

using namespace std;
using namespace RakNet;

#define SLEEP(arg)  ( usleep( (arg) *1000 ) )

NetworkingManager::NetworkingManager(IExit *mExit) :
    serverAddress(""),
    numConnections(0),
    chosenGameRole(NO_GAME_ROLE),
    mExit(mExit),
    inLobby(true)
{
    sd.port = 0;
    replicaManager.SetDefaultPacketReliability(RELIABLE_ORDERED);
    replicaManager.SetAutoSerializeInterval(1);

    collabInfo = new CollaborationInfo("",NO_NETWORK_ROLE,NO_GAME_ROLE);
}

NetworkingManager::~NetworkingManager() {
    cout << "...packet" << endl;
    delete packet;
    cout << "...discoveryagent" << endl;
    delete discoveryAgent;
    cout << "...lobby" << endl;
    delete lobby;
}

void NetworkingManager::tick() {
    bool quit = false;

    if (inLobby) {
        runLobby();
        return;
    }

    for (packet = rakPeer->Receive(); packet; rakPeer->DeallocatePacket(packet), packet = rakPeer->Receive()) {
            switch (packet->data[0]) {
                case ID_CONNECTION_ATTEMPT_FAILED:
                    printf("ID_CONNECTION_ATTEMPT_FAILED\n");
                    quit = true;
                    break;
                case ID_DISCONNECTION_NOTIFICATION:                    printf("You have been disconnected.\n");
                    quit = true;
                    break;                case ID_CONNECTION_LOST:
                    printf("You have been disconnected.\n");
                    quit = true;
                    break;
            }
    }
    if (quit) mExit->exit();
    replicaManager.doUpdate();
}

NetworkRole NetworkingManager::determineRole(NetworkRole desiredRole) {
    if (desiredRole == SERVER) {
        discoveryAgent->beServer();
        return SERVER;
    }
    else if (desiredRole == CLIENT) {
        serverAddress = discoveryAgent->findServer(6001,6000,1);
        if (serverAddress.compare("") == 0) {
            printf("No servers could be found. You are now the server.\n");
            return SERVER;
        }
        return CLIENT;
    }
    else if (desiredRole == DEVELOPMENTSERVER) { return DEVELOPMENTSERVER; }
}

bool NetworkingManager::startNetworking(NetworkRole desiredRole) {
    NetworkRole actualRole = NO_NETWORK_ROLE;
    discoveryAgent = new DiscoveryAgent();
    actualRole = determineRole(desiredRole);

    networkRole = actualRole;

    rakPeer = RakNetworkFactory::GetRakPeerInterface();

    rakPeer->SetNetworkIDManager(&networkIdManager);
    networkIdManager.SetIsNetworkIDAuthority((actualRole == SERVER || actualRole == DEVELOPMENTSERVER));
    
    if (actualRole == SERVER) sd.port = Const::SERVER_PORT;

    rakPeer->Startup(3,100,&sd,1);
    rakPeer->AttachPlugin(&replicaManager);
    rakPeer->SetMaximumIncomingConnections(3);

    lobby = new Lobby(rakPeer, discoveryAgent, networkRole);

    if (actualRole == CLIENT) {
        return lobby->connect(serverAddress, Const::SERVER_PORT);
    }

    return true;
}

bool NetworkingManager::hostGame(bool development) {
    NetworkRole actualRole = NO_NETWORK_ROLE;
    discoveryAgent = new DiscoveryAgent();
    if (development)
        networkRole = DEVELOPMENTSERVER;
    else
        networkRole = SERVER;

    rakPeer = RakNetworkFactory::GetRakPeerInterface();

    rakPeer->SetNetworkIDManager(&networkIdManager);
    networkIdManager.SetIsNetworkIDAuthority((networkRole == SERVER || networkRole == DEVELOPMENTSERVER));
    
    if (actualRole == SERVER) sd.port = Const::SERVER_PORT;

    rakPeer->Startup(3,100,&sd,1);
    rakPeer->AttachPlugin(&replicaManager);
    rakPeer->SetMaximumIncomingConnections(3);

    lobby = new Lobby(rakPeer, discoveryAgent, networkRole);
    return true;
}

std::vector<string> NetworkingManager::findGames() {
    std::vector<string> servers = discoveryAgent->findServers(6001,6000,1);
    for(std::vector<string>::const_iterator it=servers.begin();it!=servers.end(); ++it) {
        std::cout << *it << std::endl;
        serverAddress = *it;
    }
    return servers;
}

bool NetworkingManager::connectToGame(int id) {
    lobby = new Lobby(rakPeer, discoveryAgent, CLIENT);
    return lobby->connect(serverAddress, Const::SERVER_PORT);
}

void NetworkingManager::runLobby() {
    if (!lobby->wait()) return;
    
    if (networkRole == SERVER) {
        startGame();
        discoveryAgent->destroyServer();
    }
    chosenGameRole = lobby->getChosenGameRole();
    if (chosenGameRole == NO_GAME_ROLE) chosenGameRole = PILOT;
    collabInfo = new CollaborationInfo(lobby->getChosenNick(), networkRole, chosenGameRole);
    inLobby = false;
}

void NetworkingManager::startGame() {
    RakNet::BitStream dataStream;
    
    dataStream.Write(START_GAME);
    rakPeer->Send(&dataStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
}

void NetworkingManager::stopNetworking() {
    rakPeer->Shutdown(100,0);
    RakNetworkFactory::DestroyRakPeerInterface(rakPeer);
}

bool NetworkingManager::replicate(ReplicaObject *object) {
    replicaManager.Reference(object);
    replicaManager.doUpdate();
    return true;
}

ReplicaObject* NetworkingManager::getReplica(string name, bool blocking) {
    DataStructures::Multilist<ML_STACK, Replica3*> replicaList; 
    DataStructures::DefaultIndexType index;
    replicaManager.GetReferencedReplicaList(replicaList);

    while (true) {
        try {
            for (index=0; index < replicaList.GetSize(); index++) {
                ReplicaObject * temp = ((ReplicaObject *) replicaList[index]);
                if (temp->GetName().StrCmp(RakNet::RakString(name.c_str())) == 0) return temp;
            }
        }
        catch (...) {

        }
        if (!blocking) return 0;
        this->tick();
        replicaManager.GetReferencedReplicaList(replicaList);
    }
}

ReplicaObject* NetworkingManager::getReplica(int index, bool blocking) {
    if (blocking) {
        while (replicaManager.GetReplicaCount() == 0) {
            this->tick();
        }
    }
    try {
        return (ReplicaObject *) replicaManager.GetReplicaAtIndex(index);
    }
    catch (...) {
        return 0;
    }
}

std::vector<ReplicaObject*> NetworkingManager::getReplicas(string name) {
    DataStructures::Multilist<ML_STACK, Replica3*> replicaList;
    DataStructures::DefaultIndexType index;
    replicaManager.GetReferencedReplicaList(replicaList);

    std::vector<ReplicaObject*> replicas = std::vector<ReplicaObject*>();
    try {
        for (index=0; index < replicaList.GetSize(); index++) {
            ReplicaObject * temp = ((ReplicaObject *) replicaList[index]);
            if (temp->GetName().StrCmp(RakNet::RakString(name.c_str())) == 0) replicas.push_back(temp);
        }
    }
    catch (...) {}
    return replicas;
}

