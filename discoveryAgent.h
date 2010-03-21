#ifndef DISCOVERYAGENT_H
#define DISCOVERYAGENT_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "RakPeerInterface.h"

using namespace RakNet;
using namespace std;

class DiscoveryAgent {
  private:
    RakPeerInterface *server;
    RakPeerInterface *searchClient;
    bool createServer(int port);
    int numConnections;

    bool alreadyListed(std::string address);

    std::vector<string> servers;
    
  public:
    DiscoveryAgent();

    void beServer();
    void destroyServer();

    void destroyClient();

    void startServerListUpdate(int serverPort);
    void updateServerList();
    std::vector<string> getServerList();
};

#endif
