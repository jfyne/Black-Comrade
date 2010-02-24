
default: all

include Makefile.local

CC = g++ -g
CFLAGS = 

OUTPUT = test

SRCS = main.cpp stateUpdate.cpp shipState.cpp accelerationState.cpp motionState.cpp inputState.cpp pilotControls.cpp gunState.cpp audioState.cpp replicaConnection.cpp replicaObject.cpp ourReplicaManager.cpp networkingManager.cpp discoveryAgent.cpp mapManager.cpp rayQuery.cpp soundManager.cpp miniGameManager.cpp testMiniGame.cpp rect2D.cpp collaborationInfo.cpp lobby.cpp bullet.cpp bulletManager.cpp collisionDetection.cpp collisionManager.cpp collision.cpp gameStateMachine.cpp gameParameterMap.cpp waypoint.cpp printState.cpp enemy.cpp swarm.cpp swarmManager.cpp flying.cpp mapTile.cpp navigatorControls.cpp engineerControls.cpp lines.cpp DynamicLines.cpp DynamicRenderable.cpp sceneNodeManager.cpp guiManager.cpp guiStatusUpdater.cpp damageState.cpp constManager.cpp pathFinder.cpp pathTile.cpp notificationManager.cpp systemManager.cpp notification.cpp radarGui.cpp console.cpp


OBJS = $(SRCS:.cpp=.o)


all: $(OUTPUT)
	@echo Finished!
	
$(OUTPUT) : $(OBJS)
	$(CC) -o $(OUTPUT) $(OBJS) $(LFLAGS) $(LIBS)
	
.cpp.o:
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDES)
	
run:
	export LD_LIBRARY_PATH="${OGRE_PATH}/lib/:${FMOD_PATH}/lib/"; ./$(OUTPUT)
	
debug:
	export LD_LIBRARY_PATH="${OGRE_PATH}/lib/:${FMOD_PATH}/lib/"; gdb ./$(OUTPUT)

.PHONY: clean depend

clean:
	rm *.o $(OUTPUT)
	
depend: $(SRCS)
	makedepend -f Makefile.local $^
# DO NOT DELETE
