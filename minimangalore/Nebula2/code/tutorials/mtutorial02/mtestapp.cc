//------------------------------------------------------------------------------
//  mTutorial01/mTestapp.cc
//  (C) 2006 je.a.le@wanadoo.fr for nebula2
//------------------------------------------------------------------------------
#include "mtutorial02/mtestapp.h" 

#include "scene/nsceneserver.h"
#include "managers/entitymanager.h"
#include "managers/factorymanager.h"
#include "managers/focusmanager.h"
#include "application/gamestatehandler.h"

#include "gui/nguiserver.h" 
#include "gui/nguiwindow.h" 
#include "gui/nguilabel.h"
#include "gui/nguitextlabel.h" 

#include "game/property.h"
#include "game/entity.h"
#include "properties/lightproperty.h"
#include "properties/graphicsproperty.h"

#include "mtutorial02/state/mtestgamestatehandler.h"
#include "mtutorial02/property/polarcameraproperty.h"

namespace mTutorial
{
mTestApp* mTestApp::Singleton = 0;

//------------------------------------------------------------------------------
/**
	Constructor
*/
mTestApp::mTestApp()
{
    n_assert(0 == Singleton);
    Singleton = this;
}

//------------------------------------------------------------------------------
/**
	Destructor
*/
mTestApp::~mTestApp()
{
    n_assert(0 != Singleton);
    Singleton = 0;
}

//------------------------------------------------------------------------------
/**
    Override this method in subclasses to return a different application name.
*/
nString mTestApp::GetAppName() const
{
    return "Tutorial 02, gui and static camera";
}

//------------------------------------------------------------------------------
/**
    Override this method in subclasses to return a different version string.
*/
nString mTestApp::GetAppVersion() const
{
    return "1.0";
}

//------------------------------------------------------------------------------
/**
    Get the application vendor. This is usually the publishers company name.
*/
nString mTestApp::GetVendorName() const
{
    return "Radon Labs GmbH";
}

//------------------------------------------------------------------------------
/**
    Sets up default state which may be modified by command line args and
    user profile.
*/
void mTestApp::SetupFromDefaults()
{
    App::SetupFromDefaults();
}

//------------------------------------------------------------------------------
/**
 setup the app's input mapping (called in SetupSubsystems())
*/
void mTestApp::SetupDefaultInputMapping() {
	App::SetupDefaultInputMapping();  
}

//------------------------------------------------------------------------------
/**
    This initializes some objects owned by DsaApp.
*/
bool mTestApp::Open()
{   
     if (App::Open())
    {
        // FIXME: turn off clip plane fencing and occlusion query (FOR NOW) 
        // because of compatibility problems on Radeon cards
        nSceneServer::Instance()->SetClipPlaneFencing(false);
        nSceneServer::Instance()->SetOcclusionQuery(false);

		// then setup all parts of the application : input, camera, world, etc...
		this->SetupGui();
		
		// add some object to lock at...
		this->CreateViewedEntity();
		// add at least one light
        this->SetupLightsInScene();
        // add a came where we can look from
        this->SetupCamera();
            		
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
    Clean up objects created by Open().
*/
void mTestApp::Close()
{
    App::Close();
}

//------------------------------------------------------------------------------
/**
    This method is called once per-frame by App::Run(). It is used here
    to check if reset is requested and that case to evoke it.
*/
void mTestApp::OnFrame()
{
	static char buf[512];
	float	phi = 0.0f;
	float	theta = 0.0f;
	float	dist = 0.0f;
	
	// read informations from camera. EVERYTHING is done throught manager and properties.
	// PS : a faster way would be to save the entity ref inside a var but this describe
	// how manager and properties could be used. (This is the name of the camera we gave it)
	Ptr<Game::Entity> CameraEntity = Managers::EntityManager::Instance()->GetEntityByName( "MainGameCamera", true );	// only if exist
	
	if( CameraEntity != null ) {
		// Var (attr) are saved inside the entity (ps : sometime inside a specific property instead)
		dist = CameraEntity->GetFloat( Attr::PolarCameraDistance );
		phi = CameraEntity->GetFloat( Attr::PolarCameraPhi );
		theta = CameraEntity->GetFloat( Attr::PolarCameraTheta );        
		
		// useless here but a sample to how getting a specific property ref from entity
//		Ptr<Properties::PolarCameraProperty> camprop = (Properties::PolarCameraProperty *)(CameraEntity->FindProperty( PolarCameraProperty::RTTI ));
	}

	//Update the label with the current frame displayed
	sprintf( buf, "Current frame : %d\n Camera Phi : %.2f - Theta : %.2f - Dist : %.2f\n",
		Graphics::Server::Instance()->GetFrameId(), phi, theta, dist );
	textLabel->SetText( buf );
}
//------------------------------------------------------------------------------
/**
// setup the state of our application, directly start in "game" state
*/
void mTestApp::SetupStateHandlers()
{
    // initialize application state handlers
    // NOTA : nothing new, just replace default state handle by our...
    Ptr<Application::GameStateHandler> mtestgameStateHandler = n_new(Application::mTestGameStateHandler);
    mtestgameStateHandler->SetName("Game");
    mtestgameStateHandler->SetExitState("Exit");
	mtestgameStateHandler->SetSetupMode(Application::mTestGameStateHandler::EmptyWorld);
    this->AddStateHandler(mtestgameStateHandler);
	this->SetState("Game");	
}

//------------------------------------------------------------------------------
/**
    This does not setup the dragbox object.
    The purpose is to display usefull information as frame rate, debug info, etc...
*/
void mTestApp::SetupGui()
{
     // initialize Nebula2 Gui server
    nGuiServer* guiServer = nGuiServer::Instance();
    nKernelServer* kernelServer = nKernelServer::Instance();
    guiServer->SetRootPath("/res/gui");
    guiServer->SetDisplaySize(vector2(float(this->displayMode.GetWidth()), float(this->displayMode.GetHeight())));
   
    // initialize the overlay gui
    const float borderSize = 0.02f;

    kernelServer->PushCwd(nGuiServer::Instance()->GetRootWindowPointer());
   
	// create  a new label which will be modified into the onFrame callback
	textLabel = (nGuiTextLabel*) nKernelServer::Instance()->New("nguitextlabel", "HelpLabel");
    textLabel->SetFont("GuiSmall");
    textLabel->SetAlignment(nGuiTextLabel::Left);
    textLabel->SetColor(vector4(1.0f, 1.0f, 1.0f, 1.0f));
    textLabel->SetClipping(false);
    
    textLabel->SetText( "\n\n" );	// only set to allow GetTextExtent to return something...  
	vector2 textExtent = textLabel->GetTextExtent();
    rectangle textRect(vector2(0.0f, 0.0f), textExtent );
    textLabel->SetRect(textRect);
	textLabel->OnShow();

    kernelServer->PopCwd();
}

//------------------------------------------------------------------------------
/**
// statically load some object, here the opelblitz
*/
void mTestApp::CreateViewedEntity()
{
// name of the object, relative to export:gfxlib
nString	objectName = "examples/opelblitz";
Ptr<Game::Entity> entity;
Ptr<Game::Property> graphicProperty;

	//------------------------------------
	// The object, every thing is done though property : here add the graphic property
	// to our object in order to get it... on screen !
 	entity = Managers::FactoryManager::Instance()->CreateEntityByClassName("Entity");
	graphicProperty = Managers::FactoryManager::Instance()->CreateProperty("GraphicsProperty");

	// attach properties
    entity->AttachProperty( graphicProperty );
	entity->SetString( Attr::Graphics, objectName );

	// a transformation matrix is mandatory, even if just init
    matrix44 entityTransform;
    entity->SetMatrix44(Attr::Transform, entityTransform);

    // attach to world
    Managers::EntityManager::Instance()->AttachEntity(entity);
}

//------------------------------------------------------------------------------
/**
// no light, no display !!
*/
void mTestApp::SetupLightsInScene()
{
Ptr<Game::Entity> entity;
Ptr<Game::Property> lightProperty;

	// create an object in the same wat as the opelblitz one, but this time
	// attach a light property instead of graphics.
	// there must be at least one light
    //create new Entity for light in scene, create lightProperty and attach it to the entity
    entity = Managers::FactoryManager::Instance()->CreateEntityByClassName("Entity");
    lightProperty = Managers::FactoryManager::Instance()->CreateProperty("LightProperty");
    entity->AttachProperty(lightProperty);

    //set position of lightsource
    matrix44 lightTransform;
    lightTransform.translate(vector3(0.0, 100.0f, 0.0));
    entity->SetMatrix44(Attr::Transform, lightTransform);

    //set Attributes of the lightsource
    entity->SetString(Attr::LightType, "Point");
    entity->SetVector4(Attr::LightColor, vector4(1.0f, 1.0f, 1.0f, 1.0f));
    entity->SetFloat(Attr::LightRange, 1000.0f);
    entity->SetVector4(Attr::LightAmbient, vector4(1.0f, 0.0f, 0.0f, 0.0f));
    entity->SetBool(Attr::LightCastShadows, false);

    //attach lightproperty to entity
    Managers::EntityManager::Instance()->AttachEntity(entity);    
}
//------------------------------------------------------------------------------
/**
*/
void mTestApp::SetupCamera()
{
Ptr<Game::Entity> entity;
Ptr<Game::Property> cameraProperty;

    // create new Entity as a camera,
    // attach our more flexible camera with polar coordinate system property
    entity = Managers::FactoryManager::Instance()->CreateEntityByClassName("Entity");
    cameraProperty = Managers::FactoryManager::Instance()->CreateProperty("PolarCameraProperty");
	entity->AttachProperty(cameraProperty);        
 
	// It's important to name the entity since we use this name inside the OnFrame loop in order
	// to display camera value
	entity->SetString( Attr::Name, "MainGameCamera" );
	
	// set some default values from where the camera will look at the "opelblitz"
	entity->SetVector3( Attr::PolarCameraLookAt, vector3.zero );	// look at origine
	entity->SetFloat( Attr::PolarCameraPhi, -45.0f );	// -45, that's not a mistake... do our math !
	entity->SetFloat( Attr::PolarCameraTheta, 45.0f );
	entity->SetFloat( Attr::PolarCameraDistance, 10.0f );

    //attach camera to world (entity pool), and tell mangalore we use it for display !!!
    Managers::EntityManager::Instance()->AttachEntity(entity);
    Managers::FocusManager::Instance()->SetCameraFocusEntity(entity);        
}

}; // namespace