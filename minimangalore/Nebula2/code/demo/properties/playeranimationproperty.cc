//------------------------------------------------------------------------------
//  properties/playeranimationproperty.cc
//  (C) 2007 Kim Hyoun Woo
//------------------------------------------------------------------------------
#include "properties/playeranimationproperty.h"
#include "game/entity.h"
#include "msg/gfxsetanimation.h"
#include "msg/playerstatus.h"

#include "script/animscript.h"

#include "kernel/ntimeserver.h"

namespace Properties
{

using namespace Message;

ImplementRtti(Properties::PlayerAnimationProperty, Game::Property);
ImplementFactory(Properties::PlayerAnimationProperty);

//------------------------------------------------------------------------------
/**
*/
bool
PlayerAnimationProperty::Accepts(Message::Msg* msg)
{
    n_assert(msg);
    if (msg->CheckId(Message::PlayerStatus::Id)) return true;
    return Game::Property::Accepts(msg);
}

//------------------------------------------------------------------------------
/**
*/
void
PlayerAnimationProperty::HandleMessage(Message::Msg* msg)
{
    //TODO: Put other player animation message handle code.
 
    // various player actions.
    if (msg->CheckId(Message::PlayerStatus::Id))
    {
        PlayerStatus* playerStatus = (PlayerStatus*) msg;

		//TEST:
		//playerStatus->fb = 0;

  //      if (playerStatus->fb > 0)
		//	RequestAnimation("FightSh_FWD", "", n_rand(0.0f, 1.0f));//forward.
		//else
		//if (playerStatus->fb < 0)
		//	RequestAnimation("FightSh_BWD", "", n_rand(0.0f, 1.0f));//backward.
		//else
  //          RequestAnimation("IDLE_stance", "", n_rand(0.0f, 1.0f));//backward.



		//PlayerStatus::Type type = playerStatus->GetAnimationType();
		//switch(type)
		//{
		//    case PlayerStatus::CrouchRollLeft:
		//		RequestAnimation("Crouch_Roll_L", ""/*none*/, n_rand(0.0f, 1.0f));
		//		break;
		//	case PlayerStatus::CrouchRollRight:
		//		RequestAnimation("Crouch_Roll_R", ""/*none*/, n_rand(0.0f, 1.0f));
		//		break;
		//	//...
		//	default:
		//		break;
		//}


    }

	//How can we send msg parameter to lua script side?
	
	this->luaScript->Update(this, msg);

	nString result;
	this->luaScript->Run("DoAnimation()", result);
}

//------------------------------------------------------------------------------
/**
*/
void 
PlayerAnimationProperty::OnActivate()
{
    Game::Property::OnActivate();

    // by default go to idle animation
    this->RequestAnimation("FIGHTIDLE", "", n_rand(0.0f, 1.0f));

	Script::LuaServer* luaServer = Script::LuaServer::Instance();

	// register animation script functions
	nLuaServer* scriptServer = (nLuaServer*)luaServer->GetScriptServer();
	//Script::RegisterAnimFuncntions(scriptServer->GetContext());

	// create script handling instance then load and run the loaded script file.
	this->luaScript = luaServer->CreateScript();
	
	Script::RegisterAnimFuncntions(this->luaScript->GetContext());

	luaScript->RunFile("data:scripts/anim.lua");
}

//------------------------------------------------------------------------------
/**
*/
void
PlayerAnimationProperty::RequestAnimation(const nString& baseAnimation, const nString& overlayAnimation, float baseAnimTimeOffset) const
{
    Ptr<Message::GfxSetAnimation> setAnimation = Message::GfxSetAnimation::Create();
    setAnimation->SetBaseAnimation(baseAnimation);
    setAnimation->SetOverlayAnimation(overlayAnimation);
    setAnimation->SetBaseAnimTimeOffset(baseAnimTimeOffset);
    setAnimation->SetFadeInTime(0.2);
    this->GetEntity()->SendSync(setAnimation);
}

}
