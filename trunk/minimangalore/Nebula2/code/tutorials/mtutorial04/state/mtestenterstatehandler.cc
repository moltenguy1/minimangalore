//------------------------------------------------------------------------------
//  application/gamestatehandler.cc
//  (C) 2006 je.a.le@wanadoo.fr for nebula2
//------------------------------------------------------------------------------
#include "mtutorial04/state/mtestenterstatehandler.h"

#include "properties/inputproperty.h"
#include "attr/attributes.h"
#include "game/entity.h"
#include "managers/entitymanager.h"

#include "gui/nguiwindow.h" 
#include "gui/nguiserver.h"
#include "gui/nguitextlabel.h" 

namespace Application
{
ImplementRtti(Application::mTestEnterStateHandler, Application::GameStateHandler);
ImplementFactory(Application::mTestEnterStateHandler);

using namespace Game;
using namespace Managers;

//------------------------------------------------------------------------------
/**
*/
mTestEnterStateHandler::mTestEnterStateHandler()
{
	this->SetExitState( "ExitState" );
	this->SetName( "EnterState" );
	this->SetSetupMode(EmptyWorld);
}

//------------------------------------------------------------------------------
/**
*/
mTestEnterStateHandler::~mTestEnterStateHandler()
{
    // empty
}

 //------------------------------------------------------------------------------
/// called when the state represented by this state handler is entered
void mTestEnterStateHandler::OnStateEnter(const nString& prevState)
{
	// use the default load process
	GameStateHandler::OnStateEnter( prevState );
}
    
//------------------------------------------------------------------------------
/// called when the state represented by this state handler is left
void mTestEnterStateHandler::OnStateLeave(const nString& nextState)
{
	// the default behave is to save the state and clean everything
	// this state setup the wolrd and move to "gamestatehandler"
	// overide default and so far do... nothing...
}

//------------------------------------------------------------------------------
/**
    The per-frame handler method.
*/
nString mTestEnterStateHandler::OnFrame()
{
	// must return the name of next state handler
	return( "GameState" );
}


} // namespace Application