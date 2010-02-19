
#ifndef NOTIFICATION_MANAGER_H
#define NOTIFICATION_MANAGER_H

#include "ITickable.h"
#include "gameStateMachine.h"
#include "shipState.h"
#include "mapManager.h"
#include "damageState.h"
#include "constManager.h"
#include <vector>
#include <limits>
#include <cstdlib>
#include <string>
#include <sstream>

#include "replicaObject.h"
#include "collaborationInfo.h"

#include "notificationType.h"
#include "notification.h"

class NotificationManager : public ITickable, public ReplicaObject
{
private:
    NotificationType nextType;
    Notification *notification;
    Notification *lastNotification;

    std::vector<Notification> queue;
    std::map <NotificationType,int> recency;

    CollaborationInfo *collabInfo;
    GameStateMachine *stateMachine;
    GameState lastStateNotified;
    MapManager *mapManager;
    ShipState *shipState;
    DamageState *damageState;
    bool mIsNewNotification;

    int tickcount;
    int maxDelay;

    bool controlsDisplayed;
    bool stateChangePending;

    void checkComments();
    void checkGameState();
    void checkShipPosition();
    void checkHealth();

    bool isTimely(NotificationType notification, int delaySinceMe, int delaySinceLast);
    int getTimeSince(NotificationType notification);
    int getTimeSinceLast();
    void updateRecencies();
    void prepareNotification();
	
public:
    NotificationManager();
    NotificationManager(CollaborationInfo *collabInfo, GameStateMachine *stateMachine,
                        MapManager *mapManager, ShipState *shipState,
                        DamageState *damageState);
    void tick();
	
    Notification *getCurrentNotification();
    bool hasNewNotification();

    void setCollaborationInfo(CollaborationInfo *collabInfo);

    virtual RakNet::RakString GetName(void) const;
    virtual RM3SerializationResult Serialize(SerializeParameters *serializeParameters);
    virtual void Deserialize(RakNet::DeserializeParameters *deserializeParameters);
};

#endif