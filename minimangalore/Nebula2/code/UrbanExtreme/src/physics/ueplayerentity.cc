#include "physics/ueplayerentity.h"

#include "physics/server.h"
#include "physics/rigidbody.h"
#include "physics/shape.h"
#include "physics/boxshape.h"
#include "physics/composite.h"
#include "physics/level.h"
#include "physics/physicsutil.h"
#include "physics/amotor.h"
#include "physics/ragdoll.h"
#include "physics/compositeloader.h"

namespace Physics
{
ImplementRtti(Physics::UePlayerEntity, Physics::CharEntity);
ImplementFactory(Physics::UePlayerEntity);

//------------------------------------------------------------------------------
/**
*/
UePlayerEntity::UePlayerEntity()
{
/*
radius(0.3f),
height(1.75f),
hover(0.2f),
*/
	radius=0.15f;
	height=0.8f;
	hover=0.2f;

	defCurGravity= -3.0f;
	defAddGravity= -0.3f;		
	defAccGravity= -0.01f;		

	curGravity = defCurGravity;
	addGravity = defAddGravity;
	accGravity = defAccGravity;

}

//------------------------------------------------------------------------------
/**
*/
UePlayerEntity::~UePlayerEntity()
{
//	n_assert(this->baseBody == 0);
}

void 
UePlayerEntity::CreateDefaultComposite()
{
	Server* physicsServer = Physics::Server::Instance();

	// create a composite with a spherical rigid body and an angular motor
	this->defaultComposite = physicsServer->CreateComposite();

	// create a base rigid body object
	this->baseBody = physicsServer->CreateRigidBody();
	this->baseBody->SetName("UePlayerEntityBody");


	float capsuleLength = this->height - 2.0f * this->radius - this->hover;
	matrix44 upRight;
	upRight.rotate_x(n_deg2rad(90.0f));
	upRight.translate(vector3(0.0f, capsuleLength * 0.5f - 0.22f, 0.0f));

	Ptr<Shape> shape = (Physics::Shape*) physicsServer->CreateCapsuleShape(upRight, MaterialTable::StringToMaterialType("Character"), this->radius, capsuleLength);
	this->baseBody->BeginShapes(1);
	this->baseBody->AddShape(shape);
	this->baseBody->EndShapes();
	this->defaultComposite->BeginBodies(1);
	this->defaultComposite->AddBody(this->baseBody);
	this->defaultComposite->EndBodies();

	// create an amotor connected to the base shape
	this->aMotor = physicsServer->CreateAMotor();
	this->aMotor->SetBodies(this->baseBody, 0);
	this->aMotor->SetNumAxes(2);
	this->aMotor->AxisParams(0).SetAxis(vector3(1.0f, 0.0f, 0.0f));
	this->aMotor->AxisParams(1).SetAxis(vector3(0.0f, 0.0f, 1.0f));
	this->aMotor->AxisParams(0).SetFMax(100000.0f);
	this->aMotor->AxisParams(1).SetFMax(100000.0f);
	this->defaultComposite->BeginJoints(1);
	this->defaultComposite->AddJoint(this->aMotor);
	this->defaultComposite->EndJoints();	

	
}

void
UePlayerEntity::OnActivate()
{
	Physics::CharEntity::OnActivate();
}

//------------------------------------------------------------------------------
/**
*/
void
UePlayerEntity::OnDeactivate()
{
	Physics::CharEntity::OnDeactivate();
}

//------------------------------------------------------------------------------
/**
Called before a simulation step is taken. This will convert the
linear desired velocity into an angular velocity.
*/
void
UePlayerEntity::OnStepBefore()
{
/*
	if (this->IsCollisionEnabled() && !this->IsRagdollActive())
	{
		// detect a activation from collide or if the entity should move
		if ((this->wasDisabledInTheLastFrame && this->IsEnabled())
			|| this->desiredVelocity.len() > 0.0f)
		{*/

			this->wasDisabledInTheLastFrame = false;

			if (!this->IsEnabled())
			{
				this->SetEnabled(true);
			}
			Physics::Server* physicsServer = Physics::Server::Instance();
			RigidBody* body = this->baseBody;
			dBodyID odeBodyId = body->GetOdeBodyId();

			// get "touching ground status" and determine ground material
			float distToGround;
			this->onGround = this->CheckGround(distToGround);

			addGravity += accGravity;
			curGravity += addGravity;

			// get current velocity and mass of body
			vector3 desVel(this->desiredVelocity.x, curGravity, this->desiredVelocity.z);
			
			float m = body->GetMass();
			vector3 curVel = body->GetLinearVelocity();

			// compute resulting impulse
			vector3 p = -(curVel - desVel) * m;

			// convert impulse to forceb    
			dVector3 odeForce;
			dWorldImpulseToForce(physicsServer->GetOdeWorldId(), dReal(this->level->GetStepSize()), p.x, p.y, p.z, odeForce);

			// set new force
			dBodyAddForce(odeBodyId, odeForce[0], odeForce[1], odeForce[2]);
	/*
		}
			else
			{
				this->wasDisabledInTheLastFrame = true;
				if (this->IsEnabled())
				{
					this->SetEnabled(false);
				}
			}
		}*/
	
	// NOTE: do NOT call the parent class, we don't need any damping
}

void UePlayerEntity::OnStepAfter()
{

	CharEntity::OnStepAfter();
}

bool 
UePlayerEntity::OnCollide(Shape* collidee)
{	
	if(collidee->GetType() == Physics::Shape::Mesh)
	{
		curGravity = defCurGravity;	
		addGravity = defAddGravity;			
	}		
	return CharEntity::OnCollide(collidee);
}


}  // namespace Physics

