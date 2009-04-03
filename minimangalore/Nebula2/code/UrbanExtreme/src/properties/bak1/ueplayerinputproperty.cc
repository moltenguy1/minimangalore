//-----------------------------------------------------------------------------
//  properties/playerinputproperty.cc

#include "properties/ueplayerinputproperty.h"
#include "properties/ueplayerphysicsproperty.h"

#include "managers/focusmanager.h"
#include "managers/envquerymanager.h"
#include "managers/timemanager.h"

#include "input/server.h"
#include "foundation/factory.h"

#include "msg/animstop.h"
#include "msg/cameraorbit.h"
#include "msg/movedirection.h"
#include "msg/movegoto.h"
#include "msg/movestop.h"
#include "msg/moveturn.h"
#include "msg/moverotate.h"
#include "msg/cameradistance.h"
#include "msg/camerareset.h"
#include "msg/updatetransform.h"
#include "msg/gfxsetanimation.h"
#include "msg/ueplayerkey.h"

#include "game/time/inputtimesource.h"
#include "game/entity.h"

//#include "attr/attributes.h"
#include "attributes/ueattributes.h"

namespace Properties
{
ImplementRtti(Properties::UePlayerInputProperty, Properties::InputProperty);
ImplementFactory(Properties::UePlayerInputProperty);

using namespace Game;
using namespace Managers;

//-----------------------------------------------------------------------------
/**
*/
UePlayerInputProperty::UePlayerInputProperty()
{

}

//-----------------------------------------------------------------------------
/**
*/
UePlayerInputProperty::~UePlayerInputProperty()
{
}

//-----------------------------------------------------------------------------
/**
*/
void
UePlayerInputProperty::OnBeginFrame()
{
    // only do something if we have the input focus
    if (FocusManager::Instance()->GetInputFocusEntity() == GetEntity())
    {
        this->HandleInput();
	}
    InputProperty::OnBeginFrame();
}

void
UePlayerInputProperty::SendMove(const vector3& dir)
{
    // send a MoveGoto to the mouse intersection point
    //const vector3& mousePos = envQueryMgr->GetMousePos3d();

    const vector3& from = GetEntity()->GetMatrix44(Attr::Transform).pos_component();

	Ptr<Message::MoveGoto> msg = Message::MoveGoto::Create();
    
	vector3 to = from;
	to += dir;
	msg->SetPosition(to);
	
	GetEntity()->SendSync(msg);

    // record the current time for the HandleLMBPressed() method
    this->moveGotoTime = InputTimeSource::Instance()->GetTime();
}

void UePlayerInputProperty::SendMoveDirection(const vector3& dir)
{
	Ptr<Message::MoveDirection> msg = Message::MoveDirection::Create();

	msg->SetDirection(dir);
	msg->SetCameraRelative(true);

	GetEntity()->SendSync(msg);	
}

//////////////////////////////////////////////////////////////////////////
// void SendTurn(const vector3& dir)
//////////////////////////////////////////////////////////////////////////
void UePlayerInputProperty::SendTurn(const float dir)
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

	GetEntity()->SendSync(Updatemsg);

	this->moveGotoTime = InputTimeSource::Instance()->GetTime();
}

void UePlayerInputProperty::SendTurnTest(const float dir)
{
	Ptr<Message::MoveRotate> msg = Message::MoveRotate::Create();

	msg->SetAngle(dir);

	GetEntity()->SendAsync(msg);
}

void UePlayerInputProperty::SendTurnTest2(const vector3& dir)
{
	Ptr<Message::MoveTurn> msg = Message::MoveTurn::Create();

	msg->SetDirection(dir);
	msg->SetCameraRelative(true);

	GetEntity()->SendAsync(msg);
}

//////////////////////////////////////////////////////////////////////////
// void HandleInput()
//////////////////////////////////////////////////////////////////////////
void UePlayerInputProperty::HandleInput()
{	
    Input::Server* inputServer = Input::Server::Instance();

	

	if (inputServer->GetButtonDown("moveForward"))
	{	
		GetEntity()->SetBool(Attr::accelerate,true);
	}
	else if (inputServer->GetButtonUp("moveForward"))
	{
		GetEntity()->SetBool(Attr::accelerate,false);
	}
	
	if (inputServer->GetButtonDown("moveLeft"))
	{	
		GetEntity()->SetBool(Attr::left,true);
	}
	else if (inputServer->GetButtonUp("moveLeft"))
	{
		GetEntity()->SetBool(Attr::left,false);
	}		

	if (inputServer->GetButtonDown("moveRight"))
	{	
		GetEntity()->SetBool(Attr::right,true);
	}
	else if (inputServer->GetButtonUp("moveRight"))
	{
		GetEntity()->SetBool(Attr::right,false);
	}

	if (inputServer->GetButtonDown("moveBackward"))
	{	
		GetEntity()->SetBool(Attr::dobreak,true);
	}
	else if (inputServer->GetButtonUp("moveBackward"))
	{
		GetEntity()->SetBool(Attr::dobreak,false);
	}

	if (inputServer->GetButtonDown("poseUp"))
	{	
		GetEntity()->SetBool(Attr::poseUp,true);
	}
	else if (inputServer->GetButtonUp("poseUp"))
	{
		GetEntity()->SetBool(Attr::poseUp,false);
	}

	if (inputServer->GetButtonDown("poseDn"))
	{	
		GetEntity()->SetBool(Attr::poseDn,true);
	}
	else if (inputServer->GetButtonUp("poseDn"))
	{
		GetEntity()->SetBool(Attr::poseDn,false);
	}

	// 포즈에 따른 회전 각
	const float	runRotate[3] = { 12.0f,20.0f,8.0f };
	const float stopRotate = 3.0f;
	float relVelocity = GetEntity()->GetFloat(Attr::RelVelocity);	
	int poseCurr = GetEntity()->GetInt(Attr::pose);

	if (inputServer->GetButtonPressed("moveLeft"))
	{	
		if (relVelocity == 0.0f)	
		{
			Ptr<Message::MoveRotate> msg = Message::MoveRotate::Create();
			msg->SetAngle(n_deg2rad(stopRotate));
			GetEntity()->SendSync(msg);
		}
		else
		{
			Ptr<Message::MoveRotate> msg = Message::MoveRotate::Create();
			msg->SetAngle(n_deg2rad(runRotate[poseCurr]));
			GetEntity()->SendSync(msg);
		}
		Ptr<Message::CameraReset> cameramsg = Message::CameraReset::Create();
		GetEntity()->SendSync(cameramsg);		
	}
	else if (inputServer->GetButtonPressed("moveRight"))
	{
		if (relVelocity == 0.0f)	
		{
			Ptr<Message::MoveRotate> msg = Message::MoveRotate::Create();
			msg->SetAngle(-n_deg2rad(stopRotate));
			GetEntity()->SendSync(msg);
		}
		else
		{
			Ptr<Message::MoveRotate> msg = Message::MoveRotate::Create();
			msg->SetAngle(-n_deg2rad(runRotate[poseCurr]));
			GetEntity()->SendSync(msg);
		}
	
		Ptr<Message::CameraReset> cameramsg = Message::CameraReset::Create();
		GetEntity()->SendSync(cameramsg);		
	}	
	GetEntity()->SetFloat(Attr::RelVelocity,relVelocity);
}

void UePlayerInputProperty::SendRotate( const float angle )
{
	Ptr<Message::MoveRotate> msg = Message::MoveRotate::Create();
	msg->SetAngle(angle);

	GetEntity()->SendSync(msg);
}


};// end of namespace
