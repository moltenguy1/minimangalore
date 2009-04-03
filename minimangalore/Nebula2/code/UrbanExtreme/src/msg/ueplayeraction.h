#ifndef MSG_UEPLAYERACTION_H
#define MSG_UEPLAYERACTION_H

#include "message/msg.h"

namespace Message
{
class UePlayerAction : public Msg
{
    DeclareRtti;
    DeclareFactory(UePlayerAction);
    DeclareMsgId;

public:
			
	enum ActionType
	{
		S_Idle,
		S_Change_C,
		S_Run,
		S_Angle_L,
		S_Angle_R,
		S_Speed,
		
		C_Change_P,
		C_Change_S,
		C_Run,
		C_Angle_L,
		C_Angle_R,
		C_Speed,

		P_Change_C,
		P_Angle_L,
		P_Angle_R,
		P_Speed,		
	};
		
	enum ShapeType
	{
		Stand=0,
		Crounch,
		Prone,
	};	
     

public:
    UePlayerAction();

	void SetActionType(const UePlayerAction::ActionType& t);
	///
	const UePlayerAction::ActionType GetActionType() const;

protected:
    ActionType	actionType;	// ÀÚ¼¼
};

RegisterFactory(UePlayerAction);

//------------------------------------------------------------------------------
/**
*/
inline
UePlayerAction::UePlayerAction()
{
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
UePlayerAction::SetActionType(const UePlayerAction::ActionType& t)
{
	this->actionType = t;
}

//------------------------------------------------------------------------------
/**
*/
inline
const UePlayerAction::ActionType 
UePlayerAction::GetActionType() const
{
	return this->actionType;
}





} // namespace Messge

//------------------------------------------------------------------------------
#endif
