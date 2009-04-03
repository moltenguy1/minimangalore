#ifndef PROPERTIES_PLAYERANIMATIONPROPERTY_H
#define PROPERTIES_PLAYERANIMATIONPROPERTY_H
//------------------------------------------------------------------------------
/**
    @class PlayerAnimationProperty
	
*/
#include "game/property.h"

#include "properties/actoranimationproperty.h"

namespace Properties
{

class UePlayerAnimationProperty : public ActorAnimationProperty
{
    DeclareRtti;
    DeclareFactory(UePlayerAnimationProperty);

public:
	//
	virtual void UePlayerAnimationProperty::OnActivate();

	virtual bool Accepts(Message::Msg* msg);
	virtual void HandleMessage(Message::Msg* msg);


private:
	

};

RegisterFactory(UePlayerAnimationProperty);

} // namespace Properties
#endif
