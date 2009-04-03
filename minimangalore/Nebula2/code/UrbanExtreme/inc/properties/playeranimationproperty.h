#ifndef PROPERTIES_PLAYERANIMATIONPROPERTY_H
#define PROPERTIES_PLAYERANIMATIONPROPERTY_H
//------------------------------------------------------------------------------
/**
    @class PlayerAnimationProperty
	
*/
#include "game/property.h"

#include "script/luascript.h"
#include "script/luaserver.h"

namespace Properties
{

class PlayerAnimationProperty : public Game::Property
{
    DeclareRtti;
    DeclareFactory(PlayerAnimationProperty);

public:
	///
	virtual void OnActivate();
	///
    bool Accepts(Message::Msg* msg);
    /// handle messages that may result to animation switch
    virtual void HandleMessage(Message::Msg* msg);


protected:
    void RequestAnimation(const nString& baseAnimation, 
		const nString& overlayAnimation, float baseAnimTimeOffset) const;

private:
	Script::LuaScript* luaScript;

};

RegisterFactory(PlayerAnimationProperty);

} // namespace Properties
#endif
