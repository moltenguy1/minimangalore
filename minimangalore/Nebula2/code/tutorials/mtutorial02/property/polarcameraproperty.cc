//------------------------------------------------------------------------------
//  properties/Polarcameraproperty.cc
//  (C) je.a.le@wanadoo.fr
//------------------------------------------------------------------------------
#include "mtutorial02/property/polarcameraproperty.h"

#include "managers/focusmanager.h"
#include "managers/entitymanager.h"
#include "graphics/server.h"
#include "graphics/cameraentity.h"
#include "graphics/entity.h"
#include "game/entity.h"

#include "properties/transformableproperty.h"
#include "properties/graphicsproperty.h"

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

}; // namespace Properties
