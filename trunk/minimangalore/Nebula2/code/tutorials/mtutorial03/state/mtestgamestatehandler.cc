//------------------------------------------------------------------------------
//  application/gamestatehandler.cc
//  (C) 2006 je.a.le@wanadoo.fr for nebula2
//------------------------------------------------------------------------------
#include "mtutorial03/state/mtestgamestatehandler.h"

#include "properties/inputproperty.h"
#include "attr/attributes.h"
#include "game/entity.h"
#include "managers/entitymanager.h"

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
    // empty
}

//------------------------------------------------------------------------------
/**
*/
mTestGameStateHandler::~mTestGameStateHandler()
{
    // empty
}


//------------------------------------------------------------------------------
/**
    The per-frame handler method.
*/
nString
mTestGameStateHandler::OnFrame()
{
	return( GameStateHandler::OnFrame() );
}

} // namespace Application