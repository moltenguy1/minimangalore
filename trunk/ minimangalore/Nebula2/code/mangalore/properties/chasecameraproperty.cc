//------------------------------------------------------------------------------
//  properties/chasecameraproperty.cc
//  (C) 2005 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "properties/chasecameraproperty.h"
#include "game/time/inputtimesource.h"
#include "managers/focusmanager.h"
#include "game/entity.h"
#include "graphics/server.h"
#include "graphics/cameraentity.h"
#include "msg/cameraorbit.h"
#include "msg/camerareset.h"
#include "msg/cameradistance.h"
#include "properties/physicsproperty.h"
#include "physics/filterset.h"
#include "physics/physicsutil.h"

// chase camera specific attributes
namespace Attr
{
    DefineFloat(CameraDistance);
    DefineFloat(CameraMinDistance);
    DefineFloat(CameraMaxDistance);
    DefineFloat(CameraDistanceStep);
    DefineVector3(CameraOffset);
    DefineFloat(CameraLowStop);
    DefineFloat(CameraHighStop);
    DefineFloat(CameraAngularVelocity);
    DefineFloat(CameraLinearGain);
    DefineFloat(CameraAngularGain);
    DefineFloat(CameraDefaultTheta);
}

namespace Properties
{
ImplementRtti(Properties::ChaseCameraProperty, Properties::CameraProperty);
ImplementFactory(Properties::ChaseCameraProperty);

using namespace Game;
using namespace Message;
using namespace Managers;

//------------------------------------------------------------------------------
/**
*/
ChaseCameraProperty::ChaseCameraProperty() :
    cameraDistance(0.0f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ChaseCameraProperty::~ChaseCameraProperty()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    This adds the default attributes to the property.
*/
void
ChaseCameraProperty::SetupDefaultAttributes()
{
    CameraProperty::SetupDefaultAttributes();

    GetEntity()->SetFloat(Attr::CameraDistance, 7.0f);
    GetEntity()->SetFloat(Attr::CameraMinDistance, 1.5f);
    GetEntity()->SetFloat(Attr::CameraMaxDistance, 15.0f);
    GetEntity()->SetVector3(Attr::CameraOffset, vector3(0.0f, 1.5f, 0.0f));
    GetEntity()->SetFloat(Attr::CameraLowStop, -30.0f);
    GetEntity()->SetFloat(Attr::CameraHighStop, 30.0f);
    GetEntity()->SetFloat(Attr::CameraAngularVelocity, 6.0f);
    GetEntity()->SetFloat(Attr::CameraDistanceStep, 1.0f);
    GetEntity()->SetFloat(Attr::CameraLinearGain, -10.0f);
    GetEntity()->SetFloat(Attr::CameraAngularGain, -15.0f);
    GetEntity()->SetFloat(Attr::CameraDefaultTheta, n_deg2rad(-20.0f));
}

//------------------------------------------------------------------------------
/**
    This method is called once when the property is attached to the entity.
*/
void
ChaseCameraProperty::OnActivate()
{
    CameraProperty::OnActivate();

    // reset the camera
    this->HandleCameraReset();
}

//------------------------------------------------------------------------------
/**
    This method is called when the entity obtains the camera focus.
*/
void
ChaseCameraProperty::OnObtainFocus()
{
    // initialize the feedback loops with the current camera values so
    // that we get a smooth interpolation to the new position
    Graphics::CameraEntity* camera = Graphics::Server::Instance()->GetCamera();
    const matrix44& m = camera->GetTransform();
    nTime time = InputTimeSource::Instance()->GetTime();
    this->cameraPos.Reset(time, 0.0001f, GetEntity()->GetFloat(Attr::CameraLinearGain), m.pos_component());
    this->cameraLookat.Reset(time, 0.0001f, GetEntity()->GetFloat(Attr::CameraAngularGain), m.pos_component() - (m.z_component() * 10.0f));

    CameraProperty::OnObtainFocus();
}

//------------------------------------------------------------------------------
/**
    This method is called before the camera is "rendered".
*/
void
ChaseCameraProperty::OnRender()
{
    // handle pending camera messages, these are usually generated by the input property
    this->HandlePendingMessages();

    // only do something if we have the camera focus
    if (FocusManager::Instance()->GetCameraFocusEntity() == GetEntity())
    {
        // update the camera
        this->UpdateCamera();
    }

    // important: call parent class
    CameraProperty::OnRender();
}

//------------------------------------------------------------------------------
/**
    This method checks if we accept the given message.
*/
bool
ChaseCameraProperty::Accepts(Message::Msg* msg)
{
    return msg->CheckId(CameraOrbit::Id) ||
           msg->CheckId(CameraReset::Id) ||
           msg->CheckId(CameraDistance::Id);
}

//------------------------------------------------------------------------------
/**
    This method handles pending messages for the property. It is called
    by the OnRender() method.
*/
void
ChaseCameraProperty::HandleMessage(Message::Msg* msg)
{
    if (msg->CheckId(CameraOrbit::Id))
    {
        CameraOrbit* cameraOrbitMsg = (CameraOrbit*) msg;
        this->HandleCameraOrbit(cameraOrbitMsg->GetHorizontalRotation(), cameraOrbitMsg->GetVerticalRotation());
    }
    else if (msg->CheckId(CameraReset::Id))
    {
        this->HandleCameraReset();
    }
    else if (msg->CheckId(CameraDistance::Id))
    {
        CameraDistance* cameraDistanceMsg = (CameraDistance*) msg;
        this->HandleCameraDistanceChange(cameraDistanceMsg->GetRelativeDistanceChange());
    }
}

//------------------------------------------------------------------------------
/**
    Handle a camera distance change.
*/
void
ChaseCameraProperty::HandleCameraDistanceChange(float d)
{
    float minDist  = GetEntity()->GetFloat(Attr::CameraMinDistance);
    float maxDist  = GetEntity()->GetFloat(Attr::CameraMaxDistance);
    float zoomStep = GetEntity()->GetFloat(Attr::CameraDistanceStep);
    this->cameraDistance = n_clamp(this->cameraDistance + (d * zoomStep), minDist, maxDist);
}

//------------------------------------------------------------------------------
/**
    Handle a camera reset.
*/
void
ChaseCameraProperty::HandleCameraReset()
{
    float curTheta = GetEntity()->GetFloat(Attr::CameraDefaultTheta);
    const matrix44& m = GetEntity()->GetMatrix44(Attr::Transform);
    this->cameraAngles.set(m.z_component());
    this->cameraAngles.theta = curTheta;
    this->cameraDistance = GetEntity()->GetFloat(Attr::CameraDistance);
}

//------------------------------------------------------------------------------
/**
    Handle a camera orbit.
*/
void
ChaseCameraProperty::HandleCameraOrbit(float dRho, float dTheta)
{
    float angularVelocity = GetEntity()->GetFloat(Attr::CameraAngularVelocity);
    float lowStop = n_deg2rad(GetEntity()->GetFloat(Attr::CameraLowStop));
    float hiStop  = n_deg2rad(GetEntity()->GetFloat(Attr::CameraHighStop));

    float frameTime = (float)InputTimeSource::Instance()->GetFrameTime();
    this->cameraAngles.rho += dRho * angularVelocity * frameTime;
    this->cameraAngles.theta += dTheta * angularVelocity * frameTime;
    this->cameraAngles.theta = n_clamp(this->cameraAngles.theta, lowStop, hiStop);
}

//------------------------------------------------------------------------------
/**
    Do a ray check between 'from' and 'to' and return a replacement
    point for 'to'.
*/
vector3
ChaseCameraProperty::DoCollideCheck(const vector3& from, const vector3& to)
{
    static const vector3 up(0.0f, 1.0f, 0.0f);
    matrix44 m;
    m.set_translation(from);
    m.lookatRh(to, up);

    // setup the exclude set for the ray check
    float outContactDist;
    Physics::FilterSet excludeSet;
    AbstractPhysicsProperty* physProp = (AbstractPhysicsProperty*) GetEntity()->FindProperty(AbstractPhysicsProperty::RTTI);
    if (physProp && physProp->IsEnabled())
    {
        Physics::Entity* phyEntity = physProp->GetPhysicsEntity();
        if (phyEntity)
        {
            excludeSet.AddEntityId(phyEntity->GetUniqueId());
        }
    }
    Physics::PhysicsUtil::RayBundleCheck(from, to, up, m.x_component(), 0.25f, excludeSet, outContactDist);

    vector3 vec = to - from;
    vec.norm();
    vector3 newTo = from + vec * outContactDist;

    return newTo;
}

//------------------------------------------------------------------------------
/**
    Update the camera position and orientation from the current
    orbit polar angles.
*/
void
ChaseCameraProperty::UpdateCamera()
{
    Graphics::CameraEntity* camera = Graphics::Server::Instance()->GetCamera();
    n_assert(camera != 0);
    static const vector3 upVec(0.0f, 1.0f, 0.0f);

    // compute the lookat point in global space
    const matrix44& m44 = GetEntity()->GetMatrix44(Attr::Transform);
    matrix33 m33 = matrix33(m44.x_component(), m44.y_component(), m44.z_component());
    vector3 lookatPoint = m44.pos_component() + m33 * GetEntity()->GetVector3(Attr::CameraOffset);

    // compute the collided goal position
    matrix44 orbitMatrix;
    orbitMatrix.rotate_x(this->cameraAngles.theta);
    orbitMatrix.rotate_y(this->cameraAngles.rho);
    orbitMatrix.translate(lookatPoint);
    vector3 goalPos = orbitMatrix.pos_component() + orbitMatrix.z_component() * this->cameraDistance;
    goalPos = this->DoCollideCheck(lookatPoint, goalPos);

    // check if the camera is currently at the origin, if yes it is in its initial
    // position and should not interpolate towards its target position
    const vector3& camPos = camera->GetTransform().pos_component();
    if (camPos.isequal(vector3::zero, 0.0f))
    {
        nTime time = InputTimeSource::Instance()->GetTime();
        this->cameraPos.Reset(time, 0.0001f, GetEntity()->GetFloat(Attr::CameraLinearGain), goalPos);
        this->cameraLookat.Reset(time, 0.0001f, GetEntity()->GetFloat(Attr::CameraAngularGain), lookatPoint);
    }

    // feed and update the feedback loops
    nTime time = InputTimeSource::Instance()->GetTime();
    this->cameraPos.SetGoal(goalPos);
    this->cameraLookat.SetGoal(lookatPoint);
    this->cameraPos.Update(time);
    this->cameraLookat.Update(time);

    // construct the new camera matrix
    matrix44 cameraMatrix;
    cameraMatrix.translate(this->cameraPos.GetState());
    cameraMatrix.lookatRh(this->cameraLookat.GetState(), upVec);

    // update the graphics subsystem camera
    camera->SetTransform(cameraMatrix);
}

} // namespace Properties
