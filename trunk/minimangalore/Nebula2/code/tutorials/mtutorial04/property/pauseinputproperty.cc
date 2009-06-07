//------------------------------------------------------------------------------
//  properties/inputproperty.cc
//  (C) je.a.le@wanadoo.fr
//------------------------------------------------------------------------------
#include "mtutorial04/property/pauseinputproperty.h"

#include "properties/inputproperty.h"
#include "managers/focusmanager.h"
#include "game/entity.h"
#include "input/server.h"
#include "input/event.h"
#include "input/mapping.h"
#include "input/priority.h"
#include "input/source.h"
#include "foundation/factory.h" 

#include "mtutorial04/mtestapp.h"

namespace Properties
{
ImplementRtti(Properties::PauseInputProperty, Properties::InputProperty);
ImplementFactory(Properties::PauseInputProperty);

using namespace Game;
using namespace Managers;
using namespace Input;

//------------------------------------------------------------------------------
PauseInputProperty::PauseInputProperty()
{
    // empty
}

//------------------------------------------------------------------------------
PauseInputProperty::~PauseInputProperty()
{
    // empty
}

//------------------------------------------------------------------------------
/**
/// setup default entity attributes
*/
void PauseInputProperty::SetupDefaultAttributes()
{
	InputProperty::SetupDefaultAttributes();
}

//------------------------------------------------------------------------------
/**
    This method is called by the FocusManager when our entity gains the
    camera focus. Override this method if your subclass needs to do
    some initialization when gaining the camera focus.
*/
void
PauseInputProperty::OnObtainFocus()
{
	InputProperty::OnObtainFocus();

	// add input sink
    Input::Server* inputServer = Input::Server::Instance();
	
	// first, if mapping not already set (from startup script...), does it
	if( inputServer->HasMapping( "pause" ) == false ) {
		inputServer->AddMapping("pause", "keyb0:p");
		
		// (fixed as svn 3267) to use the "pause" key :
		//inputServer->AddMapping("pause", "keyb0:pause");
		// but cause of a directx bug, you must comment too this line
		// code/nebula2/src/input/nkeyboarddevice.cc line #42
	}

	// attach sink, when this mapping is set, input server will infor this property.
	inputServer->AttachInputSink("pause", Input::InputPriority_MousePositionTracking, this );	
	}

//------------------------------------------------------------------------------
/**
    This method is called by the FocusManager when our entity loses
    the camera focus. Override this method if your subclass needs to do any
    cleanup work here.
*/
void PauseInputProperty::OnLoseFocus()
{
	InputProperty::OnLoseFocus();

	// remove input sink
    Input::Server* inputServer = Input::Server::Instance();

	inputServer->RemoveInputSink("pause", this );	
}

//------------------------------------------------------------------------------
/**
    This method is inherited from the Port class. If your property acts as
    a message handler you must implement the Accepts() method to return
    true for each message that is accepted. By default no messages are accepted.
*/
bool PauseInputProperty::Accepts(Message::Msg* msg)
{
	n_assert(msg);
	
	// accept input message
	if( msg->CheckId(Input::Event::Id) ) {
		return true;
	}

	// not here...
    return( Message::Port::Accepts(msg) );
}

//------------------------------------------------------------------------------
/**
    This method is inherited from the Port class. If your property acts as
    a message handler you must implement the HandleMessage() method to
    process the incoming messages. Please note that HandleMessage() will
    not be called "automagically" because the Property doesn't know at which
    point in the frame you want to handle pending messages.

    Thus, you must call the HandlePendingMessages() yourself from either OnBeginFrame(),
    OnMoveBefore(), OnMoveAfter() or OnRender().

    The simple rule is: if you override the Accepts() method, you must also call
    HandlePendingMessages() either in OnBeginFrame(), OnMoveBefore(), OnMoveAfter()
    or OnRender()
*/
// NOTA : HandlePendingMessages is already call from Property::OnFrame
void PauseInputProperty::HandleMessage(Message::Msg* msg)
{
    n_assert(msg);
   
	if( this->Accepts( msg ) == false ) {
		return;
	}

	// check for input msg
	if (msg->CheckId(Input::Event::Id))
    {
        Event* event = static_cast<Event*>(msg);
   
		// NOTA : there only one state valid at a time, beside, selecting the next state
		// is the job of the app (unique too). Using messages would be stupid. So to 
		// switch game state (game<-->pause), let's directly call the app

		switch( event->GetType() ) {
			case Input::Event::Type::ButtonDown :
			{
				const nString& name = event->GetMapping()->GetName();

				if( name == "pause" ) {
					// inform the app we want this state.
					// NOTA : no assert should be made : it's not because we
					// request it that we will actually get it...
					mTutorial::mTestApp::Instance()->RequestState( "GameState" );	
					
					// inform other listener this message was handled
					event->SetHandled();			
					return;					
				}				
			}
			
			default :
				break;
		}
		
		return;
    }

    Message::Port::HandleMessage(msg);
}

}; // namespace Properties
