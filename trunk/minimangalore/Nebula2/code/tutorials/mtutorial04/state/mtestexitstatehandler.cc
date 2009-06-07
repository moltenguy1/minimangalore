//------------------------------------------------------------------------------
//  application/gamestatehandler.cc
//  (C) 2006 je.a.le@wanadoo.fr for nebula2
//------------------------------------------------------------------------------
#include "mtutorial04/state/mtestexitstatehandler.h"

#include "properties/inputproperty.h"
#include "attr/attributes.h"
#include "game/entity.h"
#include "managers/entitymanager.h"

namespace Application
{
ImplementRtti(Application::mTestExitStateHandler, Application::GameStateHandler);
ImplementFactory(Application::mTestExitStateHandler);

using namespace Game;
using namespace Managers;

//------------------------------------------------------------------------------
/**
*/
mTestExitStateHandler::mTestExitStateHandler() 
{
	this->SetExitState( "Exit" );
	this->SetName( "ExitState" );
	this->SetSetupMode(EmptyWorld);
}

//------------------------------------------------------------------------------
/**
*/
mTestExitStateHandler::~mTestExitStateHandler()
{
    // empty
}

 //------------------------------------------------------------------------------
/// called when the state represented by this state handler is entered
void mTestExitStateHandler::OnStateEnter(const nString& prevState)
{
	// we don't want a new world when enter this state...
}
    
//------------------------------------------------------------------------------
/// called when the state represented by this state handler is left
void mTestExitStateHandler::OnStateLeave(const nString& nextState )
{
	// and don't forget to call inherit (will clean up level, switch focus, etc...)
	GameStateHandler::OnStateLeave( nextState );	
}

//------------------------------------------------------------------------------
/**
    The per-frame handler method.
*/
nString mTestExitStateHandler::OnFrame()
{
	// must return the name of the next state
	return( "Exit" );
}

} // namespace Application