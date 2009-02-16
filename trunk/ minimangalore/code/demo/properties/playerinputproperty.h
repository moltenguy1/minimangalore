#ifndef PROPERTIES_PLAYERINPUTPRPERTY_H
#define PROPERTIES_PLAYERINPUTPRPERTY_H
//-----------------------------------------------------------------------------
/**
    @class Properties::PlayerInputProperty

    (C)2007  Kim Hyoun Woo
*/
#include "properties/inputproperty.h"
#include "msg/playerstatus.h"

namespace Properties
{
//class Message::PlayerStatus;

class PlayerInputProperty : public InputProperty
{
public:
    DeclareRtti;
    DeclareFactory(PlayerInputProperty);

public:
    ///
    PlayerInputProperty();
    ///
    virtual ~PlayerInputProperty();
    ///
    virtual void OnBeginFrame(); 

protected:
	///
    virtual void HandleInput();
	///
    void SendMove(const vector3& dir);

protected:
	Message::PlayerStatus playerMsg;

	nTime moveGotoTime;
};

RegisterFactory(PlayerInputProperty);

};
#endif
