//------------------------------------------------------------------------------
//  application/gamestatehandler.cc
//  (C) 2006 je.a.le@wanadoo.fr for nebula2
//------------------------------------------------------------------------------
#include "mtutorial04/state/mTestgamestatehandler.h"

#include "kernel/nreferenced.h"

#include "properties/inputproperty.h"
#include "attr/attributes.h"
#include "game/entity.h"
#include "game/property.h"
#include "managers/entitymanager.h"
#include "managers/factorymanager.h"
#include "managers/focusmanager.h"

#include "gui/nguiwindow.h" 
#include "gui/nguiserver.h"
#include "gui/nguitextlabel.h" 

#include "mtutorial04/property/polarcameraproperty.h"

namespace Application
{
ImplementRtti(Application::mTestGameStateHandler, Application::GameStateHandler);
ImplementFactory(Application::mTestGameStateHandler);

using namespace Game;
using namespace Managers;

//------------------------------------------------------------------------------
/**
*/
mTestGameStateHandler::mTestGameStateHandler()
{
	this->SetExitState( "ExitState" );
	this->SetName( "GameState" );
	this->SetSetupMode(EmptyWorld);
}

//------------------------------------------------------------------------------
/**
*/
mTestGameStateHandler::~mTestGameStateHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/// called when attached to application object
/// Now we will create label and other data that belong to game state
void mTestGameStateHandler::OnAttachToApplication()
{
	// create gui label this state (game) will use.
	// formelly inside app
	SetupGui();
	
	// NOTA : it's tempting to add our input entity/property here. The problem is
	// that world is NOT yet created ! so when it will happen -OnStateEnter- ; we will trigger
	// and assert...
}

 //------------------------------------------------------------------------------
/// called when the state represented by this state handler is entered
void mTestGameStateHandler::OnStateEnter(const nString& prevState)
{
	// show this state handler info
	textLabel->Show();

	// This is a good place to take care about input switching
	// sol 1 : switch the current input entity/property with a new one.
	// the problem is we assum we know all about others states, entities, properties.. : bad
	// sol 2 : check if an entity is set - eventually do it - switch input to it
	// NOTA : nebula don't like when no entity at all have input...	
	// BIS : ok, you can too set pointer instead of using names...
	Ptr<Game::Entity> entity =  Managers::EntityManager::Instance()->GetEntityByName( "PolarCameraControl", true );
	if( entity == 0 ) {
		// Add an entity that will take care of input direct this state loop
		Ptr<Game::Property> inputProperty;		

		entity = Managers::FactoryManager::Instance()->CreateEntityByClassName("Entity");
		// let's name our entity. It's easier to found en entity when it has a name !
		entity->SetString( Attr::Name, "PolarCameraControl" );
	    
		inputProperty = Managers::FactoryManager::Instance()->CreateProperty("PolarCameraInputProperty");
		entity->AttachProperty(inputProperty);        
	    
		Managers::EntityManager::Instance()->AttachEntity(entity);
	}

	// request focus for input
	Managers::FocusManager::Instance()->SetInputFocusEntity( entity );	
}
    
//------------------------------------------------------------------------------
/// called when the state represented by this state handler is left
void mTestGameStateHandler::OnStateLeave(const nString& nextState)
{
	// leaving,
	textLabel->Hide();
	
	// DON'T call inherit 'default) function !! that would cleanup the world !!
}

//------------------------------------------------------------------------------
/**
    The per-frame handler method.
*/
nString mTestGameStateHandler::OnFrame()
{
	static char buf[512];
	float	phi = 0.0f;
	float	theta = 0.0f;
	float	dist = 0.0f;
	
	// read informations from camera. EVERYTHING is done throught manager and properties.
	// PS : a faster way would be to save the entity ref inside a var but this describe
	// how manager and properties could be used. (This is the name of the camera we gave it)
	Ptr<Game::Entity> CameraEntity = Managers::EntityManager::Instance()->GetEntityByName( "MainGameCamera", true );	// only if exist
	
	if( CameraEntity != 0 ) {
		if( CameraEntity.isvalid() ) {
			// Var (attr) are saved inside the entity (ps : sometime inside a specific property instead)
			dist = CameraEntity->GetFloat( Attr::PolarCameraDistance );
			phi = CameraEntity->GetFloat( Attr::PolarCameraPhi );
			theta = CameraEntity->GetFloat( Attr::PolarCameraTheta );        
		}
	}

	//Update the label with the current frame displayed
	sprintf( buf, "Current frame : %d\n Camera Phi : %.2f - Theta : %.2f - Dist : %.2f\nPress \"P\" to enter pause state",
		Graphics::Server::Instance()->GetFrameId(), phi, theta, dist );
	textLabel->SetText( buf );

	// return the next state (actually still game....)
	return( GameStateHandler::OnFrame() );
}

//------------------------------------------------------------------------------
/**
    This does not setup the dragbox object.
    The purpose is to display usefull information as frame rate, debug info, etc...
*/
void mTestGameStateHandler::SetupGui()
{
     // initialize Nebula2 Gui server
    nGuiServer* guiServer = nGuiServer::Instance();
    nKernelServer* kernelServer = nKernelServer::Instance();    
    
    kernelServer->PushCwd(nGuiServer::Instance()->GetRootWindowPointer());
     
	// create  a new label which will be modified into the onFrame callback
	this->textLabel = (nGuiTextLabel*) nKernelServer::Instance()->New("nguitextlabel", "HelpLabel");
    this->textLabel->SetFont("GuiSmall");
    this->textLabel->SetAlignment(nGuiTextLabel::Left);
    this->textLabel->SetColor(vector4(1.0f, 1.0f, 1.0f, 1.0f));
    this->textLabel->SetClipping(false);
    this->textLabel->SetText( "\n\n\n" );	// only set to allow GetTextExtent to return something...  
    this->textLabel->OnShow();
    
	vector2 textExtent = this->textLabel->GetTextExtent();
    rectangle textRect(vector2(0.0f, 0.0f), textExtent );
    this->textLabel->SetRect(textRect);

    kernelServer->PopCwd();

	// creating the widget doesn't mean it time to display it (anymore) !!!
	this->textLabel->Hide();
}

} // namespace Application