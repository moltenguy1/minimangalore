//------------------------------------------------------------------------------
//  properties/inputproperty.cc
//  (C) je.a.le@wanadoo.fr
//------------------------------------------------------------------------------
#include "mtutorial05/property/opelinputproperty.h"

#include "properties/inputproperty.h"
#include "managers/focusmanager.h"
#include "game/entity.h"
#include "input/server.h"
#include "input/event.h"
#include "input/mapping.h"
#include "input/priority.h"
#include "input/source.h"
#include "foundation/factory.h" 

#include "mtutorial05/message/polarsystemvalues.h"
#include "mtutorial05/message/opelcontrolvalues.h"
#include "mtutorial05/mTestapp.h"

namespace Properties
{
ImplementRtti(Properties::OpelInputProperty, Properties::InputProperty);
ImplementFactory(Properties::OpelInputProperty);

using namespace Message;
using namespace Game;
using namespace Managers;
using namespace Input;

//------------------------------------------------------------------------------
OpelInputProperty::OpelInputProperty()
{
    // empty
}

//------------------------------------------------------------------------------
OpelInputProperty::~OpelInputProperty()
{
    // empty
}

//------------------------------------------------------------------------------
/**
/// setup default entity attributes
*/
void OpelInputProperty::SetupDefaultAttributes()
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
OpelInputProperty::OnObtainFocus()
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
		
		// control for the opelblitz
		inputServer->AddMapping("moveForward", "keyb0:up");
		inputServer->AddMapping("moveBackward", "keyb0:down");
		inputServer->AddMapping("turnLeft", "keyb0:left");
		inputServer->AddMapping("turnRight", "keyb0:right");
	}

	// attach sink, when this mapping is set, input server will infor this property.
	inputServer->AttachInputSink("vwrLeft", Input::InputPriority_MousePositionTracking, this );	
	inputServer->AttachInputSink("vwrRight", Input::InputPriority_MousePositionTracking, this );	
	inputServer->AttachInputSink("vwrUp", Input::InputPriority_MousePositionTracking, this );	
	inputServer->AttachInputSink("vwrDown", Input::InputPriority_MousePositionTracking, this );	
	inputServer->AttachInputSink("zoomOut", Input::InputPriority_MousePositionTracking, this );	
	inputServer->AttachInputSink("zoomIn", Input::InputPriority_MousePositionTracking, this );	
	
	inputServer->AttachInputSink("moveForward", Input::InputPriority_DefaultGamePriority, this );	   
	inputServer->AttachInputSink("moveBackward", Input::InputPriority_DefaultGamePriority, this );	   
	inputServer->AttachInputSink("turnLeft", Input::InputPriority_DefaultGamePriority, this );	   
	inputServer->AttachInputSink("turnRight", Input::InputPriority_DefaultGamePriority, this );	 
}

//------------------------------------------------------------------------------
/**
    This method is called by the FocusManager when our entity loses
    the camera focus. Override this method if your subclass needs to do any
    cleanup work here.
*/
void OpelInputProperty::OnLoseFocus()
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
	
	inputServer->RemoveInputSink("moveForward", this );	   
	inputServer->RemoveInputSink("moveBackward", this );	   
	inputServer->RemoveInputSink("turnLeft", this );	   
	inputServer->RemoveInputSink("turnRight", this );	   
}

//------------------------------------------------------------------------------
/**
    This method is inherited from the Port class. If your property acts as
    a cam handler you must implement the Accepts() method to return
    true for each cam that is accepted. By default no cams are accepted.
*/
bool OpelInputProperty::Accepts(Message::Msg* msg)
{
	n_assert(msg);
	
	// accept input message ; only from keyboard/mouse
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
    not be called "automatically" because the Property doesn't know at which
    point in the frame you want to handle pending messages.

    Thus, you must call the HandlePendingMessages() yourself from either OnBeginFrame(),
    OnMoveBefore(), OnMoveAfter() or OnRender().

    The simple rule is: if you override the Accepts() method, you must also call
    HandlePendingMessages() either in OnBeginFrame(), OnMoveBefore(), OnMoveAfter()
    or OnRender()
*/
// NOTA : HandlePendingMessages is already call from Property::OnFrame
void OpelInputProperty::HandleMessage(Message::Msg* msg)
{
    n_assert(msg);
   
	// check for input msg
	if (msg->CheckId(Input::Event::Id))
    {
        Event* event = static_cast<Event*>(msg);
   
		// create a new message for the camera
 		Ptr<PolarSystemValues> cam = Message::PolarSystemValues::Create(); 			
 		cam->SetUseDynamicsValues();

		// and a message for the opel 
		Ptr<OpelControlValues> opel = Message::OpelControlValues::Create();
		
		switch( event->GetType() ) {
			case Input::Event::Type::AxisMoved :
			{
				const nString& name = event->GetMapping()->GetName();

				if( name == "vwrLeft" ) {				
					cam->SetTheta( event->GetAxisValue() );					
					break;
				}
				
				if( name == "vwrRight" ) {
					cam->SetTheta( 0 - event->GetAxisValue() );					
					break;					
				}

				if( name == "vwrUp" ) {
					cam->SetPhi( event->GetAxisValue() );					
					break;
				}
				
				if( name == "vwrDown" ) {
					cam->SetPhi( 0 - event->GetAxisValue() );					
					break;
				}
				
				// not found
				break;
			}

			case Input::Event::Type::ButtonDown :
			{
				const nString& name = event->GetMapping()->GetName();

				if( name == "zoomOut" ) {
					cam->SetDistance( 5.0 );					
					break;					
				}
				
				if( name == "zoomIn" ) {
					cam->SetDistance( -5.0 );					
					break;
				}
		
				// !!! values are directly from the 3d model
				if( name == "moveForward" ) {
					opel->ApplySpeedVelocity( -20.0f );
					break;					
				}				

				if( name == "moveBackward" ) {
					opel->ApplySpeedVelocity( 20.0f );
					break;					
				}				
				if( name == "turnLeft" ) {
					opel->ApplySteeringVelocity( -3.0f );				
					break;					
				}				
				if( name == "turnRight" ) {
					opel->ApplySteeringVelocity( 3.0f );				
					break;					
				}				

				break;
			}
			
			case Input::Event::ButtonUp :
			{
				const nString& name = event->GetMapping()->GetName();
				
				if( name == "moveForward" ) {
					opel->ApplySpeedVelocity( 0.0f );
					break;					
				}				

				if( name == "moveBackward" ) {
					opel->ApplySpeedVelocity( 0.0f );
					break;	
				}				
				
				if( name == "turnLeft" ) {
					opel->ApplySteeringVelocity( 0.0f );				
					break;					
				}		
						
				if( name == "turnRight" ) {
					opel->ApplySteeringVelocity( 0.0f );				
					break;					
				}				
			}
			
			default :
				break;
		}
		
		// NOTA : a message handle only one info, so it will be cam or opel...
		
		// broadcast only if at least one value set
		if( (cam->GetMask() & PolarSystemValues::M_MASK) ) {
			// broadcast the message. the first listener that undersand it 
			// (here extcameraproperty) will use it. with SendAsync it's possible
			// to target an only one listener. Using Sync call the listener would stop
			// is work to parse the cam, the result could be undetermined....
			cam->BroadcastAsync();		
			event->SetHandled();
			return;
		} 
		
		// broadcast only if at least one value set
		if( (opel->GetMask() & OpelControlValues::M_MASK) ) {
			opel->BroadcastAsync();
			event->SetHandled();
			return;
		}		
    }

    Message::Port::HandleMessage(msg);
}

}; // namespace Properties
