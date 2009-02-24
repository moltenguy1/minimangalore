//------------------------------------------------------------------------------
//  physics/playerentity.cc
//  (C) 2007 Kim Hyoun Woo
//------------------------------------------------------------------------------
#include "physics/playerentity.h"
#include "physics/server.h"
#include "physics/rigidbody.h"
#include "physics/shape.h"
#include "physics/boxshape.h"
#include "physics/composite.h"
#include "physics/level.h"
#include "physics/physicsutil.h"
#include "physics/compositeloader.h"

namespace Physics
{
ImplementRtti(Physics::PlayerEntity, Physics::Entity);
ImplementFactory(Physics::PlayerEntity);

//------------------------------------------------------------------------------
/**
*/
PlayerEntity::PlayerEntity() :
    radius(0.3f),
	height(1.75f),
	hover(0.2f),
	nebCharacter(0)
{
}

//------------------------------------------------------------------------------
/**
*/
PlayerEntity::~PlayerEntity()
{
	n_assert(this->baseBody == 0);
}

//------------------------------------------------------------------------------
/**
*/
void
PlayerEntity::OnActivate()
{
    n_assert(this->baseBody == 0);
    Server* physicsServer = Physics::Server::Instance();
    this->active = true;

    // create a composite with a spherical rigid body and an angular motor
    this->defaultComposite = physicsServer->CreateComposite();

	// create capsule for a shape which to be used for a player.
    float capsuleLength = this->height - 2.0f * this->radius - this->hover;
    matrix44 upRight;
    upRight.rotate_x(n_deg2rad(90.0f));
    upRight.translate(vector3(0.0f, capsuleLength * 0.5f, 0.0f));
    Ptr<Shape> shape = (Physics::Shape*) physicsServer->CreateCapsuleShape(upRight, 
		MaterialTable::StringToMaterialType("Character"), this->radius, capsuleLength);

    // create a base rigid body object
    this->baseBody = physicsServer->CreateRigidBody();
    this->baseBody->SetName("PlayerEntityBody");

    this->baseBody->BeginShapes(1);
    this->baseBody->AddShape(shape);
    this->baseBody->EndShapes();

    this->defaultComposite->BeginBodies(1);
    this->defaultComposite->AddBody(this->baseBody);
    this->defaultComposite->EndBodies();

    this->SetComposite(this->defaultComposite);
    this->defaultComposite->Attach(physicsServer->GetOdeWorldId(), 
		physicsServer->GetOdeDynamicSpaceId(), physicsServer->GetOdeStaticSpaceId());
    this->active = true;

    // NOTE: do NOT call parent class
}

//------------------------------------------------------------------------------
/**
*/
void
PlayerEntity::OnDeactivate()
{
    n_assert(this->baseBody != 0);
    Level* level = Physics::Server::Instance()->GetLevel();
    n_assert(level);

    Entity::OnDeactivate();

    this->baseBody = 0;
    this->defaultComposite = 0;
    this->SetCharacter(0);
}


//------------------------------------------------------------------------------
/**
    This function checks if the character is currently touching the ground
    by doing a vertical ray check.
*/
bool
PlayerEntity::CheckGround(float& dist, float& depth)
{
    Physics::Server* physicsServer = Physics::Server::Instance();

    vector3 pos = this->GetTransform().pos_component();
    vector3 from(pos.x, pos.y + this->radius * 2.0f, pos.z);
    vector3 dir(0.0f, -this->radius * 4.0f, 0.0f);

    FilterSet excludeSet = this->groundExcludeSet;
    excludeSet.AddEntityId(this->GetUniqueId());
    const ContactPoint* contact = Physics::Server::Instance()->GetClosestContactAlongRay(from, dir, excludeSet);
    if (contact)
    {
        dist = vector3(contact->GetPosition() - from).len() - this->radius * 2.0f;
        this->groundMaterial = contact->GetMaterial();

		depth = contact->GetDepth();

        return true;
    }
    else
    {
        dist = 2.0f * this->radius;
        this->groundMaterial = InvalidMaterial;
        return false;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
PlayerEntity::OnStepBefore()
{
    Physics::Server* physicsServer = Physics::Server::Instance();
    RigidBody* body = this->baseBody;
    dBodyID odeBodyId = body->GetOdeBodyId();

    float rTurn  = 0;
    //float fForce = 0;
    //float lForce = 0;
    float uForce = 0;
    bool damp = true;

    vector3 up(0, 1, 0);
	vector3 lvel = body->GetLinearVelocity();//obj_->lVel();
	vector3 avel = body->GetAngularVelocity();//obj_->aVel();

	const matrix44 &bodyTM = body->GetTransform();

	// bosy position.
	vector3 pos = bodyTM.pos_component();//obj_->pos();

    // bosy rotation.
    vector3 actual_right, actual_up, actual_front;
	actual_right = bodyTM.x_component();
	actual_up = bodyTM.y_component();
	actual_front = bodyTM.z_component();
    //obj_->ori( &actual_right, &actual_up, &actual_front );

    vector3 front( actual_front.x, 0, actual_front.z );
	front.norm();//D3DXVec3Normalize( &front, &front );


    float m = body->GetMass();

	vector3 curVel = body->GetLinearVelocity();
	float actualVel = curVel.len();
	vector3 nVel = curVel;
	if (actualVel > 1e-1)
	{
		nVel.norm();
	}
	else
	{
		actualVel = 0;
		nVel = vector3(0.0f, 0.0f, 0.0f);
	}

    float const mass = m;
    float const kneeForce = 30 * mass;
    float const lBrakeForce = 10 * mass;
    float const aBrakeForce = 3 * mass;
    float const sideBrakeForce = 9 * mass;
    float runForce = 7 * mass;
    float const jumpForce = 500 * mass;   //  it's a very short impulse
    float const turnTorque = 7 * mass;
    float const maxVel = 4;
    float const legLength = 0.9f;
    float const kneeExtend = 0.31f;  //  how much to "push" to extend the knees
    float const rightTorque = 15 * mass;

    if( actualVel > maxVel ) {
      runForce = (actualVel > maxVel+1) ? 0 : (actualVel - maxVel) * runForce;
    }


	if (touched)
	{
	    //touched on the upper part

        this->floorContact = true;
        touched = false;
	}
	else
	{
	}

    // check ground
	float distGround, depth;
    if (this->CheckGround(distGround, depth))
	{
		float heightSoon = depth + 0.5f * lvel.y;
		float kneeScale = legLength - heightSoon + kneeExtend;
		if( kneeScale < 0 ) 
		{
			kneeScale = 0;
		}
		else 
		if( kneeScale > 1 ) 
		{
			kneeScale = 1;
		}
        uForce += kneeForce * kneeScale;
		this->floorContact = true;
	}

	if (this->jump)
	{
		this->upForce += jumpForce;//const;
		this->timeToJump = 1.0f;
	}

	if (floorContact)
	{
		if(damp)
		{
			if (actualVel > 0)
			{
                float forceToStop = m * actualVel / 0.01f;   //  step size
                if( forceToStop > lBrakeForce/*const*/ ) 
					forceToStop = lBrakeForce;

				//  don't dampen Y as much as the others
				dBodyAddForce(odeBodyId, -nVel.x * forceToStop, -nVel.y * forceToStop * 0.25f, -nVel.z * forceToStop);
			}
			else
			{
				dBodySetLinearVel(odeBodyId, 0, 0, 0);
			}
		}
		else
		{
			dBodyAddRelForce(odeBodyId, -actual_right.dot(lvel) * sideBrakeForce * (1 - strafing), 0, 0 );
		}

      dBodyAddTorque(odeBodyId, -avel.x, -avel.y, -avel.z );
      dBodyAddRelForce(odeBodyId, -this->leftwardForce, uForce, this->forwardForce );
      dBodyAddRelTorque(odeBodyId, 0, rTurn, 0 );
	}

    //  calculate rightening forces, even if not floor contacted
    vector3 rt;
    //  point is relative, vel is global
    dBodyGetRelPointVel(odeBodyId, 1, 0, 0, &rt.x );
    rt *= 0.02f; //  how far into the future to look
    rt += actual_right;
	float turnLeft = -up.dot(rt);//-D3DXVec3Dot( &up, &rt );
    vector3 fw;
    dBodyGetRelPointVel(odeBodyId, 0, 0, 1, &fw.x );
    fw *= 0.02f;
    fw += actual_front;
    float turnForward = up.dot(fw);//D3DXVec3Dot( &up, &fw );
    dBodyAddRelTorque(odeBodyId, turnForward * rightTorque, 0, turnLeft * rightTorque );

    this->floorContact = false;
}

//------------------------------------------------------------------------------
/**
*/
bool 
PlayerEntity::OnCollide(Shape* collidee)
{
    bool validCollision = Entity::OnCollide(collidee);
    validCollision &= !this->groundExcludeSet.CheckShape(collidee);
    return validCollision;
}

//------------------------------------------------------------------------------
/**
*/
void 
PlayerEntity::SetTransform(const matrix44& m)
{
    matrix44 offsetMatrix(m);
    offsetMatrix.M42 += this->radius + this->hover;
    Entity::SetTransform(offsetMatrix);
    this->lookatDirection = -m.z_component();
}

//------------------------------------------------------------------------------
/**
*/
matrix44 
PlayerEntity::GetTransform() const
{
    matrix44 tmp = Entity::GetTransform();
    static vector3 upVector(0.0f, 1.0f, 0.0f);
    matrix44 fixedTransform;
    fixedTransform.lookatRh(this->lookatDirection, upVector);
    fixedTransform.translate(tmp.pos_component());
    fixedTransform.M42 -= this->radius + this->hover;
    return fixedTransform;
}

//------------------------------------------------------------------------------
/**
*/
bool 
PlayerEntity::HasTransformChanged() const
{
    // compare current look direction and desired lookat direction
    vector3 curLookAt = -Entity::GetTransform().z_component();
    if (!this->lookatDirection.isequal(curLookAt, 0.01f))
    {
        return true;
    }
    else
    {
        return Entity::HasTransformChanged();
    }
}

}  // namespace Physics

