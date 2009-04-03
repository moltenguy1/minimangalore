#include "msg/ueplayerkey.h"

namespace Message
{
ImplementRtti(Message::UePlayerKey, Message::Msg);
ImplementFactory(Message::UePlayerKey);
ImplementMsgId(UePlayerKey);

void UePlayerKey::Clear()
{
	dobreak = false;
	accelerate = false;
	left = false;
	right = false;
}

} // namespace Message
