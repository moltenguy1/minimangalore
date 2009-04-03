#ifndef MSG_PLAYERSTATUS_H
#define MSG_PLAYERSTATUS_H
//------------------------------------------------------------------------------
/**
    @class Message::PlayerStatus

    Message to handle various Player Status.

    (C) 2007 Kim Hyoun Woo
*/
#include "message/msg.h"

//------------------------------------------------------------------------------
namespace Message
{
class PlayerStatus : public Msg
{
    DeclareRtti;
    DeclareFactory(PlayerStatus);
    DeclareMsgId;

public:
    enum Type
    {
        CrouchRollLeft = 0,
        CrouchRollRight,
    };

public:
    PlayerStatus();

	Type GetAnimationType() const;

    void SetActionType(const PlayerStatus::Type& t);
    ///
    const PlayerStatus::Type GetActionType() const;

	///
	void Clear();

	float GetForward() const;
	float GetLeftward() const;


public:
     float fb;
     float lr;
	 float turn;
	 float straf;
     bool jump;

protected:
    Type type;



};

RegisterFactory(PlayerStatus);

//------------------------------------------------------------------------------
/**
*/
inline
PlayerStatus::PlayerStatus()
{
}

//------------------------------------------------------------------------------
/**
*/
inline
PlayerStatus::Type 
PlayerStatus::GetAnimationType() const
{
	return this->type;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
PlayerStatus::SetActionType(const PlayerStatus::Type& t)
{
    this->type = t;
}

//------------------------------------------------------------------------------
/**
*/
inline
const PlayerStatus::Type 
PlayerStatus::GetActionType() const
{
    return this->type;
}

inline
float PlayerStatus::GetForward() const
{
	return this->fb;
}

inline
float PlayerStatus::GetLeftward() const
{
	return this->lr;
}

} // namespace Messge

//------------------------------------------------------------------------------
#endif
