#ifndef MSG_CROUCHROLLLEFT_H
#define MSG_CROUCHROLLLEFT_H

#include "message/msg.h"

namespace Message
{
class CrouchRoll : public Msg
{
    DeclareRtti;
    DeclareFactory(CrouchRoll);
    DeclareMsgId;

public:
    enum Dir
    {
        Left = 0,
        Right
    };

public:
    /// constructor
    CrouchRoll(); 
    /// set the direction left or right
    void SetDirection(const CrouchRoll::Dir& dir);
    /// get the direction 
    const CrouchRoll::Dir GetDirection() const; 

protected:
    Dir direction;

};

RegisterFactory(CrouchRoll);

//------------------------------------------------------------------------------
/**
*/
inline
void
CrouchRoll::SetDirection(const CrouchRoll::Dir& dir)
{
    this->direction = dir;
}

//------------------------------------------------------------------------------
/**
*/
inline
const CrouchRoll::Dir
CrouchRoll::GetDirection() const
{
    return this->direction;
}
}
#endif