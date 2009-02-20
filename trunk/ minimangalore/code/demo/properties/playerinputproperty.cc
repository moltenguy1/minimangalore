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
#include "msg/moveturn.h"
#include "msg/moverotate.h"
#include "msg/playerstatus.h"
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

//////////////////////////////////////////////////////////////////////////
// void HandleInput()
//////////////////////////////////////////////////////////////////////////
void PlayerInputProperty::HandleInput()
{
    Input::Server* inputServer = Input::Server::Instance();

	this->playerMsg.Clear();

	if (inputServer->GetButtonPressed("mouseLMB"))
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

	if( inputServer->GetButtonPressed("mouseRMB") )
	{
		float x = inputServer->GetSlider("mouseLeft") - inputServer->GetSlider("mouseRight");
		float y = inputServer->GetSlider("mouseUp") - inputServer->GetSlider("mouseDown");

		Ptr<Message::CameraOrbit> camMsg = Message::CameraOrbit::Create();
		camMsg->SetHorizontalRotation(x);
		camMsg->SetVerticalRotation(y);
		GetEntity()->SendAsync(camMsg);

		//this->SendTurn();
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

	if( inputServer->GetButtonDown("moveBackward"))
	{
		this->SendMove(vector3(0.0f, 0.0f, velocity));
	}
	else if( inputServer->GetButtonPressed("moveBackward"))
	{
		nTime curTime = InputTimeSource::Instance()->GetTime();
		if ((curTime - this->moveGotoTime) > 0.25f)
		{
			this->SendMove(vector3(0.0f, 0.0f, velocity));
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
