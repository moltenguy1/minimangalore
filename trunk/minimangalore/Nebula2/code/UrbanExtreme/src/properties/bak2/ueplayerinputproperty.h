#ifndef PROPERTIES_UEPLAYERINPUTPRPERTY_H
#define PROPERTIES_UEPLAYERINPUTPRPERTY_H

#include "properties/inputproperty.h"
#include "msg/ueplayerkey.h"



namespace Properties
{
class Message::UePlayerKey;

class UePlayerInputProperty : public InputProperty
{
public:
    DeclareRtti;
    DeclareFactory(UePlayerInputProperty);

public:
    ///
    UePlayerInputProperty();
    ///
    virtual ~UePlayerInputProperty();
    ///
    virtual void OnBeginFrame(); 

protected:
	///
    virtual void HandleInput();
	///
    void SendMove(const vector3& dir);
	void SendTurn(const float dir);
	void SendTurnTest(const float dir);
	void SendTurnTest2(const vector3& dir);
	void SendMoveDirection(const vector3& dir);
	void SendRotate(const float angle);
protected:
	
	nTime moveGotoTime;	
};

RegisterFactory(UePlayerInputProperty);

};
#endif
