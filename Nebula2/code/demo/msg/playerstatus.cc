//------------------------------------------------------------------------------
//  msg/playerstatus.cc
//  (C) 2007 Kim Hyoun Woo
//------------------------------------------------------------------------------
#include "msg/playerstatus.h"

namespace Message
{
ImplementRtti(Message::PlayerStatus, Message::Msg);
ImplementFactory(Message::PlayerStatus);
ImplementMsgId(PlayerStatus);

//------------------------------------------------------------------------------
/**
*/
void
PlayerStatus::Clear()
{
    this->fb = 0.0f;
	this->lr = 0.0f;
	this->jump = 0.0f;
}

} // namespace Message
