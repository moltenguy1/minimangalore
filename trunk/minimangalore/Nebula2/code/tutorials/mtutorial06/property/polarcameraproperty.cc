//------------------------------------------------------------------------------
//  properties/Polarcameraproperty.cc
//  (C) je.a.le@wanadoo.fr
//------------------------------------------------------------------------------
#include "mtutorial06/property/polarcameraproperty.h"

#include "managers/focusmanager.h"
#include "managers/entitymanager.h"
#include "graphics/server.h"
#include "graphics/cameraentity.h"
#include "graphics/entity.h"
#include "game/entity.h"

// handler some math functions nmath.h don't....
#include "mtutorial06/mathext.h"

// the message class used for communication
#include "mtutorial06/message/polarsystemvalues.h"

// Polar camera specific attributes
namespace Attr
{
 	DefineVector3(PolarCameraLookAt);		// where to look at
    DefineFloat(PolarCameraPhi);			// phi angle
	DefineFloat(PolarCameraTheta);		// theta angle
	DefineFloat(PolarCameraDistance)		// distance fron look at
};

namespace Properties
{
ImplementRtti(Properties::PolarCameraProperty, Properties::CameraProperty);
ImplementFactory(Properties::PolarCameraProperty);

using namespace Game;
using namespace Managers;
using namespace Message;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
PolarCameraProperty::PolarCameraProperty()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
PolarCameraProperty::~PolarCameraProperty()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void PolarCameraProperty::OnRender()
{
    if (FocusManager::Instance()->GetCameraFocusEntity() == GetEntity())
    {
        // update the graphics subsystem's camera transform
 		this->m.ident();
		this->m.translate( vector3( 0.0f, 0.0f, GetEntity()->GetFloat( Attr::PolarCameraDistance ) ) );
		this->m.rotate_x( n_deg2rad( GetEntity()->GetFloat( Attr::PolarCameraPhi ) ) );
		this->m.rotate_y( n_deg2rad( GetEntity()->GetFloat( Attr::PolarCameraTheta ) ) );        
		this->m.translate( GetEntity()->GetVector3( Attr::PolarCameraLookAt ) );

        Graphics::CameraEntity* camera = Graphics::Server::Instance()->GetCamera();
        n_assert(camera != 0);
        camera->SetTransform(this->m);
    }

    // important: call parent class at the end to apply any shake effects
    CameraProperty::OnRender();
}
//------------------------------------------------------------------------------
/**
    This adds the default attributes to the property. (and set the Attr... )
*/
void PolarCameraProperty::SetupDefaultAttributes()
{
    CameraProperty::SetupDefaultAttributes();

	this->m.ident();
	GetEntity()->SetFloat( Attr::PolarCameraTheta, 0.0f );
	GetEntity()->SetFloat( Attr::PolarCameraPhi, 0.0f );
	GetEntity()->SetFloat( Attr::PolarCameraDistance, 100.0f );
	GetEntity()->SetVector3( Attr::PolarCameraLookAt, vector3.zero );
}

//------------------------------------------------------------------------------
/**
    This method is inherited from the Port class. If your property acts as
    a message handler you must implement the Accepts() method to return
    true for each message that is accepted. By default no messages are accepted.
*/
bool PolarCameraProperty::Accepts(Message::Msg* msg)
{
	n_assert(msg);
	
	// accept input message
	if( msg->CheckId(Message::PolarSystemValues::Id) ) {
		return true;
	}

	// let's see if message inherit
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
void PolarCameraProperty::HandleMessage(Message::Msg* msg)
{
    n_assert(msg);
   
	// ok, handled but which type ( here won't care about inherit message )
	if( msg->CheckId( Message::PolarSystemValues::Id ) ) {
		PolarSystemValues *	val = static_cast<PolarSystemValues *>(msg);
	
		// check for each value
		if( val->GetMask() && PolarSystemValues::M_PHI ) {
			GetEntity()->SetFloat( Attr::PolarCameraPhi, 
				MathExt::clampVal( -90.0f, 0.0f, 				
				( val->GetMask() && PolarSystemValues::M_DYNAMIC ? 
					GetEntity()->GetFloat( Attr::PolarCameraPhi ) + val->GetPhi() :
					val->GetPhi() )				
				 ) );
		}
	
		if( val->GetMask() && PolarSystemValues::M_THETA ) {
			GetEntity()->SetFloat( Attr::PolarCameraTheta, 
				MathExt::rotateVal( 0.0f, 360.0f, 
				( val->GetMask() && PolarSystemValues::M_DYNAMIC ? 
					GetEntity()->GetFloat( Attr::PolarCameraTheta ) + val->GetTheta() :
					val->GetTheta() )				
				 ) );
		}
	
		if( val->GetMask() && PolarSystemValues::M_DISTANCE ) {
			GetEntity()->SetFloat( Attr::PolarCameraDistance, 
				MathExt::clampVal( 0.0f, 100000.0,
				( val->GetMask() && PolarSystemValues::M_DYNAMIC ? 
					GetEntity()->GetFloat( Attr::PolarCameraDistance ) + val->GetDistance() :
					val->GetDistance() )				
				 ) );
		}
	
		if( val->GetMask() && PolarSystemValues::M_LOOKAT ) {
			GetEntity()->SetVector3( Attr::PolarCameraLookAt, 
				( val->GetMask() && PolarSystemValues::M_DYNAMIC ? 
					GetEntity()->GetVector3( Attr::PolarCameraLookAt ) + val->GetLookAt() :
					val->GetLookAt() )				
				);
		}
	
		return;
	}

	// not here ! pass to npolar...
    Message::Port::HandleMessage(msg);
}

}; // namespace Properties
