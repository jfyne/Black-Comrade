#ifndef NETWORKINGMANAGER_H
#define NETWORKINGMANAGER_H

#include "ITickable.h"
#include "IExit.h"

// raknet required includes
#include "StringTable.h"
#include "RakPeerInterface.h"
#include "RakNetworkFactory.h"
#include <stdio.h>
#include "Kbhit.h"
#include <string.h>
#include "BitStream.h"
#include "MessageIdentifiers.h"
#include "ReplicaManager3.h"
#include "NetworkIDManager.h"
#include "networkRole.h"
#include "RakSleep.h"
#include "FormatString.h"
#include "RakString.h"
#include "GetTime.h"
#include "SocketLayer.h"

#include "DiscoveryAgent.h"

#include "ReplicaObject.h"
#include "shipState.h"
#include "ReplicaConnection.h"
#include "OurReplicaManager.h"

#include <string>
#include <cstdio>

using namespace std;
using namespace RakNet;

class NetworkingManager : public ITickable {

private:
        DiscoveryAgent *discoveryAgent;

        Packet *packet;
        SocketDescriptor sd;
        static const int SERVER_PORT=6001;
        NetworkIDManager networkIdManager;
        bool isServer;
        RakPeerInterface *rakPeer;
        string serverAddress;

        IExit *mExit;

public:

        NetworkingManager(IExit *mExit);
        ~NetworkingManager();

        OurReplicaManager replicaManager;
        virtual void tick();
        NetworkRole startNetworking(NetworkRole desiredRole);
        void stopNetworking();

        bool replicate(ReplicaObject *object);
        ReplicaObject *getReplica(string name, bool blocking);
        ReplicaObject *getReplica(int index, bool blocking);
};


#endif