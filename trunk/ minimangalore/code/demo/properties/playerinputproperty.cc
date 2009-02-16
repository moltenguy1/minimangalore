//-----------------------------------------------------------------------------
//  properties/playerinputproperty.cc
//
//  (c)2007  Kim Hyoun Woo
//-----------------------------------------------------------------------------
#include "properties/playerinputproperty.h"
#include "managers/focusmanager.h"
#include "input/server.h"
#include "foundation/factory.h"
#include "game/entity.h"
#include "managers/envquerymanager.h"
#include "managers/timemanager.h"
//#include "msg/movedirection.h"
//#include "msg/movestop.h"
#include "msg/cameraorbit.h"
#include "msg/movedirection.h"
#include "msg/movegoto.h"
#include "msg/movestop.h"
#include "msg/playerstatus.h"

#include "game/time/inputtimesource.h"

#include "msg/gfxsetanimation.h"

namespace Properties
{
ImplementRtti(Properties::PlayerInputProperty, Properties::InputProperty);
ImplementFactory(Properties::PlayerInputProperty);

using namespace Game;
using namespace Managers;

//-----------------------------------------------------------------------------
/**
*/
PlayerInputProperty::PlayerInputProperty()
{
}

//-----------------------------------------------------------------------------
/**
*/
PlayerInputProperty::~PlayerInputProperty()
{
}

//-----------------------------------------------------------------------------
/**
*/
void
PlayerInputProperty::OnBeginFrame()
{
    // only do something if we have the input focus
    if (FocusManager::Instance()->GetInputFocusEntity() == GetEntity())
    {        
        this->HandleInput();
    }	
    InputProperty::OnBeginFrame();
}

void
PlayerInputProperty::SendMove(const vector3& dir)
{
        // send a MoveGoto to the mouse intersection point
        //const vector3& mousePos = envQueryMgr->GetMousePos3d();

        const vector3& from = GetEntity()->GetMatrix44(Attr::Transform).pos_component();

		Ptr<Message::MoveGoto> msg = Message::MoveGoto::Create();
        
		vector3 to = from;
		to += dir;
		msg->SetPosition(to);
		
		GetEntity()->SendAsync(msg);

        // record the current time for the HandleLMBPressed() method
        this->moveGotoTime = InputTimeSource::Instance()->GetTime();
}

//-----------------------------------------------------------------------------
/**
*/
void
PlayerInputProperty::HandleInput()
{
    Input::Server* inputServer = Input::Server::Instance();

	/*
    const float vel = 1.0f;
    static const vector3 left(-vel, 0.0f, 0.0f);
    static const vector3 right(vel, 0.0f, 0.0f);
    static const vector3 up(0.0f, 0.0f, vel);
    static const vector3 down(0.0f, 0.0f, -vel);
    bool moved = false;

    if (inputServer->GetButtonDown("moveLeft"))
    {
        Ptr<Message::MoveDirection> msg = Message::MoveDirection::Create();
        msg->SetCameraRelative(true);
        msg->SetDirection(left);
        GetEntity()->SendAsync(msg);
        moved = true;
    }
    if (inputServer->GetButtonDown("moveRight"))
    {
        Ptr<Message::MoveDirection> msg = Message::MoveDirection::Create();
        msg->SetCameraRelative(true);
        msg->SetDirection(right);
        GetEntity()->SendAsync(msg);
        moved = true;
    }
    if (inputServer->GetButtonDown("moveBackward"))
    {
        Ptr<Message::MoveDirection> msg = Message::MoveDirection::Create();
        msg->SetCameraRelative(true);
        msg->SetDirection(up);
        GetEntity()->SendAsync(msg);
        moved = true;
    }
    if (inputServer->GetButtonDown("moveForward"))
    {
        Ptr<Message::MoveDirection> msg = Message::MoveDirection::Create();
        msg->SetCameraRelative(true);
        msg->SetDirection(down);
        GetEntity()->SendAsync(msg);
        moved = true;
    }

    if (!moved)
    {
        Ptr<Message::MoveStop> msg = Message::MoveStop::Create();
        GetEntity()->SendAsync(msg);
    }
	*/

	this->playerMsg.Clear();

if (inputServer->GetButtonPressed("mouseMMB"))
{
    // get horizontal and vertical mouse movement
    //Input::Server* inputServer = Input::Server::Instance();
    float x = inputServer->GetSlider("mouseLeft") - inputServer->GetSlider("mouseRight");
    float y = inputServer->GetSlider("mouseUp") - inputServer->GetSlider("mouseDown");

    // create CameraOrbit message
	Ptr<Message::CameraOrbit> camMsg = Message::CameraOrbit::Create();
    camMsg->SetHorizontalRotation(x);
    camMsg->SetVerticalRotation(y);
    GetEntity()->SendAsync(camMsg);
}


	const float velocity = 1.5f;

	if (inputServer->GetButtonDown("moveForward"))
	{
        this->SendMove(vector3(0.0f, 0.0f, -velocity));
	}
	else
	if(inputServer->GetButtonPressed("moveForward"))
	{
		nTime curTime = InputTimeSource::Instance()->GetTime();
		if ((curTime - this->moveGotoTime) > 0.25f)
		{
            this->SendMove(vector3(0.0f, 0.0f, -velocity));
		}
	}

	if (inputServer->GetButtonDown("moveLeft"))
	{
		this->SendMove(vector3(-velocity, 0.0f, 0.0f));
	}
	else
	if(inputServer->GetButtonPressed("moveLeft"))
	{
		nTime curTime = InputTimeSource::Instance()->GetTime();
		if ((curTime - this->moveGotoTime) > 0.25f)
		{
			this->SendMove(vector3(-velocity, 0.0f, 0.0f));
		}
	}

	if (inputServer->GetButtonDown("moveRight"))
	{
		this->SendMove(vector3(velocity, 0.0f, 0.0f));

	}
	else
	if(inputServer->GetButtonPressed("moveRight"))
	{
		nTime curTime = InputTimeSource::Instance()->GetTime();
		if ((curTime - this->moveGotoTime) > 0.25f)
		{
			this->SendMove(vector3(velocity, 0.0f, 0.0f));
		}
	}
}

//-----------------------------------------------------------------------------
/**
*/
/*
void
PlayerInputProperty::SendMoveDirection(const vector3& vel)
{
    const vector3& curPos = ;
    Ptr<Message::MoveGoto> msg = Message::MoveGoto::Create();

    vector3 nextPos = curPos + vel;
    msg->SetPosition(nextPos);

    GetEntity()->SendAsync(msg);
}
*/

};// end of namespace
