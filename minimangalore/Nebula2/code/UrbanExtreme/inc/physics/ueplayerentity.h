#ifndef PHYSICS_UEPLAYERENTITY_H
#define PHYSICS_UEPLAYERENTITY_H
//------------------------------------------------------------------------------
/**
*/
#include "physics/charentity.h"
#include "physics/materialtable.h"
#include "physics/filterset.h"
#include "character/ncharacter2.h"

//------------------------------------------------------------------------------
namespace Physics
{
class RigidBody;
class BoxShape;
class Composite;
class Hinge2Joint;

class UePlayerEntity : public CharEntity
{
    DeclareRtti;
    DeclareFactory(UePlayerEntity);

public:
    /// constructor
    UePlayerEntity(); 
    /// destructor.
    virtual ~UePlayerEntity();


	/// called when attached to game entity
	virtual void OnActivate();
	
	/// called when removed from game entity
	virtual void OnDeactivate();

	// 	Called before a simulation step is taken. This will convert the linear desired velocity into an angular velocity.
	virtual void OnStepBefore();
	virtual void OnStepAfter();

	virtual bool OnCollide(Shape* collidee);
	bool	GetOnGround() const { return onGround; }

protected:
	
	/// create the default composite, used when character is alive
	virtual void CreateDefaultComposite();

protected: 
	float	defCurGravity;
	float	defAddGravity;
	float	defAccGravity;

	float   curGravity;
	float	addGravity;
	float	accGravity;	

	bool	onGround;	
};

RegisterFactory(UePlayerEntity);

}
#endif
