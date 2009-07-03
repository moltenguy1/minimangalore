//------------------------------------------------------------------------------
//  physics/FixedJoint.cc
//  (C) 2003 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "physics/FixedJoint.h"
#include "gfx2/ngfxserver2.h"
#include "physics/rigidbody.h"

namespace Physics
{
ImplementRtti(Physics::FixedJoint, Physics::Joint);
ImplementFactory(Physics::FixedJoint);

//------------------------------------------------------------------------------
/**
*/
FixedJoint::FixedJoint() :
Joint(Joint::FixedJoint)
{
	
}

//------------------------------------------------------------------------------
/**
*/
FixedJoint::~FixedJoint()
{
	// empty
}

//------------------------------------------------------------------------------
/**
	NOTE: After Attach joint,must to call dJointSetFixed() to fix bodies
*/
void
	FixedJoint::Attach(dWorldID worldID, dJointGroupID groupID, const matrix44& m)
{
	// create ODE joint
	this->odeJointId = dJointCreateFixed(worldID, groupID);		
	

	// hand to parent class
	Joint::Attach(worldID, groupID, m);

	dJointSetFixed(this->odeJointId);

	// configure ODE joint
	this->UpdateTransform(m);
}

//------------------------------------------------------------------------------
/**
*/
void
	FixedJoint::UpdateTransform(const matrix44& m)
{
	tm = m;
}

//------------------------------------------------------------------------------
/**
*/
void
	FixedJoint::RenderDebug()
{
	if (this->IsAttached())
	{		
		//nGfxServer2::Instance()->DrawShape(nGfxServer2::Sphere,this->tm, this->GetDebugVisualizationColor());		
	}
}

} // namespace Physics
