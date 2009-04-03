//------------------------------------------------------------------------------
//  properties/playeranimationproperty.cc
//  (C) 2007 Kim Hyoun Woo
//------------------------------------------------------------------------------
#include "properties/ueplayeranimationproperty.h"
#include "game/entity.h"
#include "msg/gfxsetanimation.h"


#include "msg/ueplayeraction.h"
#include "msg/movedirection.h"
#include "msg/movesetvelocity.h"
#include "msg/movestop.h"

#include "kernel/ntimeserver.h"

namespace Properties
{

using namespace Message;

ImplementRtti(Properties::UePlayerAnimationProperty, Properties::ActorAnimationProperty);
ImplementFactory(Properties::UePlayerAnimationProperty);

void 
UePlayerAnimationProperty::OnActivate()
{
	Game::Property::OnActivate();

	// by default go to idle animation
	this->RequestAnimation("S_Idle", "", n_rand(0.0f, 1.0f));
}

void
UePlayerAnimationProperty::HandleMessage(Message::Msg* msg)
{
	n_assert(msg);
	
	if (msg->CheckId(Message::UePlayerAction::Id))
	{
		UePlayerAction* actionMsg=(UePlayerAction*)msg;
		if( actionMsg->GetActionType() == UePlayerAction::S_Idle )				this->RequestAnimation("S_Idle", "", n_rand(0.0f, 1.0f));
		else if( actionMsg->GetActionType() == UePlayerAction::S_Speed )		this->RequestAnimation("S_Speed", "", n_rand(0.0f, 1.0f));
		else if( actionMsg->GetActionType() == UePlayerAction::S_Run )			this->RequestAnimation("S_Run", "", n_rand(0.0f, 1.0f));
		else if( actionMsg->GetActionType() == UePlayerAction::S_Angle_L )		this->RequestAnimation("S_Angle_L", "", n_rand(0.0f, 1.0f));
		else if( actionMsg->GetActionType() == UePlayerAction::S_Angle_R )		this->RequestAnimation("S_Angle_R", "", n_rand(0.0f, 1.0f));
		else if( actionMsg->GetActionType() == UePlayerAction::S_Change_C )		this->RequestAnimation("S_Change_C", "", n_rand(0.0f, 1.0f));

	
		else if( actionMsg->GetActionType() == UePlayerAction::C_Speed )		this->RequestAnimation("C_Speed", "", n_rand(0.0f, 1.0f));
		else if( actionMsg->GetActionType() == UePlayerAction::C_Run )			this->RequestAnimation("C_Run", "", n_rand(0.0f, 1.0f));
		else if( actionMsg->GetActionType() == UePlayerAction::C_Angle_L )		this->RequestAnimation("C_Angle_L", "", n_rand(0.0f, 1.0f));
		else if( actionMsg->GetActionType() == UePlayerAction::C_Angle_R )		this->RequestAnimation("C_Angle_R", "", n_rand(0.0f, 1.0f));
		else if( actionMsg->GetActionType() == UePlayerAction::C_Change_S )		this->RequestAnimation("C_Change_C", "", n_rand(0.0f, 1.0f));
		else if( actionMsg->GetActionType() == UePlayerAction::C_Change_P )		this->RequestAnimation("C_Change_P", "", n_rand(0.0f, 1.0f));		
	
		else if( actionMsg->GetActionType() == UePlayerAction::P_Speed )		this->RequestAnimation("P_Speed", "", n_rand(0.0f, 1.0f));	
		else if( actionMsg->GetActionType() == UePlayerAction::P_Angle_L )		this->RequestAnimation("P_Angle_L", "", n_rand(0.0f, 1.0f));
		else if( actionMsg->GetActionType() == UePlayerAction::P_Angle_R )		this->RequestAnimation("P_Angle_R", "", n_rand(0.0f, 1.0f));
		else if( actionMsg->GetActionType() == UePlayerAction::P_Change_C )		this->RequestAnimation("P_Change_C", "", n_rand(0.0f, 1.0f));
	}	
}

bool UePlayerAnimationProperty::Accepts( Message::Msg* msg )
{
	if(msg->CheckId(UePlayerAction::Id))	
		return true;

	return false;
}

}
