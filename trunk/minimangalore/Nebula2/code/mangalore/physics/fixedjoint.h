#ifndef PHYSICS_FIXEDJOINT_H
#define PHYSICS_FIXEDJOINT_H
//------------------------------------------------------------------------------
/**
@class Physics::FixedJoint

A hinge joint. See ODE docs for details.

(C) 2003 RadonLabs GmbH
*/
#include "physics/joint.h"
#include "physics/jointaxis.h"

//------------------------------------------------------------------------------
namespace Physics
{
class FixedJoint : public Joint
{
	DeclareRtti;
	DeclareFactory(FixedJoint);

public:
	/// constructor
	FixedJoint();
	/// destructor
	virtual ~FixedJoint();
	/// initialize the joint (attach to world)
	virtual void Attach(dWorldID worldID, dJointGroupID groupID, const matrix44& m);
	/// 
	virtual void UpdateTransform(const matrix44& m);
	/// render debug visualization
	virtual void RenderDebug();

private:
	matrix44 tm;
};

RegisterFactory(FixedJoint);


} // namespace Physics
//------------------------------------------------------------------------------
#endif
