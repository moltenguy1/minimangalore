#ifndef MSG_UEPLAYERKEY_H
#define MSG_UEPLAYERKEY_H

#include "message/msg.h"

namespace Message
{
class UePlayerKey : public Msg
{
    DeclareRtti;
    DeclareFactory(UePlayerKey);
    DeclareMsgId;

public:
	enum Status
	{	
		None        =0x00,
		Break		=0x01,
		Accelerate  =0x02,		
		Left		=0x04,
		Right		=0x08,	
	};     

public:
    UePlayerKey();

	void Clear();

public:
	bool	dobreak;
	bool	accelerate;
	bool	left;	
	bool	right;
};

RegisterFactory(UePlayerKey);

//------------------------------------------------------------------------------
/**
*/
inline
UePlayerKey::UePlayerKey():
dobreak(false),
accelerate(false),
left(false),
right(false)
{
/*
	dobreak=false;
	accelerate=false;
	left=false;	
	right=false;*/

}



} // namespace Messge

//------------------------------------------------------------------------------
#endif
