//------------------------------------------------------------------------------
//  application/gamestatehandler.cc
//  (C) 2006 je.a.le@wanadoo.fr for nebula2
//------------------------------------------------------------------------------
#include "mtutorial04/state/mTestpausestatehandler.h"

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

namespace Application
{
ImplementRtti(Application::mTestPauseStateHandler, Application::GameStateHandler);
ImplementFactory(Application::mTestPauseStateHandler);

using namespace Game;
using namespace Managers;

//------------------------------------------------------------------------------
/**
*/
mTestPauseStateHandler::mTestPauseStateHandler()
{
	this->SetExitState( "ExitState" );
	this->SetName( "PauseState" );
	this->SetSetupMode(EmptyWorld);
}

//------------------------------------------------------------------------------
/**
*/
mTestPauseStateHandler::~mTestPauseStateHandler()
{
    // empty
}

//------------------------------------------------------------------------------
/// called when attached to application object
/// Now we will create label and other data that belong to game state
void mTestPauseStateHandler::OnAttachToApplication()
{
	// create gui label this state (pause) will use.
	// formelly inside app
	SetupGui();
}

 //------------------------------------------------------------------------------
/// called when the state represented by this state handler is entered
void mTestPauseStateHandler::OnStateEnter(const nString& prevState)
{
	// show this state handler info
	textLabel->Show();

	// This is a good place to take care about input switching
	// sol 1 : switch the current input entity/property with a new one.
	// the problem is we assum we know all about others states, entities, properties.. : bad
	// sol 2 : check if an entity is set - eventually do it - switch input to it
	// NOTA : nebula don't like when no entity at all have input...	
	// BIS : ok, you can too set pointer instead of using names...
	Ptr<Game::Entity> entity =  Managers::EntityManager::Instance()->GetEntityByName( "PauseControl", true );
	if( entity == 0 ) {
		// Add an entity that will take care of input direct this state loop
		Ptr<Game::Property> inputProperty;		

		entity = Managers::FactoryManager::Instance()->CreateEntityByClassName("Entity");
		// let's name our entity. It's easier to found en entity when it has a name !
		entity->SetString( Attr::Name, "PauseControl" );
	    
		inputProperty = Managers::FactoryManager::Instance()->CreateProperty("PauseInputProperty");
		entity->AttachProperty(inputProperty);        
	    
		Managers::EntityManager::Instance()->AttachEntity(entity);
	}

	// state get active, now it's time to switch input control
	Managers::FocusManager::Instance()->SetInputFocusEntity( entity );
}
    
//------------------------------------------------------------------------------
/// called when the state represented by this state handler is left
void mTestPauseStateHandler::OnStateLeave(const nString& nextState)
{
	// leaving,
	textLabel->Hide();
	
	// DON'T call inherit 'default) function !! that would cleanup the world !!
}

//------------------------------------------------------------------------------
/**
    The per-frame handler method.
*/
nString mTestPauseStateHandler::OnFrame()
{
	return( GameStateHandler::OnFrame() );
}

//------------------------------------------------------------------------------
/**
    This does not setup the dragbox object.
    The purpose is to display usefull information as frame rate, debug info, etc...
*/
void mTestPauseStateHandler::SetupGui()
{
     // initialize Nebula2 Gui server
    nGuiServer* guiServer = nGuiServer::Instance();
    nKernelServer* kernelServer = nKernelServer::Instance();    
    
    kernelServer->PushCwd(nGuiServer::Instance()->GetRootWindowPointer());
   
	// create  a new label which will be modified into the onFrame callback
	this->textLabel = (nGuiTextLabel*) nKernelServer::Instance()->New("nguitextlabel", "PauseLabel");
    this->textLabel->SetFont("GuiSmall");
    this->textLabel->SetColor(vector4(1.0f, 1.0f, 0.0f, 1.0f));
    this->textLabel->SetClipping(false);
    this->textLabel->SetAlignment( nGuiTextLabel::Center );    
    this->textLabel->SetText( "PAUSE\nPress \"P\" to resume game..." );
	this->textLabel->OnShow();
	
	vector2 textExtent = textLabel->GetTextExtent();	
	rectangle volume;
    volume.v0.set( 0.5f - 0.5f * textExtent.x, 0.5f - 0.5f * textExtent.y );    
    volume.v1.set(0.5f + 0.5f * textExtent.x, 0.5f + 0.5f * textExtent.y );
    this->textLabel->SetRect(volume);
    
    kernelServer->PopCwd();

	// creating the widget doesn't mean it time to display it (anymore) !!!
	this->textLabel->Hide();	
}

} // namespace Application