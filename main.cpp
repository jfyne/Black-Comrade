
#include "main.h"
#include <iostream>
#include "stateUpdate.h"

#include "networkRole.h"
#include "collaborationInfo.h"
#include "networkingManager.h"

using namespace RakNet;

Main::Main() {

    // networking
    networkingManager = new NetworkingManager(this);
    collabInfo = runLoby(networkingManager);

	// Start Ogre
    root = configRoot();
    sceneMgr = root->createSceneManager(ST_GENERIC);
    window = root->initialise(true, collabInfo->getGameRoleString());
    
    configResources();
    
    // Ship Node
    shipSceneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    Entity *shipEntity = sceneMgr->createEntity("ourship", "ourship.mesh");
    shipSceneNode->attachObject(shipEntity);
    if(collabInfo->getGameRole() == PILOT) {
    	shipEntity->setVisible(false);
    }
    
    // Camera
    camera = createCamera(shipSceneNode);
    if(collabInfo->getGameRole() == PILOT) {
        camera->setPosition(Vector3(0,0,5));
    } else if(collabInfo->getGameRole() == NAVIGATOR) {
        camera->setPosition(Vector3(3.5,0,0));
    } else if(collabInfo->getGameRole() == ENGINEER) {
        camera->setPosition(Vector3(-3.5,0,0));
    }
    createViewPort();
    
    // Create Map
    mapMgr = new MapManager("examplemap.txt",sceneMgr);
    mapMgr->outputMap(sceneMgr->getRootSceneNode());
    
    // Collision Manager
	collisionMgr = new CollisionManager(sceneMgr,mapMgr);
    
    // Game Loop
    gameLoop = new StateUpdate();
    root->addFrameListener(gameLoop);
    
    // User Input
    inputState = new InputState(window, false, this,true,true);
    gameLoop->addTickable(inputState);
    
    // Pilot --- Flying 1.0 ---
    // if(collabInfo->getGameRole() == PILOT) {
	    // pilotControls = new PilotControls(inputState,camera);
	    // accelerationState = new AccelerationState(pilotControls);
	    // motionState = new MotionState(accelerationState);
	    // gameLoop->addTickable(pilotControls);
	    // gameLoop->addTickable(accelerationState);
	    // gameLoop->addTickable(motionState);
    // }
    
    // pilot new Flying
    if(collabInfo->getGameRole() == PILOT) {
        collisionMgr->addMesh(shipEntity);
        pilotControls = new PilotControls(inputState,camera);
        flying = new Flying( pilotControls, collisionMgr );
        gameLoop->addTickable(pilotControls);
        gameLoop->addTickable(flying);
    }
    
    // Navigator Controls
    if(collabInfo->getGameRole() == NAVIGATOR) {
	    navigatorControls = new NavigatorControls(inputState,camera);
	    gameLoop->addTickable(navigatorControls);
    }
    
    // Engineer Controls
    if(collabInfo->getGameRole() == ENGINEER) {
	    engineerControls = new EngineerControls(inputState,camera);
	    gameLoop->addTickable(engineerControls);
    }
    
    // Ship State
    if(collabInfo->getGameRole() == PILOT) {
	    shipState = new ShipState(shipSceneNode,flying);
	    networkingManager->replicate(shipState);
    } else {
    	shipState = 
    		(ShipState*) networkingManager->getReplica("ShipState",true);
    	shipState->shipSceneNode = shipSceneNode;
    }
    shipState->position = new Vector3(mapMgr->startx,0,mapMgr->starty);
    gameLoop->addTickable(shipState);

	// GameState
	if(collabInfo->getGameRole() == PILOT) {
	    gameStateMachine = new GameStateMachine(mapMgr,shipState);
	    networkingManager->replicate(gameStateMachine);
	    
    } else {
    	gameStateMachine = 
    		(GameStateMachine*) networkingManager->
    			getReplica("GameStateMachine",true);
    }
	gameLoop->addTickable(gameStateMachine);
	gameParameterMap = new GameParameterMap(gameStateMachine);

	// Print Game State changes
	printState = new PrintState(gameStateMachine);
	gameLoop->addTickable(printState);

	// Pilot Gun State
	if(collabInfo->getGameRole() == PILOT) {
	    pilotGunState = new GunState(pilotControls,collabInfo->getGameRole());
	    networkingManager->replicate(pilotGunState);
	} else {
    	pilotGunState = (GunState*) networkingManager->
    		getReplica("PilotGunState",true);
    }
    gameLoop->addTickable(pilotGunState);
    
    // Navigator Gun State
	if(collabInfo->getGameRole() == NAVIGATOR) {
	    navigatorGunState = new GunState(navigatorControls,collabInfo->getGameRole());
	    networkingManager->replicate(navigatorGunState);
	    gameLoop->addTickable(navigatorGunState);
	} else {
    	if (collabInfo->getNetworkRole() != DEVELOPMENTSERVER) {
    	    navigatorGunState = (GunState*) networkingManager->
    	        getReplica("NavigatorGunState",true);
    	        std::cout << "Got nav gun from net" << std::endl;
    	    gameLoop->addTickable(navigatorGunState);
    	}
    }
    
    // Engineer Gun State
	if(collabInfo->getGameRole() == ENGINEER) {
	    engineerGunState = new GunState(engineerControls,collabInfo->getGameRole());
	    networkingManager->replicate(engineerGunState);
	    gameLoop->addTickable(engineerGunState);
	} else {
    	if (collabInfo->getNetworkRole() != DEVELOPMENTSERVER) {
    	    engineerGunState = (GunState*) networkingManager->
    		    getReplica("EngineerGunState",true);
    		    std::cout << "Got eng gun from net" << std::endl;
    		gameLoop->addTickable(engineerGunState);
        }
    }

	// TODO: start the enemies pointing towards the ship?
	// Swarm Manager
	swarmMgr = new SwarmManager(sceneMgr, gameParameterMap, mapMgr,
		shipState,collisionMgr);
	gameLoop->addTickable(swarmMgr);

        gameLoop->addTickable(networkingManager);

	// Bullet Manager
	//if(collabInfo->getGameRole() == PILOT) {
	    bulletMgr = new BulletManager(shipSceneNode,sceneMgr,pilotGunState,
	    	engineerGunState,navigatorGunState,collisionMgr,swarmMgr);
	    //networkingManager->replicate(bulletMgr);
	    gameLoop->addTickable(bulletMgr);
    //} else {
    	//bulletMgr = 
    	//	(BulletManager*) networkingManager->getReplica("BulletManager",true);
    //}

	// Audio
	//soundMgr = new SoundManager();
	//gameLoop->addTickable(soundMgr);
	//audioState = new AudioState(gunState,soundMgr,shipSceneNode);
	//gameLoop->addTickable(audioState);

	// Last class to be added to the game loop
    
    // Start Rendering Loop
    root->startRendering();
    networkingManager->stopNetworking();
}

Root *Main::configRoot()
{
	Root *root = new Root();

    if (!root->restoreConfig())
        root->showConfigDialog();
        
    return root;
}

void Main::configResources()
{
	ResourceGroupManager::getSingleton().addResourceLocation(
                    ".", "FileSystem", "General");

    // Set the paths to look for various resources
    ResourceGroupManager::getSingleton().addResourceLocation(
                    "models", "FileSystem", "General");

    ResourceGroupManager::getSingleton().addResourceLocation(
                    "sounds", "FileSystem", "Genescenemral");

    ResourceGroupManager::getSingleton().addResourceLocation(
                    "materials/scripts", "FileSystem", "General");

    ResourceGroupManager::getSingleton().addResourceLocation(
                    "materials/programs", "FileSystem", "General");

    ResourceGroupManager::getSingleton().addResourceLocation(
                    "materials/textures", "FileSystem", "General");
                    
    ResourceGroupManager::getSingleton().addResourceLocation(
                    "particles", "FileSystem", "General"); 
                   
    // Magic Resource line
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

CollaborationInfo *Main::runLoby(NetworkingManager *networkingManager) {
    
    CollaborationInfo *collabInfo;

    char ch;
    printf("Start as (c)lient, (s)erver or (d)evelopment server?\n");

    while(true) {
        ch=getch();
        if (ch=='c' || ch=='C')
        {
            collabInfo = networkingManager->startNetworking(CLIENT);
            break;
        }
        else if (ch=='d' || ch=='D')
        {
            collabInfo = networkingManager->startNetworking(DEVELOPMENTSERVER);
            printf("DEVELOPMENT SERVER\n");
            break;
        }
        else if (ch=='s' || ch=='S')
        {
            collabInfo = networkingManager->startNetworking(SERVER);
            break;
        }
    }
    
    return collabInfo;
}

Camera *Main::createCamera(SceneNode *shipSceneNode) {

    Camera *camera = sceneMgr->createCamera("mainCam");

    shipSceneNode->attachObject(camera);

    camera->setPosition(Vector3(0,0,0));
    camera->lookAt(Vector3(0,0,1));
    //camera->setFOVy(Radian(2.0943951));
    camera->setNearClipDistance(1);
    camera->setFarClipDistance(1500);
    
    // Lighting
    //sceneMgr->setShadowColour(ColourValue(0.5,0.5,0.5));

    //sceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);
    
    // Add some sexy fog
    //ColourValue fadeColour(0.1,0.1,0.1);
    //sceneMgr->setFog(FOG_LINEAR, fadeColour, 0.0, 0, 300);
    
    Light *sp = sceneMgr->createLight("ShipLight");
    sp->setType(Light::LT_POINT);
    sp->setDiffuseColour(1.0,1.0,1.0);
    sp->setSpecularColour(0.2,0.2,0.7);
    sp->setDirection(Vector3(0,0,1));
    //sp->setAttenuation(10000, 0.7, 0.000025, 0.0000045);

    shipSceneNode->attachObject(sp);
    
    return camera;
}

void Main::createViewPort() {

    Viewport *vp = window->addViewport(camera);
    vp->setBackgroundColour(ColourValue(1,0,0));
    
    camera->setAspectRatio(
        Real(vp->getActualWidth()) / Real(vp->getActualHeight()*1.17));
    //camera->setAspectRatio(1.17);
}

int main()
{
    Main *main = new Main();

    delete main;
}

Main::~Main()
{
}

void Main::exit()
{
    gameLoop->running = false;
}

