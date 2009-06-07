#ifndef MSG_DAMAGEVALUES_H
#define MSG_DAMAGEVALUES_H
//------------------------------------------------------------------------------
/**
    @class Message::DamageValues

	this message enform an entity about damage on a specific (child) entity/mesh
	
   	message id is Message::DamageValues::Id

    (C) 2006 je.a.le@wanadoo.fr
*/
#include "message/msg.h"

//------------------------------------------------------------------------------
namespace Message
{
class DamageValues : public Msg
{
    DeclareRtti;
    DeclareFactory(DamageValues);
    DeclareMsgId;

public:
	// type of damage (level) must be applied
	typedef enum {
		D_NONE,		// no damage
		D_LEVEL_1,	// type : slit damage
		D_LEVEL_2,	// more severe damage
		D_LEVEL_3,	// even more severe damage, typ. apply some mesh alteration too
					// not used on this tut
		//etc...
		
		// those 2 are just for tutorial purpose :-)
		D_LEVEL_YELLOW,
		D_LEVEL_RED,
		
		D_LEVEL_MAX
	} Type;

    /// constructor
    DamageValues();
    DamageValues( DamageValues::Type type, nString name ); // init with static vars
           
    // set / get the values : the message is not responsible
    // whether or not values are valid. it - or the property that send it -
    // may not be aware of some restrictions set by an other property
    // His job is just to pass data between classes
    
    void	SetType( DamageValues::Type type );
    DamageValues::Type GetType() const;
    
    void	SetEntityName( nString name );
    nString	GetEntityName() const;
         
	// this define which data is valid into this message.
	// could be all or just a single value
	// DYNAMIC :  if set, datas are dynamic range (inc/dec) instead of static (angle)
	enum {
		M_TYPE		= 0x1,	
		M_NAME		= 0x2,
		M_MASK		= 0x3	// mask for data bits (see input property...)
	};
	
	short	GetMask() const;
	
	bool	DataIsSet() const; // all values must be set for this message, true = yes
	
private:
    Type	type;
    nString	name;
    short	mask;		// set which data is valid into this message 
};

RegisterFactory(DamageValues);

//------------------------------------------------------------------------------
inline DamageValues::DamageValues() :	
    type(D_NONE),
    name(0),
    mask(0)
{
}

//------------------------------------------------------------------------------
inline DamageValues::DamageValues( DamageValues::Type type, nString name ) : // init with static vars
mask(false)
{
	n_assert( name != 0 );
	n_assert( type < D_LEVEL_MAX );
	this->type = type;
	this->name = name;
}

//------------------------------------------------------------------------------
inline short DamageValues::GetMask() const
{
	return( mask );
}

//------------------------------------------------------------------------------
inline void	DamageValues::SetType( DamageValues::Type type )
{
	n_assert( type < D_LEVEL_MAX );
	this->type = type;
	mask |= M_TYPE;
}

//------------------------------------------------------------------------------
inline DamageValues::Type DamageValues::GetType() const
{
	return( this->type );
}

//------------------------------------------------------------------------------
inline void	DamageValues::SetEntityName( nString name )
{
	n_assert( name != 0 );
	this->name = name;
	mask |= M_NAME;
}

//------------------------------------------------------------------------------
inline nString DamageValues::GetEntityName() const
{
	return( this->name );
}

inline	bool DamageValues::DataIsSet() const
{
	return( ( ( mask & M_MASK ) == M_MASK ) );
}

} // namespace Message
//------------------------------------------------------------------------------
#endif
