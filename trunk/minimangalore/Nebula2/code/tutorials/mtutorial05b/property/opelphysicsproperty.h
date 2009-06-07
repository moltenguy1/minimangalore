#ifndef PROPERTIES_OPELPHYSICSPROPERTY_H
#define PROPERTIES_OPELPHYSICSPROPERTY_H
//------------------------------------------------------------------------------
/**
    @class Properties::InputProperty

    A physics property adds basic physical behavior to a game entity.
    The default behavior is that of a passive physics object which will
    just passively roll and bounce around. Implement more advanced behavior
    in subclasses.

    The physics property maintains the attributes:

    Attr::Transform
    Attr::VelocityVector		// velocity of the entity

    // this property subclass the default physicsproperty, adding more control
    // over physics like steering, velocity, etc... 
    
    // this property add :
    (Attr::SteeringAngle);	// actual steering angle, in RAD ; READ ONLY
	(Attr::SteeringVelocity);	// actual velocity on the steering
	(Attr::SpeedVelocity);	// actual velocity on the wheel

    (C) je.a.le@wanadoo.Fr
*/
#include "game/property.h"
#include "attr/attributes.h"
#include "properties/physicsproperty.h"
#include "managers/focusmanager.h"
#include "game/entity.h"


//------------------------------------------------------------------------------
namespace Attr
{
	// ok, this need some expications. There no solution for requesting a specific
	// speed or angle, by doing that, you mess with the ode simulation : BAD
	// the only control you have is to apply forces on motor, than you look at the 
	// result. you MUST read the ode doc too.
	
	// BTW : if you plan to "play" with physic, I strongly suggest reading mecanic's books...
	// Attr will be used as bridge between ode and mangalore. There're a set for applying
	// forces, and an other for the "answer"
	
	// WRITE / READ
	DeclareFloat(RequestSteeringVelocity);	// request force on the direction
	DeclareFloat(RequestEngineVelocity);	// request force on the wheels
	
	DeclareFloat(SteeringAngle);	// actual steering angle, in RAD ; READ ONLY
	DeclareFloat(SteeringVelocity);	// actual velocity on the steering
	DeclareFloat(EngineVelocity);	// actual velocity on the wheel
	
};

//------------------------------------------------------------------------------
namespace Properties
{
class OpelPhysicsProperty : public PhysicsProperty
{
    DeclareRtti;
    DeclareFactory(OpelPhysicsProperty);

public:
    /// constructor
    OpelPhysicsProperty();
    /// destructor
    virtual ~OpelPhysicsProperty();
    /// setup default entity attributes
    virtual void SetupDefaultAttributes();
    
    /// called before movement happens : transfer physics forces from mangalore to 
    /// ode hinge
    virtual void OnMoveBefore();
    
	/// called after movement has happened : transfer from ode to mangalore
    virtual void OnMoveAfter();
    
    /// external control over the entity is done through message
	/// return true if message is accepted by a property
    virtual bool Accepts(Message::Msg* msg);
    /// handle a single message
    virtual void HandleMessage(Message::Msg* msg);

	// few constant ; make life so simple.....
	#define STEERING_FMAX 5.0f
	
	#define ENGINE_FMAX 20.f
	#define ENGINE_VELOCITY_MAX 30.0f	
	#define ENGINE_VELOCITY_INC 0.5f
	#define ENGINE_VELOCITY_DEC 0.3f
};

RegisterFactory(OpelPhysicsProperty);

}; // namespace Property
//------------------------------------------------------------------------------
#endif
