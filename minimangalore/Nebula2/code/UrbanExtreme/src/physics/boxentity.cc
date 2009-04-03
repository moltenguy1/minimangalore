//----------------------------------------------------------------------------
//  (c) 2009    
//----------------------------------------------------------------------------
#include "physics/boxentity.h"
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
	ImplementRtti(Physics::BoxEntity, Physics::Entity);
	ImplementFactory(Physics::BoxEntity);

	//----------------------------------------------------------------------------
/**
*/
BoxEntity::BoxEntity()
:
radius(0.3f),
height(1.75f),
hover(0.2f),
nebCharacter(0)
{
}

//----------------------------------------------------------------------------
/**
*/
BoxEntity::~BoxEntity()
{
}


//----------------------------------------------------------------------------
// EOF
//----------------------------------------------------------------------------

}