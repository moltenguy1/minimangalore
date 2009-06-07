#ifndef MSG_OPELCONTROLVALUES_H
#define MSG_OPELCONTROLVALUES_H
//------------------------------------------------------------------------------
/**
    @class Message::OpelControlValue

	This message handler value an input control might send to control the 
	physics part of the opel, like steering, accelerate, brake, etc...

	This is prety much the same pattern i use for PolarSystemValue

	message id is Message::OpelControlValue::Id

    (C) 2006 je.a.le@wanadoo.fr
*/
#include "message/msg.h"

//------------------------------------------------------------------------------
namespace Message
{
class OpelControlValues : public Msg
{
    DeclareRtti;
    DeclareFactory(OpelControlValues);
    DeclareMsgId;

public:
    /// constructor
    OpelControlValues();
           
    void	ApplySteeringVelocity( float v );	// add v to the steering velocity
    float	GetSteeringVelocity() const;		// get the requested velocity
    
    void	ApplySpeedVelocity( float v );	// add v to the wheel velocity : accelerate/brake
    float	GetSpeedVelocity() const;
    
	// this define which data is valid into this message.
	// could be all or just a single value
	// DYNAMIC :  if set, datas are dynamic range (inc/dec) instead of static (angle)
	enum {
		M_DYNAMIC	= 0x1,	
		M_STEERING	= 0x2,	// 1 == steering value
		M_SPEED		= 0x4,	// etc..		
		M_MASK		= 0xFFFE
	};
	
	short	GetMask() const;
	void	SetUseDynamicsValues();
	bool	GetUseDynamicsValues() const;
	
	bool	DataIsSet() const; // return true if a list a "real" value is set

private:
    float	steering;
    float	speed;
    short	mask;		// set which data is valid into this message 
};

RegisterFactory(OpelControlValues);

//------------------------------------------------------------------------------
inline OpelControlValues::OpelControlValues() :	
    steering(0.0f),
    speed(0.0f),
    mask(0)
{
}

//------------------------------------------------------------------------------
inline short OpelControlValues::GetMask() const
{
	return( mask );
}

//------------------------------------------------------------------------------
inline void	OpelControlValues::ApplySteeringVelocity( float v )
{
	steering = v;
	mask |= M_STEERING;
}

//------------------------------------------------------------------------------
inline float OpelControlValues::GetSteeringVelocity() const
{
	return( steering );
}

//------------------------------------------------------------------------------
inline void	OpelControlValues::ApplySpeedVelocity( float v )
{
	speed = v;
	mask |= M_SPEED;
}

//------------------------------------------------------------------------------
inline float OpelControlValues::GetSpeedVelocity() const
{
	return( speed );
}
     
//------------------------------------------------------------------------------
inline void	OpelControlValues::SetUseDynamicsValues()
{
	mask |= M_DYNAMIC;
}

//------------------------------------------------------------------------------
inline bool OpelControlValues::GetUseDynamicsValues() const
{
	return( mask && M_DYNAMIC );
}

//------------------------------------------------------------------------------
inline bool OpelControlValues::DataIsSet() const
{
	return( (mask & M_MASK) );
}


} // namespace Message
//------------------------------------------------------------------------------
#endif
