//-----------------------------------------------------------------------------
#include "foundation/ptr.h"

#include "demoapp/demogamestate.h"

#include "managers/factorymanager.h"

#include "input/ninputserver.h"
#include "video/nvideoserver.h"
#include "particle/nparticleserver.h"
#include "particle/nparticleserver2.h"

#include "properties/graphicsproperty.h"
#include "properties/lightproperty.h"
#include "properties/transformableproperty.h"
#include "properties/environmentgraphicsproperty.h"
#include "properties/environmentcollideproperty.h"
#include "properties/physicsproperty.h"
#include "properties/chasecameraproperty.h"

//#include "physics/materialtable.h"
#include "physics/level.h"
#include "physics/meshshape.h"
#include "physics/boxshape.h"

#include "util/segmentedgfxutil.h"

// #include "level/stage.h"
// #include "level/view.h"

//-----------------------------------------------------------------------------
/**
*/
DemoGameState::DemoGameState()
{
}

//-----------------------------------------------------------------------------
/**
*/
DemoGameState::~DemoGameState()
{
}

//-----------------------------------------------------------------------------
/**
*/
void 
DemoGameState::OnStateEnter(const nString& prevState)
{
    GameStateHandler::OnStateEnter(prevState);

    //TODO: Load FSM

    // create an default light entity
    this->CreateDefaultLight();

// begin start to load level
	this->CreateLevel();
/*
	nString exportPath = nFileServer2::Instance()->GetAssign(nString("export"));
	nString mapPath;
	mapPath = exportPath + "base" + "/";

	Graphics::Stage* stage = Graphics::Stage::Create();
	stage->Open(mapPath, nString("game/admin"));

	// specify camera
	Graphics::CameraEntity* camera = Graphics::Server::Instance()->GetCamera();
	matrix44 tm = camera->GetView();//camera->GetTransform();
	tm.set_translation(stage->GetInitPos());
	camera->SetTransform(tm);

	Graphics::View* view = Graphics::View::Create();
	view->Open();

	Graphics::Level* level = Graphics::Server::Instance()->GetLevel();
	level->SetView(view);
*/
// end start to load level

    // create a camera entity
    //this->CreateCamera();

	// create environment such as ground etc.
	this->CreateEnvironment();

    // create an player entity
    this->CreatePlayer();
	this->CreateBox();

	Ptr<Input::Server> inputServer = Input::Server::Instance();
	inputServer->AddMapping("moveForward", "keyb0:w");
	inputServer->AddMapping("moveBackward", "keyb0:s");
	inputServer->AddMapping("moveLeft", "keyb0:a");
	inputServer->AddMapping("moveRight", "keyb0:d");
	inputServer->AddMapping("jump", "keyb0:space");
}

//-----------------------------------------------------------------------------
/**
*/
void 
DemoGameState::OnStateLeave(const nString& nextState)
{
    //TODO: put state leave code 

// 	Graphics::Stage* stage = Graphics::Stage::Instance();
// 	stage = 0;
	
 
    GameStateHandler::OnStateLeave(nextState);
}

//-----------------------------------------------------------------------------
/**
*/
nString 
DemoGameState::OnFrame()
{
	/*
    bool running = true;

    // toggle visualizations
    if (nInputServer::Instance()->GetButton("togglePhysicsVisualization"))
    {
        this->physicsVisualizationEnabled = !this->physicsVisualizationEnabled;
    }
    if (nInputServer::Instance()->GetButton("toggleGraphicsVisualization"))
    {
        this->graphicsVisualizationEnabled = !this->graphicsVisualizationEnabled;
    }
    if (nInputServer::Instance()->GetButton("toggleFOVVisualization"))
    {
        this->fovVisualization = !this->fovVisualization;
    }

    // trigger subsystem and Nebula servers
    nVideoServer::Instance()->Trigger();
    Input::Server::Instance()->Trigger();
    running &= Foundation::Server::Instance()->GetScriptServer()->Trigger();

    // trigger the audio and game subsystems
    Audio::Server::Instance()->BeginScene();
    Game::Server::Instance()->OnFrame();
    Audio::Server::Instance()->EndScene();
    VFX::Server::Instance()->BeginScene();
    nParticleServer::Instance()->Trigger();
    nParticleServer2::Instance()->Trigger();
    running &= Graphics::Server::Instance()->Trigger();

    if (Graphics::Server::Instance()->BeginRender())
    {
        UI::Server::Instance()->Render();
        Graphics::Server::Instance()->Render();

        if (this->graphicsVisualizationEnabled)
        {
            Graphics::Server::Instance()->RenderDebug();
        }
        if (this->physicsVisualizationEnabled)
        {
            Physics::Server::Instance()->RenderDebug();
        }
        if (this->fovVisualization)
        {
            Navigation::Server::Instance()->RenderDebug();
        }
        Graphics::Server::Instance()->EndRender();
    }
    VFX::Server::Instance()->EndScene();

    // trigger kernel server
    nKernelServer::Instance()->Trigger();

    if (!running)
    {
        return this->exitState;
    }
    else
    {
        return this->GetName();
    }
	*/
	return GameStateHandler::OnFrame();
}


//-----------------------------------------------------------------------------
/**
*/
bool DemoGameState::CreateDefaultLight()
{
   Managers::FactoryManager *factoryManager = Managers::FactoryManager::Instance();
   Ptr<Game::Entity> light = factoryManager->CreateEntityByClassName("Entity");
   Ptr<Game::Property> lp = factoryManager->CreateProperty("LightProperty");
   light->AttachProperty(lp);

   matrix44 mx;
   mx.rotate_z(N_PI / 8);
   mx.rotate_x(-N_PI / 3);
   mx.translate(vector3(0, 100, 0));
   light->SetMatrix44(Attr::Transform, mx);
   light->SetString(Attr::LightType, "Directional");
   light->SetVector4(Attr::LightColor, vector4(0.8f, 0.8f, 0.9f, 0.7f));
   //light->SetVector3(Attr::LightPos, vector3(45.0f, 45.0f, 45.0f));
   //light->SetVector4(Attr::LightRot, vector4(0.0f, 0.0f, 0.0f, 1.0f));
   light->SetVector4(Attr::LightAmbient, vector4(0.2f, 0.2f, 0.1f, 0.0f));
   light->SetBool(Attr::LightCastShadows, true);
   Managers::EntityManager::Instance()->AttachEntity(light);
   
   nCamera2& camera =  nGfxServer2::Instance()->GetCamera();
   
   float l,r,t,b,n,f;
   camera.GetViewVolume(l, r, t, b, n, f);
   
   return true;
}

//-----------------------------------------------------------------------------
/**
*/
bool DemoGameState::CreateCamera()
{
    Managers::FactoryManager *factoryManager = Managers::FactoryManager::Instance();
    Ptr<Game::Entity> camera = factoryManager->CreateEntityByClassName("Entity");

	Ptr<Game::Property> vc = factoryManager->CreateProperty("VideoCameraProperty");
    camera->AttachProperty(vc);

	Ptr<Game::Property> ip = factoryManager->CreateProperty("InputProperty");
    camera->AttachProperty(ip);

	Ptr<Game::Property> pp = factoryManager->CreateProperty("PathAnimProperty");
    camera->AttachProperty(pp);

    matrix44 tm;
    vector3 pos;

    //pos = Graphics::Stage::Instance()->GetInitPos();
    tm.set_translation(pos);

	camera->SetMatrix44(Attr::Transform, tm);

    Managers::EntityManager::Instance()->AttachEntity(camera);
    Managers::FocusManager::Instance()->SetFocusEntity(camera);

    return true;
}

//-----------------------------------------------------------------------------
/**
*/
bool DemoGameState::CreatePlayer()
{
    Managers::FactoryManager *factoryManager = Managers::FactoryManager::Instance();
    this->player = factoryManager->CreateEntityByClassName("Entity");

    // specify physics entity
    //Ptr<Game::Property> pp = factoryManager->CreateProperty("PlayerPhysicsProperty");
	Ptr<Game::Property> pp = factoryManager->CreateProperty("ActorPhysicsProperty2");
    this->player->AttachProperty(pp);

    // specify graphic entity
    Ptr<Game::Property> gp = factoryManager->CreateProperty("ActorGraphicsProperty");
    this->player->AttachProperty(gp);
	
    //this->player->SetString(Attr::Graphics, "player/player03");
	//this->player->SetString(Attr::Graphics, "player/player");
	//this->player->SetString(Attr::Graphics, "player/myplayer");
 	//this->player->SetString(Attr::Graphics, "player/vicecity_player_custom02");
	
	//this->player->SetString(Attr::Graphics, "static/box01");
	//this->player->SetString(Attr::AnimSet, "Player");

	//this->player->SetString(Attr::Graphics, "player/mc01_merge04");
  	this->player->SetString(Attr::Graphics, "player/mc01_merge04_rotated");
  	this->player->SetString(Attr::AnimSet, "Player");

     Ptr<Game::Property> ip = factoryManager->CreateProperty("PlayerInputProperty");
     this->player->AttachProperty(ip);
 
 	Ptr<Game::Property> ap = factoryManager->CreateProperty("PlayerAnimationProperty");
 	this->player->AttachProperty(ap);

     Ptr<Game::Property> cp = factoryManager->CreateProperty("ChaseCameraProperty");
     this->player->AttachProperty(cp);
 
 	// Physics에서 simulation후 변경된 변환을 보내면 관련된 GraphicsProperty등에서는 해당 
 	// 메시지를 받아서 업데이트 시킨다.
 	Ptr<Game::Property> tp = factoryManager->CreateProperty("TransformableProperty");

    // begin hack
    matrix44 tm;
    //vector3 pos;

    //pos = Graphics::Stage::Instance()->GetInitPos();
    //tm.set_translation(pos);
    //end hack

	//tm.set_translation(vector3(50.0f, 0.0f, 0.0f));

 	this->player->SetMatrix44(Attr::Transform, tm);
 	this->player->AttachProperty(tp);
 
 	this->player->SetFloat(Attr::CameraDistance, 3.0f);
 	this->player->SetFloat(Attr::CameraDefaultTheta, n_deg2rad(-5.0f));

	Managers::FocusManager::Instance()->SetFocusEntity(this->player);
    Managers::EntityManager::Instance()->AttachEntity(this->player);

    return true;
}

//////////////////////////////////////////////////////////////////////////
// bool CreateBox()
//////////////////////////////////////////////////////////////////////////
bool DemoGameState::CreateBox()
{
	Ptr<Game::Entity> entity = Managers::FactoryManager::Instance()->CreateEntityByClassName("Entity");
	Ptr<Properties::PhysicsProperty> physicsProperty = (Properties::PhysicsProperty*) Managers::FactoryManager::Instance()->CreateProperty("PhysicsProperty");
	Ptr<Game::Property> graphicProperty = Managers::FactoryManager::Instance()->CreateProperty("GraphicsProperty");

	entity->AttachProperty(physicsProperty);
	entity->SetString(Attr::Physics, "physics1");

	entity->AttachProperty(graphicProperty);
	entity->SetString(Attr::Graphics, "box/physics1");

// 	matrix44 entityTransform;
// 	entity->SetMatrix44(Attr::Transform, entityTransform);

	Managers::EntityManager::Instance()->AttachEntity(entity);

	return true;
}

//////////////////////////////////////////////////////////////////////////
// bool CreateEnvironment()
//////////////////////////////////////////////////////////////////////////
bool DemoGameState::CreateEnvironment()
{
    Managers::FactoryManager *factoryManager = Managers::FactoryManager::Instance();

    // create a single envirionment graphics and colide property for all static 
	// environment objects. this reduces clutter in the game entity pool
	Ptr<Properties::EnvironmentGraphicsProperty> gfxProperty = 
		(Properties::EnvironmentGraphicsProperty*)factoryManager->CreateProperty("EnvironmentGraphicsProperty");

	Ptr<Properties::EnvironmentCollideProperty> collideProperty = 
		(Properties::EnvironmentCollideProperty*)factoryManager->CreateProperty("EnvironmentCollideProperty");

	matrix44 worldMatrix;
	Util::SegmentedGfxUtil segGfxUtil;
	nArray<Ptr<Graphics::Entity>> gfxEntities;

	nString resName("level/ground01");

    // begin hack:
    //vector3 pos;
    //pos = Graphics::Stage::Instance()->GetInitPos();

    //worldMatrix.set_translation(pos);
    // end hack:

	worldMatrix.translate(vector3(0.0f, -12.5f, 0.0f));
	gfxEntities = segGfxUtil.CreateAndSetupGraphicsEntities(resName, worldMatrix, false);

	gfxProperty->AddGraphicsEntities(gfxEntities);

	nString collideFilename;
	collideFilename.Format("export:meshes/%s_c_0.n3d2", resName.Get());

	Ptr<Physics::MeshShape> meshShape = Physics::Server::Instance()->CreateMeshShape(worldMatrix,
		Physics::MaterialTable::StringToMaterialType("Soil"),
		collideFilename);

	collideProperty->AddShape(meshShape.get());
	Ptr<Game::Entity> gameEntity = factoryManager->CreateEntityByClassName("Entity");
	gameEntity->AttachProperty(gfxProperty);
	gameEntity->AttachProperty(collideProperty);

	Managers::EntityManager::Instance()->AttachEntity(gameEntity);

	return true;
}

//-----------------------------------------------------------------------------
/**
    create a temparary dummy level
*/
void DemoGameState::CreateLevel()
{
	Physics::Server* physicsServer = Physics::Server::Instance();

	//create a box as ground
    matrix44 m;
	m.translate(vector3(0.0f, -0.5f, 0.0f));
	Ptr<Physics::BoxShape> ground = physicsServer->CreateBoxShape(m, 
		Physics::MaterialTable::StringToMaterialType("Soil"), 
		vector3(100.0f, 0.1f, 100.0f));

	// attach ground to level
	//physicsServer->GetLevel()->AttachShape(ground);
}