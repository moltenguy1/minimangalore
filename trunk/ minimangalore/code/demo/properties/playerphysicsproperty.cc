//------------------------------------------------------------------------------
//  properties/playerphysicsproperty.cc
//  (C) 2007 Kim Hyoun Woo
//------------------------------------------------------------------------------
#include "properties/playerphysicsproperty.h"
#include "foundation/factory.h"
#include "attr/attributes.h"
#include "physics/server.h"
#include "physics/level.h"
#include "physics/rigidbody.h"
#include "msg/playerstatus.h"
#include "msg/updatetransform.h"

namespace Properties
{
ImplementRtti(Properties::PlayerPhysicsProperty, Properties::AbstractPhysicsProperty);
ImplementFactory(Properties::PlayerPhysicsProperty);

using namespace Game;
using namespace Message;
using namespace Managers;

//------------------------------------------------------------------------------
/**
*/
PlayerPhysicsProperty::PlayerPhysicsProperty()
{
}

//------------------------------------------------------------------------------
/**
*/
PlayerPhysicsProperty::~PlayerPhysicsProperty()
{
}

//------------------------------------------------------------------------------
/**
*/
void
PlayerPhysicsProperty::SetupDefaultAttributes()
{
	AbstractPhysicsProperty::SetupDefaultAttributes();

}

//------------------------------------------------------------------------------
/**
*/
bool
PlayerPhysicsProperty::Accepts(Message::Msg* msg)
{
	return msg->CheckId(PlayerStatus::Id) || 
           AbstractPhysicsProperty::Accepts(msg);
}

//------------------------------------------------------------------------------
/**
*/
void
PlayerPhysicsProperty::HandleMessage(Message::Msg* msg)
{
    if (!this->IsEnabled())
    {
        // disabled, don't listen to messages
        AbstractPhysicsProperty::HandleMessage(msg);
    }
	else
    if (msg->CheckId(PlayerStatus::Id))
	{
        this->HandlePlayerAction( (PlayerStatus*)msg );
	}
	else
	{
        AbstractPhysicsProperty::HandleMessage(msg);
	}
}

//------------------------------------------------------------------------------
/**
    The OnMoveBefore() method handles all pending messages, and other
    per-frame-stuff that must happen before the physics subsystem is
    triggered.
*/
void
PlayerPhysicsProperty::OnMoveBefore()
{
    this->HandlePendingMessages();
    if (this->IsEnabled())
    {
	}
}

//------------------------------------------------------------------------------
/**
*/
void 
PlayerPhysicsProperty::HandlePlayerAction(PlayerStatus* msg)
{
	Physics::RigidBody* body = this->playerPhysicsEntity->GetRigidBody();
	float mass = body->GetMass();

	float runForce = 7 * mass;
    float const turnTorque = 7 * mass;
	
	float ff, lf, rTurn;
	ff = lf = rTurn = 0.0f;

	ff += msg->fb; // forward/backward
	lf += msg->lr; // left/right
	rTurn += msg->turn * turnTorque/*const*/;

    float l = sqrtf( ff*ff + lf*lf );
    if( l > 0 ) 
	{
      ff /= l;
      lf /= l;
    }

	float fForce, lForce;//, uForce;
    fForce = lForce = 0.0f;

	fForce += ff * runForce/*const*/;
	lForce += lf * runForce/*const*/;

	float strafing = fabsf(msg->straf); //msg->straf: left:-1, right:+1

	this->playerPhysicsEntity->SetForwardForce(fForce);
	this->playerPhysicsEntity->SetLeftwardForce(lForce);
	this->playerPhysicsEntity->SetStraft(strafing);
	this->playerPhysicsEntity->SetTurnTorque(rTurn);

	this->playerPhysicsEntity->SetJump((bool)msg->jump);
	/*
	if (msg->jump)
	{
		//uForce += jumpForce//const;
		//this->playerPhysicsEntity->SetTimeToJump(1.0f);
		this->playerPhysicsEntity->SetJump(true);
	}
	else
        this->playerPhysicsEntity->SetJump(false);

	//this->playerPhysicsEntity->SetUpFOrce(uForce);
	*/
	
}

//------------------------------------------------------------------------------
/**
*/
Physics::Entity*
PlayerPhysicsProperty::GetPhysicsEntity() const
{
    return this->playerPhysicsEntity;
}

//------------------------------------------------------------------------------
/**
    Render a debug visualization of the current 3d navigation path.
*/
void
PlayerPhysicsProperty::OnRenderDebug()
{
}

//------------------------------------------------------------------------------
/**
    enable and activate the physics
*/
void 
PlayerPhysicsProperty::EnablePhysics()
{
    n_assert(!this->IsEnabled());

    // create a char physics entity
	this->playerPhysicsEntity = Physics::PlayerEntity::Create();
    this->playerPhysicsEntity->SetUserData(GetEntity()->GetUniqueId());
    if (GetEntity()->HasAttr(Attr::Physics))
    {
        this->playerPhysicsEntity->SetCompositeName(GetEntity()->GetString(Attr::Physics));
    }
    this->playerPhysicsEntity->SetTransform(GetEntity()->GetMatrix44(Attr::Transform));

    // attach physics entity to physics level
    Physics::Level* physicsLevel = Physics::Server::Instance()->GetLevel();
    n_assert(physicsLevel);
    physicsLevel->AttachEntity(this->playerPhysicsEntity);

	AbstractPhysicsProperty::EnablePhysics();
}
    
//------------------------------------------------------------------------------
/** 
    disable and cleanup the physics
*/
void 
PlayerPhysicsProperty::DisablePhysics()
{
	n_assert(this->IsEnabled());

    // remove from level
    Physics::Level* physicsLevel = Physics::Server::Instance()->GetLevel();
    n_assert(physicsLevel);
    physicsLevel->RemoveEntity(this->playerPhysicsEntity);

    // cleanup resource
    this->playerPhysicsEntity = 0;

    // call parent
    AbstractPhysicsProperty::DisablePhysics();
}

//------------------------------------------------------------------------------
/**
    The OnMoveAfter() method transfers the current physics entity transform to
    the game entity.
*/
void
PlayerPhysicsProperty::OnMoveAfter()
{
	if (this->IsEnabled())
	{
		//if (this->playerPhysicsEntity->HasTransformChanged())
		{
            matrix44 physicsEntityTransform = this->playerPhysicsEntity->GetTransform();

            // update game entity
            Ptr<Message::UpdateTransform> msg = Message::UpdateTransform::Create();
            msg->SetMatrix(physicsEntityTransform);
            this->GetEntity()->SendSync(msg);
		}
	}
}
} // namespace Properties