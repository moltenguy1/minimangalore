//------------------------------------------------------------------------------
//  properties/inputproperty.cc
//  (C) je.a.le@wanadoo.fr
//------------------------------------------------------------------------------
#include "mtutorial04/property/polarcamerainputproperty.h"

#include "properties/inputproperty.h"
#include "managers/focusmanager.h"
#include "game/entity.h"
#include "input/server.h"
#include "input/event.h"
#include "input/mapping.h"
#include "input/priority.h"
#include "input/source.h"
#include "foundation/factory.h" 

#include "mtutorial04/message/polarsystemvalues.h"
#include "mtutorial04/mTestapp.h"

namespace Properties
{
ImplementRtti(Properties::PolarCameraInputProperty, Properties::InputProperty);
ImplementFactory(Properties::PolarCameraInputProperty);

using namespace Message;
using namespace Game;
using namespace Managers;
using namespace Input;

//------------------------------------------------------------------------------
PolarCameraInputProperty::PolarCameraInputProperty()
{
    // empty
}

//------------------------------------------------------------------------------
PolarCameraInputProperty::~PolarCameraInputProperty()
{
    // empty
}

//------------------------------------------------------------------------------
/**
/// setup default entity attributes
*/
void PolarCameraInputProperty::SetupDefaultAttributes()
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
PolarCameraInputProperty::OnObtainFocus()
{
	InputProperty::OnObtainFocus();

	// add input sink
    Input::Server* inputServer = Input::Server::Instance();
	
	// first, if mapping not already set (from startup script...), does it
	if( inputServer->HasMapping( "vwrLeft" ) == false ) {
		inputServer->AddMapping("vwrLeft", "relmouse0:-x");
		inputServer->AddMapping("vwrRight", "relmouse0:+x");
		inputServer->AddMapping("vwrUp", "relmouse0:+y");
		inputServer->AddMapping("vwrDown", "relmouse0:-y");
		inputServer->AddMapping("zoomOut", "relmouse0:+zbtn");
		inputServer->AddMapping("zoomIn", "relmouse0:-zbtn");
		inputServer->AddMapping("moveForward", "keyb0:up");
		inputServer->AddMapping("moveBackward", "keyb0:down");
		inputServer->AddMapping("turnLeft", "keyb0:left");
		inputServer->AddMapping("turnRight", "keyb0:right");
		inputServer->AddMapping("pause", "keyb0:p");
		
		// to use the "pause" key :
		//inputServer->AddMapping("pause", "keyb0:pause");
		// but cause of a directx bug, you must comment the line
		// code/nebula2/src/input/nkeyboarddevice.cc line #42
	}

	// attach sink, when this mapping is set, input server will infor this property.
	inputServer->AttachInputSink("vwrLeft", Input::InputPriority_MousePositionTracking, this );	
	inputServer->AttachInputSink("vwrRight", Input::InputPriority_MousePositionTracking, this );	
	inputServer->AttachInputSink("vwrUp", Input::InputPriority_MousePositionTracking, this );	
	inputServer->AttachInputSink("vwrDown", Input::InputPriority_MousePositionTracking, this );	
	inputServer->AttachInputSink("zoomOut", Input::InputPriority_MousePositionTracking, this );	
	inputServer->AttachInputSink("zoomIn", Input::InputPriority_MousePositionTracking, this );	
	
	inputServer->AttachInputSink("pause", Input::InputPriority_DefaultGamePriority, this );	   
}

//------------------------------------------------------------------------------
/**
    This method is called by the FocusManager when our entity loses
    the camera focus. Override this method if your subclass needs to do any
    cleanup work here.
*/
void PolarCameraInputProperty::OnLoseFocus()
{
	InputProperty::OnLoseFocus();

	// remove input sink
    Input::Server* inputServer = Input::Server::Instance();

	inputServer->RemoveInputSink("vwrLeft", this );	
	inputServer->RemoveInputSink("vwrRight", this );	
	inputServer->RemoveInputSink("vwrUp", this );	
	inputServer->RemoveInputSink("vwrDown", this );	
	inputServer->RemoveInputSink("zoomOut", this );	
	inputServer->RemoveInputSink("zoomIn", this );		
	inputServer->RemoveInputSink("pause", this );	
}

//------------------------------------------------------------------------------
/**
    This method is inherited from the Port class. If your property acts as
    a message handler you must implement the Accepts() method to return
    true for each message that is accepted. By default no messages are accepted.
*/
bool PolarCameraInputProperty::Accepts(Message::Msg* msg)
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
void PolarCameraInputProperty::HandleMessage(Message::Msg* msg)
{
    n_assert(msg);
   
	// check for input msg
	if (msg->CheckId(Input::Event::Id))
    {
        Event* event = static_cast<Event*>(msg);
   
		// create a new message
 		Ptr<PolarSystemValues> message = Message::PolarSystemValues::Create(); 			
 		message->SetUseDynamicsValues();

		switch( event->GetType() ) {
			case Input::Event::Type::AxisMoved :
			{
				const nString& name = event->GetMapping()->GetName();

				if( name == "vwrLeft" ) {				
					message->SetTheta( event->GetAxisValue() );					
					break;
				}
				
				if( name == "vwrRight" ) {
					message->SetTheta( 0 - event->GetAxisValue() );					
					break;					
				}

				if( name == "vwrUp" ) {
					message->SetPhi( event->GetAxisValue() );					
					break;
				}
				
				if( name == "vwrDown" ) {
					message->SetPhi( 0 - event->GetAxisValue() );					
					break;
				}
				
				// not found
				break;
			}

			case Input::Event::Type::ButtonDown :
			{
				const nString& name = event->GetMapping()->GetName();

				if( name == "zoomOut" ) {
					message->SetDistance( 5.0 );					
					break;					
				}
				
				if( name == "zoomIn" ) {
					message->SetDistance( -5.0 );					
					break;
				}
		
				if( name == "pause" ) {
					// inform the app we want this state.
					// NOTA : no assert should be made : it's not because we
					// request it that we will actually get it...
					mTutorial::mTestApp::Instance()->RequestState( "PauseState" );		
					
					// inform other listener this message was handled
					event->SetHandled();		
					return;					
				}				

				break;
			}
			
			default :
				break;
		}
		
		// broadcast only if at least one value set
		if( message->DataIsSet() ) {
			// broadcast the message. the first listener that undersand it 
			// (here extcameraproperty) will use it. with SendAsync it's possible
			// to target an only one listener. Using Sync call the listener would stop
			// is word to parse the message, the result could be undtermined....
			message->BroadcastAsync();
			
			// inform other listener this message was handled
			event->SetHandled();
			return;
		}

		// not handled, continue...		
    }

    Message::Port::HandleMessage(msg);
}

}; // namespace Properties
