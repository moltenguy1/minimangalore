//----------------------------------------------------------------------------
//  tway_main.cc
//
//  (C)2007 kaikai
//----------------------------------------------------------------------------

#include "foundation/ptr.h"

#include "graphics/server.h"

#include "kernel/nfileserver2.h"

#include "managers/factorymanager.h"

#include "game/entity.h"
#include "input/server.h"

#include "msg/cameraorbit.h"
#include "msg/camerareset.h"

#include "properties/graphicsproperty.h"
#include "properties/lightproperty.h"
#include "properties/transformableproperty.h"
#include "properties/environmentgraphicsproperty.h"
#include "properties/environmentcollideproperty.h"
#include "properties/physicsproperty.h"
#include "properties/videocameraproperty.h"

#include "physics/meshshape.h"
#include "physics/capsuleshape.h"
#include "physics/sphereshape.h"
#include "physics/composite.h"
#include "physics/hinge2joint.h"

#include "util/segmentedgfxutil.h"

#include "ueplayercameraproperty.h"
#include "attributes/ueattributes.h"

namespace Properties
{
ImplementRtti(Properties::UePlayerCameraProperty, Properties::ChaseCameraProperty);
ImplementFactory(Properties::UePlayerCameraProperty);


UePlayerCameraProperty::UePlayerCameraProperty()
{

}

UePlayerCameraProperty::~UePlayerCameraProperty()
{

}

void UePlayerCameraProperty::SetupDefaultAttributes()
{
	CameraProperty::SetupDefaultAttributes();

	GetEntity()->SetFloat(Attr::CameraDistance, 0.0f);
	GetEntity()->SetFloat(Attr::CameraMinDistance, 0.0f);
	GetEntity()->SetFloat(Attr::CameraMaxDistance, 0.0f);
	GetEntity()->SetVector3(Attr::CameraOffset, vector3(0.0f, 0.0f, 0.0f));
	GetEntity()->SetFloat(Attr::CameraLowStop, 0.0f);
	GetEntity()->SetFloat(Attr::CameraHighStop, 0.0f);
	GetEntity()->SetFloat(Attr::CameraAngularVelocity, 0.0f);
	GetEntity()->SetFloat(Attr::CameraDistanceStep, 0.0f);
	GetEntity()->SetFloat(Attr::CameraLinearGain, 0.0f);
	GetEntity()->SetFloat(Attr::CameraAngularGain,0.0f);
	GetEntity()->SetFloat(Attr::CameraDefaultTheta, n_deg2rad(0.0f));


	// stand
	poseCamera[0].rotate_x(n_deg2rad(4.5f));
	poseCamera[0].translate(vector3(0.0f,0.4f,0.95f));	

	// crounch
	poseCamera[1].rotate_x(n_deg2rad(5.0f));
	poseCamera[1].translate(vector3(0.0f,0.3f,0.6f));	

	// prone
	poseCamera[2].rotate_x(n_deg2rad(5.5f));
	poseCamera[2].translate(vector3(0.0f,0.2f,0.45f));		

	onlyStandPose = false;
}

void
UePlayerCameraProperty::UpdateCamera()
{
	Graphics::CameraEntity* camera = Graphics::Server::Instance()->GetCamera();
	n_assert(camera != 0);
	static const vector3 upVec(0.0f, 1.0f, 0.0f);

	// compute the lookat point in global space
	const matrix44& m44 = GetEntity()->GetMatrix44(Attr::Transform);	
	int	poseCurr = GetEntity()->GetInt(Attr::pose);

	matrix44 newmatrix;

	newmatrix = m44;

	if (onlyStandPose)
	{
		newmatrix = poseCamera[0] * newmatrix;
	}
	else
	{
		newmatrix = poseCamera[poseCurr] * newmatrix;
	}
	
	
	// update the graphics subsystem camera
	camera->SetTransform(newmatrix);
}

}
//----------------------------------------------------------------------------
// EOF
//----------------------------------------------------------------------------
