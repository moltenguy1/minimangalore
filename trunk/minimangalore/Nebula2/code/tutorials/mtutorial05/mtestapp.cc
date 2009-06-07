//------------------------------------------------------------------------------
//  mTutorial01/mTestapp.cc
//  (C) 2006 je.a.le@wanadoo.fr for nebula2
//------------------------------------------------------------------------------
#include "mtutorial05/mtestapp.h" 

#include "scene/nsceneserver.h"
#include "managers/entitymanager.h"
#include "managers/factorymanager.h"
#include "application/gamestatehandler.h"

#include "gui/nguiserver.h" 

#include "game/property.h"
#include "game/entity.h"
#include "properties/lightproperty.h"
#include "properties/graphicsproperty.h"

#include "physics/level.h"
#include "properties/physicsproperty.h"

#include "mtutorial05/state/mtestgamestatehandler.h"
#include "mtutorial05/property/polarcameraproperty.h"
#include "mtutorial05/property/opelphysicsproperty.h"


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
    return "Tutorial 05, First physics object";
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

		// add some object to lock at...
		this->CreateViewedEntity();
		// add at least one light
        this->SetupLightsInScene();
        // add a came where we can look from
        this->SetupCameraAndInput();
            		
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
	// Move the code where it should belong : IN THE STATE HANDLER OnFrame !!!
}
//------------------------------------------------------------------------------
/**
// setup the state of our application, directly start in "game" state
*/
void mTestApp::SetupStateHandlers()
{
Ptr<Application::GameStateHandler> mtestStateHandler;

    // initialize the first state : "game"
    mtestStateHandler = n_new(Application::mTestGameStateHandler);
    this->AddStateHandler(mtestStateHandler);	// add it to the world

	this->SetState("GameState");	
}

//------------------------------------------------------------------------------
/**
// statically load some object, here the opelblitz
*/
void mTestApp::CreateViewedEntity()
{
// name of the object, relative to export:gfxlib
nString	objectName = "tutorials/opel2";
nString groundName = "tutorials/ground";
Ptr<Game::Entity> entity;
Ptr<Game::Property> graphicProperty;
Ptr<Properties::OpelPhysicsProperty> opelPhysicsProperty;
Ptr<Properties::PhysicsProperty> physicsProperty;
matrix44 entityTransform;

	//------------------------------------
	// The object, every thing is done though property : here add the graphic property
	// to our object in order to get it... on screen !
 	entity = Managers::FactoryManager::Instance()->CreateEntityByClassName("Entity");
	entity->SetString( Attr::Name, "Opelblitz" );
	
	// this entity will use physics (simulation), so we must add to it... a physics property.
	// actully we attach a subclass of PhysicsProperty, which handler more control
    opelPhysicsProperty = (Properties::OpelPhysicsProperty*) Managers::FactoryManager::Instance()->CreateProperty("OpelPhysicsProperty");

	// you MUST attach physic BEFORE graphics
	entity->AttachProperty(opelPhysicsProperty);
	// and don't forget to attach the xml file. (can use the same name because : 
	// physics:tutorials/opel2.xml , gfxlib:tutorials/opel2.n3d2, etc...)
	entity->SetString( Attr::Physics, "tutorials/opel2" );

	// and the graphic property
	graphicProperty = Managers::FactoryManager::Instance()->CreateProperty("GraphicsProperty");
    entity->AttachProperty( graphicProperty );
	entity->SetString( Attr::Graphics, objectName );

	// a transformation matrix is mandatory, even if just init
 	entityTransform.ident();
	entityTransform.translate( vector3( 0.0f, 1.0f, 0.0f ) );
    entity->SetMatrix44(Attr::Transform, entityTransform);

    // attach to world
    Managers::EntityManager::Instance()->AttachEntity(entity);    
    
    // and don't forget to enable physics simulation for this entity...
    // else, the entity will not be added to the simulation
    opelPhysicsProperty->GetPhysicsEntity()->SetEnabled(true);
    
    //---------------------------------
    // Create and attach... a ground ; might be helpfull now there's physics enabled !!
    entity = Managers::FactoryManager::Instance()->CreateEntityByClassName("Entity");
	graphicProperty = Managers::FactoryManager::Instance()->CreateProperty("GraphicsProperty");
    physicsProperty = (Properties::PhysicsProperty*) Managers::FactoryManager::Instance()->CreateProperty("PhysicsProperty");
	entity->SetString( Attr::Name, "Ground" );

	entity->AttachProperty(physicsProperty);
	entity->SetString( Attr::Physics, groundName );

    entity->AttachProperty( graphicProperty );
	entity->SetString( Attr::Graphics, groundName );
	entityTransform.ident();
    entityTransform.translate( vector3( 0.0f, 0.0f, 0.0f ) );
    entity->SetMatrix44(Attr::Transform, entityTransform);

    Managers::EntityManager::Instance()->AttachEntity(entity);    
    physicsProperty->GetPhysicsEntity()->SetEnabled(true);

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
void mTestApp::SetupCameraAndInput()
{
Ptr<Game::Entity> entity;
Ptr<Game::Property> cameraProperty;
//Ptr<Game::Property> inputProperty;

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

	// NOTA : no more input management. each state (game, pause) now take care 
	// of that. (ps : I could have set a specific camera too...)

    //attach camera to world (entity pool), and tell mangalore we use it for display !!!
    Managers::EntityManager::Instance()->AttachEntity(entity);
    Managers::FocusManager::Instance()->SetCameraFocusEntity(entity);       
}

}; // namespace