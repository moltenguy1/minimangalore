#ifndef N_OXENTITY_H
#define N_OXENTITY_H
//----------------------------------------------------------------------------
/**
    @class BoxEntity
    @ingroup 
    @brief a brief description of the class

    (c) 2009    
*/
//----------------------------------------------------------------------------
#include "physics/entity.h"
#include "physics/materialtable.h"
#include "physics/filterset.h"
#include "character/ncharacter2.h"

//----------------------------------------------------------------------------

namespace Physics
{
	class RigidBody;
	class BoxShape;
	class Composite;

class BoxEntity : public Entity
{
	DeclareRtti;
	DeclareFactory(BoxEntity);
    public:
        /// constructor
        BoxEntity();
        /// destructor
        virtual ~BoxEntity();

protected:
	Ptr<RigidBody> baseBody;
	Ptr<Composite> defaultComposite;
	float radius;
	float height;
	float hover;
	nCharacter2* nebCharacter;
	FilterSet groundExcludeSet;
	MaterialType groundMaterial;

	vector3 lookatDirection;    // looks into last valid desiredVelocity direction

	bool touched;
	bool floorContact;

	float forwardForce;
	float leftwardForce;
	float strafing;
	float turnTorque;
	bool jump;
	float timeToJump;
	float upForce;
};

RegisterFactory(BoxEntity);

}
//----------------------------------------------------------------------------
#endif // N_OXENTITY_H

