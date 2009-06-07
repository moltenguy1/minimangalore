#ifndef MSG_POLORSYSTEMVALUES_H
#define MSG_POLORSYSTEMVALUES_H
//------------------------------------------------------------------------------
/**
    @class Message::PolarSystemValue

    A msg that can handle values for a polar system coordinate (phi, theta, distance, point of view)
	Nota : could have been split in 4 specific msg, but an entity is likely to change more
	than one value. This reduce the number of message, therefore lower the cpu usage

	message id is Message::PolarSystemValue::Id

    (C) 2006 je.a.le@wanadoo.fr
*/
#include "message/msg.h"

//------------------------------------------------------------------------------
namespace Message
{
class PolarSystemValues : public Msg
{
    DeclareRtti;
    DeclareFactory(PolarSystemValues);
    DeclareMsgId;

public:
    /// constructor
    PolarSystemValues();
    PolarSystemValues( float phi, float theta, float distance, vector3 lookat); // init with static vars
           
    // set / get the values : the message is not responsible
    // whether or not values are valid. it - or the property that send it -
    // may not be aware of some restrictions set by an other property
    // His job is just to pass data between classes
    void	SetPhi( float v );
    float	GetPhi() const;
    
    void	SetTheta( float v );
    float	GetTheta() const;
    
    void	SetDistance( float v );
    float	GetDistance() const;
  
	void	SetLookAt( vector3 v );
	vector3	GetLookAt() const;
    
	// this define which data is valid into this message.
	// could be all or just a single value
	// DYNAMIC :  if set, datas are dynamic range (inc/dec) instead of static (angle)
	enum {
		M_DYNAMIC	= 0x1,	
		M_THETA		= 0x2,	// 1 == theta value valid
		M_PHI		= 0x4,	// etc..
		M_LOOKAT	= 0x8,
		M_DISTANCE	= 0xA, 
		M_MASK		= 0xFFFE	// not a value ! mask for data bits (see input property...)
	};
	
	short	GetMask() const;
	void	SetUseDynamicsValues();
	bool	GetUseDynamicsValues() const;
	
	bool	DataIsSet() const; // return true if a list a "real" value is set (phi theta, distance)
	
private:
    float	phi;
    float	theta;
    float	distance;
    vector3	lookat;
    short	mask;		// set which data is valid into this message 
};

RegisterFactory(PolarSystemValues);

//------------------------------------------------------------------------------
inline PolarSystemValues::PolarSystemValues() :	
    phi(0.0f),
    theta(0.0f),
    distance(0.0f),
    mask(0)
{
    lookat.zero;
}

//------------------------------------------------------------------------------
inline PolarSystemValues::PolarSystemValues( float phi, float theta, float distance, vector3 lookat) :
phi(phi),
theta(theta),
distance(distance),
mask(false)
{
	this->lookat = lookat;
}

//------------------------------------------------------------------------------
inline short PolarSystemValues::GetMask() const
{
	return( mask );
}

//------------------------------------------------------------------------------
inline void	PolarSystemValues::SetPhi( float v )
{
	phi = v;
	mask |= M_PHI;
}

//------------------------------------------------------------------------------
inline float PolarSystemValues::GetPhi() const
{
	return( phi );
}

//------------------------------------------------------------------------------
inline void	PolarSystemValues::SetTheta( float v )
{
	theta = v;
	mask |= M_THETA;
}

//------------------------------------------------------------------------------
inline float PolarSystemValues::GetTheta() const
{
	return( theta );
}
    
//------------------------------------------------------------------------------
inline void	PolarSystemValues::SetDistance( float v )
{
	distance = v;
	mask |= M_DISTANCE;
}

//------------------------------------------------------------------------------
inline float PolarSystemValues::GetDistance() const
{
	return( distance );
}
  
//------------------------------------------------------------------------------
inline void	PolarSystemValues::SetLookAt( vector3 v )
{
	lookat = v;
	mask |= M_LOOKAT;
}

//------------------------------------------------------------------------------
inline vector3 PolarSystemValues::GetLookAt() const
{
	return( lookat );
}

//------------------------------------------------------------------------------
inline void	PolarSystemValues::SetUseDynamicsValues()
{
	mask |= M_DYNAMIC;
}

//------------------------------------------------------------------------------
inline bool PolarSystemValues::GetUseDynamicsValues() const
{
	return( mask && M_DYNAMIC );
}

//------------------------------------------------------------------------------
inline	bool PolarSystemValues::DataIsSet() const
{
	return( ( mask & M_MASK ) );
}



} // namespace Message
//------------------------------------------------------------------------------
#endif
