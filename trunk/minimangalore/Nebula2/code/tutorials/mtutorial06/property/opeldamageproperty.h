#ifndef PROPERTIES_OPELDAMAGEPROPERTY_H
#define PROPERTIES_OPELDAMAGEPROPERTY_H
//------------------------------------------------------------------------------
/**
    @class Properties::OpelDamageProperty

    A static camera that use the polar system
    
    (C) je.a.le@wanadoo.fr
*/
#include "game/property.h"
#include "game/entity.h"

#include "mtutorial06/message/damagevalues.h"

//------------------------------------------------------------------------------
namespace Attr
{
	DeclareBool(OpelAsDamage);
};

//------------------------------------------------------------------------------
namespace Properties
{
//using namespace Attr;

class OpelDamageProperty : public Game::Property
{
    DeclareRtti;
	DeclareFactory(OpelDamageProperty);

public:
    /// constructor
    OpelDamageProperty();
    /// destructor
    virtual ~OpelDamageProperty();
    /// setup default entity attributes
    virtual void SetupDefaultAttributes();
  
	/// return true if message is accepted by a property
	/// The message walid wil be Properties::DamageValues::Id
    virtual bool Accepts(Message::Msg* msg);
    /// handle a single message
    virtual void HandleMessage(Message::Msg* msg); 
 
private:
  	// must declare local (array, hash, etc...) for each mesh this pro handle damage.
  	// for the tutorial, let's be simple : just 1 var for the chassis
//  	DamageValues::Type	chassis_damage_level;
  	
	
};

RegisterFactory(OpelDamageProperty);

}; // namespace Properties
//------------------------------------------------------------------------------
#endif    
