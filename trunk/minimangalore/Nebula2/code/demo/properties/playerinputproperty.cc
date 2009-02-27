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
#include "msg/animstop.h"
#include "msg/cameraorbit.h"
#include "msg/movedirection.h"
#include "msg/movegoto.h"
#include "msg/movestop.h"
#include "msg/moveturn.h"
#include "msg/moverotate.h"
#include "msg/playerstatus.h"
#include "msg/cameradistance.h"
#include "msg/camerareset.h"
#include "msg/updatetransform.h"

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

void PlayerInputProperty::SendMoveDirection(const vector3& dir)
{
	Ptr<Message::MoveDirection> msg = Message::MoveDirection::Create();

	msg->SetDirection(dir);
	msg->SetCameraRelative(true);

	GetEntity()->SendAsync(msg);	
}

//////////////////////////////////////////////////////////////////////////
// void SendTurn(const vector3& dir)
//////////////////////////////////////////////////////////////////////////
void PlayerInputProperty::SendTurn(const float dir)
{
	Ptr<Message::MoveRotate> msg = Message::MoveRotate::Create();
	Ptr<Message::UpdateTransform> Updatemsg = Message::UpdateTransform::Create();

	matrix44 mat = GetEntity()->GetMatrix44(Attr::Transform);

	float c = n_cos(dir);
	float s = n_sin(dir);
	int i;
	for( i=0 ; i<4 ; i++ )
	{
		float mi0 = mat.m[i][0];
		float mi2 = mat.m[i][2];
		mat.m[i][0] = mi0*c + mi2*s;
		mat.m[i][2] = mi0*-s + mi2*c;
	}

	Updatemsg->SetMatrix(mat);

	GetEntity()->SendAsync(Updatemsg);

	this->moveGotoTime = InputTimeSource::Instance()->GetTime();
}

void PlayerInputProperty::SendTurnTest(const float dir)
{
	Ptr<Message::MoveRotate> msg = Message::MoveRotate::Create();

	msg->SetAngle(dir);

	GetEntity()->SendAsync(msg);
}

void PlayerInputProperty::SendTurnTest2(const vector3& dir)
{
	Ptr<Message::MoveTurn> msg = Message::MoveTurn::Create();

	msg->SetDirection(dir);
	msg->SetCameraRelative(true);

	GetEntity()->SendAsync(msg);
}

//////////////////////////////////////////////////////////////////////////
// void HandleInput()
//////////////////////////////////////////////////////////////////////////
void PlayerInputProperty::HandleInput()
{
    Input::Server* inputServer = Input::Server::Instance();

	this->playerMsg.Clear();

	if (inputServer->GetButtonPressed("mouseLMB"))
	{
		float x = inputServer->GetSlider("mouseLeft") - inputServer->GetSlider("mouseRight");		
		this->SendTurnTest(x / 16);		

		Ptr<Message::CameraReset> cameramsg = Message::CameraReset::Create();
		GetEntity()->SendAsync(cameramsg);
	}

	if( inputServer->GetButtonPressed("mouseRMB") )
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

	const float velocity = 4.0f;

	if (inputServer->GetButtonDown("moveForward"))
	{
		this->SendMoveDirection(vector3(0.0f, 0.0f, -velocity));
	}
	else if(inputServer->GetButtonPressed("moveForward"))
	{
		nTime curTime = InputTimeSource::Instance()->GetTime();
		if ((curTime - this->moveGotoTime) > 0.25f)
		{
			this->SendMoveDirection(vector3(0.0f, 0.0f, -velocity));
		}
	}

	if( inputServer->GetButtonDown("moveBackward"))
	{
		this->SendMoveDirection(vector3(0.0f, 0.0f, velocity));
	}
	else if( inputServer->GetButtonPressed("moveBackward"))
	{
		nTime curTime = InputTimeSource::Instance()->GetTime();
		if ((curTime - this->moveGotoTime) > 0.25f)
		{
			this->SendMoveDirection(vector3(0.0f, 0.0f, velocity));
		}
	}

	if (inputServer->GetButtonDown("moveLeft"))
	{
		this->SendMoveDirection(vector3(-velocity, 0.0f, 0.0f));
	}
	else if(inputServer->GetButtonPressed("moveLeft"))
	{
		nTime curTime = InputTimeSource::Instance()->GetTime();
		if ((curTime - this->moveGotoTime) > 0.25f)
		{
			this->SendMoveDirection(vector3(-velocity, 0.0f, 0.0f));
		}
	}

	if (inputServer->GetButtonDown("moveRight"))
	{
		this->SendMoveDirection(vector3(velocity, 0.0f, 0.0f));

	}
	else if(inputServer->GetButtonPressed("moveRight"))
	{
		nTime curTime = InputTimeSource::Instance()->GetTime();
		if ((curTime - this->moveGotoTime) > 0.25f)
		{
			this->SendMoveDirection(vector3(velocity, 0.0f, 0.0f));
		}
	}

	if( inputServer->GetButtonUp("moveForward") || inputServer->GetButtonUp("moveBackward") ||
		inputServer->GetButtonUp("moveLeft") || inputServer->GetButtonUp("moveRight") )
	{
		Ptr<Message::MoveStop> msg = Message::MoveStop::Create();
		GetEntity()->SendAsync(msg);

		Ptr<Message::CameraReset> cameramsg = Message::CameraReset::Create();
		GetEntity()->SendAsync(cameramsg);
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
