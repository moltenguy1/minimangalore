//-----------------------------------------------------------------------------
#include "foundation/ptr.h"

#include "states/demogamestate.h"

#include "managers/factorymanager.h"

#include "db/server.h"

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

#include "properties/ueplayerphysicsproperty.h"

#include "physics/level.h"
#include "physics/meshshape.h"
#include "physics/boxshape.h"
#include "gui/nguitextlabel.h"
#include "gui/nguiserver.h"
#include "gui/nguicanvas.h"
#include "kernel/nkernelserver.h"
#include "variable/nvariableserver.h"
#include "util/segmentedgfxutil.h"

#include "kernel/ntimeserver.h"
#include <stdio.h>


//-----------------------------------------------------------------------------
/**
*/
DemoGameState::DemoGameState()
{
	this->SetName("GameState");
	this->SetSetupMode(EmptyWorld);
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
void DemoGameState::OnStateEnter(const nString& prevState)
{
    GameStateHandler::OnStateEnter(prevState);

	state = READY;
	onShowHelp = false;

	posGoal = vector3(-12.0f,-11.0f,-10.0f);
	timeReadyAcc = 0.0f;
	timeStartAcc = 0.0f;
	timeFinishAcc = 0.0f;
	timeAcc = 0.0f;
	timeFinish = 0.0f;
	timePrev = nTimeServer::Instance()->GetTime();

	recordMin;
	recordSec;
	recordMs;



    // create an default light entity
    this->CreateDefaultLight();

// begin start to load level
	//this->CreateLevel();

    // create a camera entity
    //this->CreateCamera();

	// create environment such as ground etc.
	this->CreateEnvironment();

    // create an player entity
    this->CreatePlayer();
	//this->CreateBox();
	this->CreateGUI();

	//this->CreateUePlayer();
	this->SetSceneVariable();

	Ptr<Input::Server> inputServer = Input::Server::Instance();

/*
 	inputServer->AddMapping("moveForward", "keyb0:w");
 	inputServer->AddMapping("moveBackward", "keyb0:s");
 	inputServer->AddMapping("moveLeft", "keyb0:a");
 	inputServer->AddMapping("moveRight", "keyb0:d");

	inputServer->AddMapping("poseUp", "keyb0:p");
	inputServer->AddMapping("poseDn", "keyb0:l");
*/
	inputServer->AddMapping("gotoGoal","keyb0:g");

	inputServer->AddMapping("help","keyb0:h");


	inputServer->AddMapping("moveForward", "keyb0:up");
	inputServer->AddMapping("moveBackward", "keyb0:down");
	inputServer->AddMapping("moveLeft", "keyb0:left");
	inputServer->AddMapping("moveRight", "keyb0:right");

	inputServer->AddMapping("poseUp", "keyb0:q");
	inputServer->AddMapping("poseDn", "keyb0:a");

	onShowHelp=false;
}

//-----------------------------------------------------------------------------
/**
*/
void DemoGameState::OnStateLeave(const nString& nextState)
{
    //TODO: put state leave code  
    GameStateHandler::OnStateLeave(nextState);
}

//-----------------------------------------------------------------------------
/**
*/
nString DemoGameState::OnFrame()
{
	Ptr<Input::Server> inputServer = Input::Server::Instance();
	if(inputServer->GetButtonPressed("help"))
	{
		this->guiCanvasHow->Show();

	}
	else	this->guiCanvasHow->Hide();

	nTime timeCurr = nTimeServer::Instance()->GetTime();
	nTime timeDelta;

	timeDelta = timeCurr - timePrev;
	this->timeAcc += timeDelta;

	this->debugString = "";

	nString str;
	vector3 temp=player->GetVector3(Attr::position);
	matrix44 lastTM=player->GetMatrix44(Attr::lastTM);
	str.SetVector3(temp);		
	AddDebugString(str);

	str.SetMatrix44(lastTM);
	AddDebugString(str);

	


	switch(state)
	{
	case READY:
		if (timeReadyAcc == 0.0f)
		{
			//Ready Sound

			//Ready UI Show
			this->guiReady->Show();

		}
		timeReadyAcc += timeDelta;
		if (timeReadyAcc > 4.0f)
		{
			
			// Ready UI Hide
			this->guiReady->Hide();
			state = START;
		}
		break;
	case START:
		if (timeStartAcc == 0.0f)
		{
			// Start UI Show
			this->guiStart->Show();
		}
		else if (timeStartAcc > 3.0f)
		{
			// Start UI hide
			this->guiStart->Hide();
		}
		timeStartAcc += timeDelta;
		if (IsUePlayerInGoalPos())
		{
			state = FINISH;
			timeFinish = timeStartAcc;			
		}
		break;
	case FINISH:
		if (timeFinishAcc == 0.0f)
		{
			 //Goal UI show
			this->guiFinish->Show();
		}
		timeFinishAcc += timeDelta;
		if ((timeFinishAcc > 3.0f)&&(timeFinishAcc < 7.0f))
		{
			// Show Continue UI 
			this->guiFinish->Hide();
			this->guiEnding[0]->Show();
		}
		else	if ((timeFinishAcc > 7.0f)&&(timeFinishAcc < 10.0f))
		{
			// Show Continue UI 
			this->guiEnding[0]->Hide();
			this->guiEnding[1]->Show();
		}
		else	if ((timeFinishAcc > 10.0f)&&(timeFinishAcc < 13.0f))
		{
			// Show Continue UI 
			this->guiEnding[1]->Hide();
			this->guiEnding[2]->Show();
		}
		else	if ((timeFinishAcc > 13.0f)&&(timeFinishAcc < 16.0f))
		{
			// Show Continue UI 
			this->guiEnding[2]->Hide();
			this->guiEnding[3]->Show();
		}
		else	if ((timeFinishAcc > 16.0f)&&(timeFinishAcc < 19.0f))
		{
			// Show Continue UI 
			this->guiEnding[4]->Hide();
			this->guiNewRecord->Show();
		}


		break;
	}
	
	
	CalculateRecord();
	UI_MeterShow();


/*
	str.SetFloat((float)timeReadyAcc);		
	AddDebugString(str);
	str.SetFloat((float)timeStartAcc);		
	AddDebugString(str);
	str.SetFloat((float)timeGoalAcc);		
	AddDebugString(str);*/


	this->guiTL_DebugText->SetText(this->debugString.Get());
	this->timePrev = timeCurr;
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


	Ptr<Game::Property> pp = factoryManager->CreateProperty("UePlayerPhysicsProperty");
	this->player->AttachProperty(pp);

	Ptr<Game::Property> ap = factoryManager->CreateProperty("UePlayerAnimationProperty");
	this->player->AttachProperty(ap);

	Ptr<Game::Property> gp = factoryManager->CreateProperty("ActorGraphicsProperty");
	this->player->AttachProperty(gp);

	this->player->SetString(Attr::Graphics, "urbanextreme/ueplayer");	// *.n2
	this->player->SetString(Attr::AnimSet, "UePlayer");	// FieldName in anim.xml 

/*
	this->player->SetString(Attr::Graphics, "urbanextreme/character01");
	this->player->SetString(Attr::AnimSet, "UePlayer");	// FieldName in anim.xml */


    Ptr<Game::Property> ip = factoryManager->CreateProperty("UePlayerInputProperty");
    this->player->AttachProperty(ip);

	Ptr<Game::Property> cp = factoryManager->CreateProperty("UePlayerCameraProperty");
	this->player->AttachProperty(cp);

 	Ptr<Game::Property> tp = factoryManager->CreateProperty("TransformableProperty");


    // begin hack
    matrix44 tm;
  
  	this->player->SetMatrix44(Attr::Transform, tm);
  	this->player->AttachProperty(tp);
 
 	Managers::FocusManager::Instance()->SetFocusEntity(this->player);
    Managers::EntityManager::Instance()->AttachEntity(this->player);

    return true;
}

//////////////////////////////////////////////////////////////////////////
// bool CreateBox()
//////////////////////////////////////////////////////////////////////////
bool DemoGameState::CreateBox()
{
	 Managers::FactoryManager *factoryManager = Managers::FactoryManager::Instance();
	Ptr<Game::Entity> entity = factoryManager->CreateEntityByClassName("Entity");
	Ptr<Properties::PhysicsProperty> physicsProperty = (Properties::PhysicsProperty*) factoryManager->CreateProperty("PhysicsProperty");
	Ptr<Game::Property> graphicProperty = factoryManager->CreateProperty("GraphicsProperty");


	entity->AttachProperty(physicsProperty);
	entity->SetString(Attr::Physics, "physics1");

	entity->AttachProperty(graphicProperty);
	entity->SetString(Attr::Graphics, "box/physics1");

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

	nString resName("urbanextreme/bg");	// gfx:/*.n2

	worldMatrix.translate(vector3(0.0f, 0.0f, 0.0f));
	gfxEntities = segGfxUtil.CreateAndSetupGraphicsEntities(resName, worldMatrix, false);

	gfxProperty->AddGraphicsEntities(gfxEntities);

	nString collideFilename;
	Ptr<Physics::MeshShape> meshShape;

	// physics_mesh
	collideFilename.Format("export:meshes/urbanextreme/bg_physicsmesh.n3d2");
	meshShape = Physics::Server::Instance()->CreateMeshShape(worldMatrix,
		Physics::MaterialTable::StringToMaterialType("Soil"),
		collideFilename);
	collideProperty->AddShape(meshShape.get());
	
	
	//기본 월드 중점 지형	
	
/*
	collideFilename.Format("export:meshes/urbanextreme/ground_physicsmesh.n3d2");
			meshShape = Physics::Server::Instance()->CreateMeshShape(worldMatrix,
				Physics::MaterialTable::StringToMaterialType("Soil"),
				collideFilename);
				
		collideProperty->AddShape(meshShape.get());*/

	
		
	


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
	//m.translate(vector3(0.0f, -0.5f, 0.0f));
	Ptr<Physics::BoxShape> ground = physicsServer->CreateBoxShape(m, 
		Physics::MaterialTable::StringToMaterialType("Soil"), 
		vector3(100.0f, 0.1f, 100.0f));

	//physicsServer->GetLevel()->SetGravity(vector3(0.0f,-9.8f,0.0f));

	// attach ground to level
	//physicsServer->GetLevel()->AttachShape(ground);
}

void DemoGameState::CreateGUI()
{
	nGuiServer* guiServer = nGuiServer::Instance();
	nKernelServer* kernelServer = nKernelServer::Instance();    
	kernelServer->PushCwd((nRoot*)nGuiServer::Instance()->GetRootWindowPointer());

	char temp[300]="Dddddddd####\r\nDEDED\r\n";

	// create  a new label which will be modified into the onFrame callback
	this->guiTL_DebugText = (nGuiTextLabel*) nKernelServer::Instance()->New("nguitextlabel", "DebugText");
	this->guiTL_DebugText->SetClipping(false);
	this->guiTL_DebugText->SetText(temp );
	
	this->guiTL_DebugText->SetRect(rectangle(vector2(0.05f,0.05f),vector2(0.05f,0.05f)));
	this->guiTL_DebugText->SetAlignment( nGuiTextLabel::Left);   
	this->guiTL_DebugText->Show();


	int i=0;	
	for (int i=0;i<=10;i++)
	{
		char filename[256]={0,};
		char objname[256]={0,};
		char brushname[256]={0,};
		sprintf(filename,"../urbanextreme/meter%d",i);
		sprintf(objname,"Obj_meter%d",i);
		sprintf(brushname,"Brush_meter%d",i);
		guiMeter[i] = (nGuiCanvas*) nKernelServer::Instance()->New("nguicanvas", objname);
		guiServer->GetSkin()->AddBrush(brushname,filename, vector2(0.0f, 0.0f), vector2(100.0f,400.0f), vector4(0.0f, 0.0f, 0.0f, 0.0f));
		guiMeter[i]->SetDefaultBrush(brushname);
		guiMeter[i]->SetRect(rectangle(vector2(924.0f/1024.0f, 200.0f/768.0f), vector2(1024.0f/1024.0f, 600.0f/768.0f)));
		guiMeter[i]->Hide();
	}

	this->guiReady = (nGuiCanvas*) nKernelServer::Instance()->New("nguicanvas", "Obj_Ready");
	guiServer->GetSkin()->AddBrush("Brush_Ready", "../urbanextreme/ready", vector2(0.0f, 0.0f), vector2(1024.0f,268.0f), vector4(0.0f, 0.0f, 0.0f, 0.0f));
	this->guiReady->SetDefaultBrush("Brush_Ready");
	this->guiReady->SetRect(rectangle(vector2(0.0f, 250.0f/768.0f), vector2(1024.0f/1024.0f, 518.0f/768.0f)));
	this->guiReady->Hide();

	this->guiStart  = (nGuiCanvas*) nKernelServer::Instance()->New("nguicanvas", "Obj_Start");
	guiServer->GetSkin()->AddBrush("Brush_Start", "../urbanextreme/start", vector2(0.0f, 0.0f), vector2(1024.0f,268.0f), vector4(0.0f, 0.0f, 0.0f, 0.0f));
	this->guiStart->SetDefaultBrush("Brush_Start");
	this->guiStart->SetRect(rectangle(vector2(0.0f, 250.0f/768.0f), vector2(1024.0f/1024.0f, 518.0f/768.0f)));
	this->guiStart->Hide();

	this->guiFinish  = (nGuiCanvas*) nKernelServer::Instance()->New("nguicanvas", "Obj_Finish");
	guiServer->GetSkin()->AddBrush("Brush_Finish", "../urbanextreme/finish", vector2(0.0f, 0.0f), vector2(1024.0f,268.0f), vector4(0.0f, 0.0f, 0.0f, 0.0f));
	this->guiFinish->SetDefaultBrush("Brush_Finish");
	this->guiFinish->SetRect(rectangle(vector2(0.0f, 250.0f/768.0f), vector2(1024.0f/1024.0f, 518.0f/768.0f)));
	this->guiFinish->Hide();

	this->guiCanvasHow = (nGuiCanvas*) nKernelServer::Instance()->New("nguicanvas", "Obj_How");
	guiServer->GetSkin()->AddBrush("Brush_How", "../urbanextreme/how", vector2(0.0f, 0.0f), vector2(1024.0f,768.0f), vector4(0.0f, 0.0f, 0.0f, 0.0f));
	this->guiCanvasHow->SetDefaultBrush("Brush_How");
	this->guiCanvasHow->SetRect(rectangle(vector2(0.0f, 0.0f), vector2(1.0f, 1.0f)));
	this->guiCanvasHow->Hide();




	this->guiEnding[0]=	(nGuiCanvas*) nKernelServer::Instance()->New("nguicanvas", "Obj_Ending2");
	guiServer->GetSkin()->AddBrush("Brush_Ending2", "../urbanextreme/ending_2", vector2(0.0f, 0.0f), vector2(1024.0f,768.0f), vector4(0.0f, 0.0f, 0.0f, 0.0f));
	this->guiEnding[0]->SetDefaultBrush("Brush_Ending2");
	this->guiEnding[0]->SetRect(rectangle(vector2(0.0f, 0.0f), vector2(1.0f, 1.0f)));
	this->guiEnding[0]->Hide();

	this->guiEnding[1]=	(nGuiCanvas*) nKernelServer::Instance()->New("nguicanvas", "Obj_Ending3");
	guiServer->GetSkin()->AddBrush("Brush_Ending3", "../urbanextreme/ending_3", vector2(0.0f, 0.0f), vector2(1024.0f,768.0f), vector4(0.0f, 0.0f, 0.0f, 0.0f));
	this->guiEnding[1]->SetDefaultBrush("Brush_Ending3");
	this->guiEnding[1]->SetRect(rectangle(vector2(0.0f, 0.0f), vector2(1.0f, 1.0f)));
	this->guiEnding[1]->Hide();

	this->guiEnding[2]=	(nGuiCanvas*) nKernelServer::Instance()->New("nguicanvas", "Obj_Ending4");
	guiServer->GetSkin()->AddBrush("Brush_Ending4", "../urbanextreme/ending_4", vector2(0.0f, 0.0f), vector2(1024.0f,768.0f), vector4(0.0f, 0.0f, 0.0f, 0.0f));
	this->guiEnding[2]->SetDefaultBrush("Brush_Ending4");
	this->guiEnding[2]->SetRect(rectangle(vector2(0.0f, 0.0f), vector2(1.0f, 1.0f)));
	this->guiEnding[2]->Hide();

	this->guiEnding[3]=	(nGuiCanvas*) nKernelServer::Instance()->New("nguicanvas", "Obj_Ending5");
	guiServer->GetSkin()->AddBrush("Brush_Ending5", "../urbanextreme/ending_5", vector2(0.0f, 0.0f), vector2(1024.0f,768.0f), vector4(0.0f, 0.0f, 0.0f, 0.0f));
	this->guiEnding[3]->SetDefaultBrush("Brush_Ending5");
	this->guiEnding[3]->SetRect(rectangle(vector2(0.0f, 0.0f), vector2(1.0f, 1.0f)));
	this->guiEnding[3]->Hide();

	this->guiNewRecord = (nGuiCanvas*) nKernelServer::Instance()->New("nguicanvas", "Obj_NewRecord");
	guiServer->GetSkin()->AddBrush("Brush_NewRecord", "../urbanextreme/ending", vector2(0.0f, 0.0f), vector2(1024.0f,768.0f), vector4(0.0f, 0.0f, 0.0f, 0.0f));
	this->guiNewRecord->SetDefaultBrush("Brush_NewRecord");
	this->guiNewRecord->SetRect(rectangle(vector2(0.0f, 0.0f), vector2(1.0f, 1.0f)));
	this->guiNewRecord->Hide();



	kernelServer->PopCwd();
}

void DemoGameState::AddDebugString( nString& str )
{
#ifdef _DEBUG
	this->debugString+=str;
	this->debugString+="\r\n";	
#endif	
}

void DemoGameState::SetSceneVariable()
{


	nVariableServer* varServer = nVariableServer::Instance();
	varServer->SetVectorVariable("FogDistances", vector4(0.0f, 1000.0f, 0.0f, 0.0f));
	varServer->SetVectorVariable("CameraFocus", vector4(0.0f, 64.2f, 0.0f, 0.0f));
	varServer->SetFloatVariable("NoiseIntensity", 0.00f);
	varServer->SetFloatVariable("NoiseScale", 0.0f);
	varServer->SetFloatVariable("NoiseFrequency", 25.0f);
	varServer->SetFloatVariable("Saturation", 1.0f);
	varServer->SetFloatVariable("HdrBrightPassThreshold", 0.8f);
	varServer->SetFloatVariable("HdrBrightPassOffset", 1.0f);
	varServer->SetFloatVariable("HDR Intensity", 0.8f);
	varServer->SetVectorVariable("FogColor", vector4(1.0f, 1.0f, 1.0f, 1.0f));


}

bool DemoGameState::IsUePlayerInGoalPos()
{
	vector3 posPlayer=this->player->GetVector3(Attr::position);
	vector3 temp;
	temp = posPlayer - posGoal;

	if(temp.len() < 5.0f )
		return true;

	return false;
}

void DemoGameState::CalculateRecord()
{
	nString str;
	this->recordMin = (int) (this->timeStartAcc/60.0f);
	this->recordSec = (int) this->timeStartAcc % 60;
	this->recordMs  =  (int)(this->timeStartAcc * 100)%100; 

/*
	str.SetInt(this->recordMin);		
	AddDebugString(str);
	str.SetInt(this->recordSec);		
	AddDebugString(str);
	str.SetInt(this->recordMs);		
	AddDebugString(str);*/

}

void DemoGameState::UI_MeterShow()
{
	float relVelocity = this->player->GetFloat(Attr::RelVelocity);
	float maxVelocity = this->player->GetFloat(Attr::MaxVelocity);

	float temp;
	int index=0;	
	if (relVelocity!=0.0f)
	{
		temp = relVelocity / maxVelocity * 10.0f;
		index= ((int)temp);
		index+=1;
		if (index>=10)
		{
			index=10;
		}
		
	}
	
	nString str;
	str.SetFloat(relVelocity);		
	AddDebugString(str);
	str.SetFloat(maxVelocity);		
	AddDebugString(str);
	str.SetInt(index);		
	AddDebugString(str);


	int i;
	for (i=0;i<=10;i++)
	{
		if (i==index)	guiMeter[i]->Show();
		else	guiMeter[i]->Hide();
	}
}