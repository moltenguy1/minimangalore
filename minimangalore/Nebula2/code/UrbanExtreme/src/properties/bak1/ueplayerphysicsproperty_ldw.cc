#include "properties/ueplayerphysicsproperty.h"
#include "foundation/factory.h"
#include "physics/server.h"
#include "physics/level.h"
#include "game/time/gametimesource.h"
#include "mathlib/polar.h"
#include "graphics/server.h"
#include "graphics/cameraentity.h"
#include "managers/entitymanager.h"
#include "navigation/server.h"
#include "gfx2/ngfxserver2.h"

#include "msg/movesetvelocity.h"
#include "msg/updatetransform.h"
#include "msg/moverotate.h"
#include "msg/movedirection.h"
#include "msg/camerareset.h"

#include "attributes/ueattributes.h"

#include "msg/ueplayeraction.h"

#include "kernel/ntimeserver.h"

namespace Properties
{
ImplementRtti(Properties::UePlayerPhysicsProperty, Properties::ActorPhysicsProperty);
ImplementFactory(Properties::UePlayerPhysicsProperty);

using namespace Game;
using namespace Message;
using namespace Managers;

//------------------------------------------------------------------------------
/**
*/
UePlayerPhysicsProperty::UePlayerPhysicsProperty()
{

}

//------------------------------------------------------------------------------
/**
*/
UePlayerPhysicsProperty::~UePlayerPhysicsProperty()
{
	
}

void 
UePlayerPhysicsProperty::SetupDefaultAttributes()
{
	ActorPhysicsProperty::SetupDefaultAttributes();
	GetEntity()->SetBool(Attr::accelerate,false);
	GetEntity()->SetBool(Attr::left,false);
	GetEntity()->SetBool(Attr::right,false);
	GetEntity()->SetBool(Attr::dobreak,false);	
	GetEntity()->SetInt(Attr::pose,Stand);

	GetEntity()->SetBool(Attr::poseUp,false);
	GetEntity()->SetBool(Attr::poseDn,false);

	GetEntity()->SetFloat(Attr::RelVelocity,0.0f);
	GetEntity()->SetVector3(Attr::position,vector3(0.0f,0.0f,0.0f));
	matrix44 tm;

	GetEntity()->SetMatrix44(Attr::lastTM,tm);

	this->timeLastOnGroundPrev = nTimeServer::Instance()->GetTime();
	timeElapseAcc=0.0f;
	respone = false;

	acceleratePrev=	false;
	dobreakePrev=	false;
	leftPrev=		false;
	rightPrev=		false;
	poseUpPrev =	false;
	poseDnPrev =	false;
	posePrev=Stand;

	// 공기저항 흉내낸 감속 수치
	
	resistanceAir[Stand]	= -0.0010f;
	resistanceAir[Crounch]	= -0.0005f;
	resistanceAir[Prone]	= -0.0001f;

	// 땅의 저항을 흉내낸 감속 수치
	resistanceGround = -0.0010f;

	// 가속 수치.
	addVelocity[Stand]		= 0.01f;
	addVelocity[Crounch]	= 0.01f;
	addVelocity[Prone]		= 0.00f;		

	// 최대속도 수치
	maxVelocity[Stand]		= 2.0f;
	maxVelocity[Crounch]	= 2.0f;
	maxVelocity[Prone]		= 2.0f;

	// 브레이크 감속 수치
	breakVelocity[Stand]	= -0.01f;
	breakVelocity[Crounch]	= -0.01f;
	breakVelocity[Prone]	= -0.01f;	
	
	// 낙하 높이 체크 수치
	returnHeight = -100.0f;
}

bool
UePlayerPhysicsProperty::Accepts(Message::Msg* msg)
{
	return msg->CheckId(UePlayerKey::Id) ||		
		ActorPhysicsProperty::Accepts(msg);
}
void
UePlayerPhysicsProperty::HandleMessage(Message::Msg* msg)
{
	ActorPhysicsProperty::HandleMessage(msg);
}


//------------------------------------------------------------------------------
/**
enable and activate the physics
*/
void 
UePlayerPhysicsProperty::EnablePhysics()
{
	n_assert(!this->IsEnabled());

	// create a char physics entity
	this->charPhysicsEntity = Physics::UePlayerEntity::Create();
	this->charPhysicsEntity->SetUserData(GetEntity()->GetUniqueId());

	if (GetEntity()->HasAttr(Attr::Physics))
	{
		this->charPhysicsEntity->SetCompositeName(GetEntity()->GetString(Attr::Physics));
	}
	this->charPhysicsEntity->SetTransform(GetEntity()->GetMatrix44(Attr::Transform));

	// attach physics entity to physics level
	Physics::Level* physicsLevel = Physics::Server::Instance()->GetLevel();
	n_assert(physicsLevel);
	physicsLevel->AttachEntity(this->charPhysicsEntity);

	// make sure we are standing still
	this->Stop();

	// initialize feedback loops for motion smoothing
	nTime time = GameTimeSource::Instance()->GetTime();
	const matrix44& entityMatrix = GetEntity()->GetMatrix44(Attr::Transform);
	this->smoothedPosition.Reset(time, 0.001f, this->positionGain, entityMatrix.pos_component());

	polar2 headingAngle(entityMatrix.z_component());
	this->smoothedHeading.Reset(time, 0.001f, this->headingGain, headingAngle.rho);

	// call parent
	AbstractPhysicsProperty::EnablePhysics();
	
}


//------------------------------------------------------------------------------
/** 
disable and cleanup the physics
*/
void 
UePlayerPhysicsProperty::DisablePhysics()
{
	ActorPhysicsProperty::DisablePhysics();
}


void UePlayerPhysicsProperty::OnBeginFrame()
{
	float relVelocity = GetEntity()->GetFloat(Attr::RelVelocity);	
	bool accelerateCurr =  GetEntity()->GetBool(Attr::accelerate);
	bool dobreakCurr = GetEntity()->GetBool(Attr::dobreak);
	bool leftCurr = GetEntity()->GetBool(Attr::left);
	bool rightCurr = GetEntity()->GetBool(Attr::right);
	bool poseUpCurr = GetEntity()->GetBool(Attr::poseUp);
	bool poseDnCurr = GetEntity()->GetBool(Attr::poseDn);
	bool poseChange=false;

	int poseCurr = GetEntity()->GetInt(Attr::pose);

	const vector3 runDirection = vector3(0.0f,0.0f,-1.0f);


	Physics::UePlayerEntity* entity=(Physics::UePlayerEntity*)GetPhysicsEntity();
	matrix44 mat = GetPhysicsEntity()->GetTransform();
	vector3 pos = mat.pos_component();

	nTime timeCurr = nTimeServer::Instance()->GetTime();
	nTime timeElapse = timeCurr - this->timeLastOnGroundPrev;		
	this->timeElapseAcc+=timeElapse;

	if ( this->timeElapseAcc > 1.0f)
	{
		if ( entity->GetOnGround())
		{
			this->lastTM	= mat;	
			GetEntity()->SetMatrix44(Attr::lastTM,mat);
		}		
		this->timeElapseAcc = 0.0f;
	}
	this->timeLastOnGroundPrev =  timeCurr;		


	if (pos.y < returnHeight)
	{
		respone = true;
	}
	

	// poseUp,poseDn 키에의한 자세변화
	if (poseUpCurr)
	{
		if (!poseUpPrev)
		{
			if(poseCurr==Crounch ) 
			{	
				poseCurr = Stand;
				poseChange=true;
			}
			else if(poseCurr==Prone) 
			{
				poseCurr = Crounch;
				poseChange=true;
			}
		}
	}
	else if(poseDnCurr)
	{
		if (!poseDnPrev)
		{
			if (relVelocity  > 0.0f)
			{
				if(poseCurr==Stand )
				{
					poseCurr = Crounch;
					poseChange=true;
				}
				else if(poseCurr==Crounch) 
				{
					poseCurr = Prone;	
					poseChange=true;
				}
			}		
		}
	}
	


	// accelerate,dobreak 키에의한 속도 변화
	if (accelerateCurr)
	{
		Ptr<Message::MoveDirection> msg = Message::MoveDirection::Create();
		msg->SetDirection(vector3(0.0f,0.0f,-1.0f));
		msg->SetCameraRelative(true);
		GetEntity()->SendSync(msg);
	
		relVelocity += addVelocity[poseCurr];
		if ( relVelocity > maxVelocity[poseCurr])
		{
			relVelocity = maxVelocity[poseCurr];
		}		
	}
	else if (dobreakCurr)
	{
		Ptr<MoveDirection> msg = MoveDirection::Create();
		msg->SetDirection(vector3(0.0f,0.0f,-1.0f));
		msg->SetCameraRelative(true);
		GetEntity()->SendSync(msg);

		relVelocity += breakVelocity[poseCurr];
		if ( relVelocity < 0.0f)
		{
			relVelocity=0.0f;
		}	
	}	

	// left,right 키에의한 속도 변화
	if (leftCurr)
	{		
		
	}
	else if (rightCurr)
	{
	
	}				

	// 저항에 의한 속도 감속
	relVelocity += resistanceAir[poseCurr];
	relVelocity += resistanceGround;

	if (relVelocity < 0.0f)
	{
		relVelocity = 0.0f;	
	}
	else
	{
		//ActorPhysicsProperty::HandleMoveDirection()
		Ptr<MoveDirection> msg = MoveDirection::Create();
		msg->SetDirection(vector3(0.0f,0.0f,-1.0f));
		msg->SetCameraRelative(true);
		GetEntity()->SendSync(msg);
	}
	


	// 액션 메시지
	if (accelerateCurr)
	{
		if (!acceleratePrev || poseChange)
		{
			Ptr<Message::UePlayerAction> msg = Message::UePlayerAction::Create();
			switch(poseCurr)
			{
			case Stand:		msg->SetActionType(Message::UePlayerAction::S_Run);	break;
			case Crounch:	msg->SetActionType(Message::UePlayerAction::C_Run);	break;
			case Prone:		msg->SetActionType(Message::UePlayerAction::P_Speed);	break;	
			}
			GetEntity()->SendAsync(msg);			
		}	
	}
	else 
	{	
		if (relVelocity == 0.0f)
		{
			ActorPhysicsProperty::SendStop();
			poseCurr = Stand;

			Ptr<Message::UePlayerAction> msg = Message::UePlayerAction::Create();
			msg->SetActionType(Message::UePlayerAction::S_Idle);
			GetEntity()->SendAsync(msg);
		}
		else if (leftCurr)
		{	
			if (!leftPrev || !acceleratePrev || poseChange)
			{
				Ptr<Message::UePlayerAction> msg = Message::UePlayerAction::Create();
				switch(poseCurr)
				{
				case Stand:		msg->SetActionType(Message::UePlayerAction::S_Angle_L);	break;
				case Crounch:	msg->SetActionType(Message::UePlayerAction::C_Angle_L);	break;
				case Prone:		msg->SetActionType(Message::UePlayerAction::P_Angle_L);	break;
				}
				
				GetEntity()->SendAsync(msg);
			}		
		}
		else if (rightCurr)
		{
			if (!rightPrev || !acceleratePrev || poseChange)
			{
				Ptr<Message::UePlayerAction> msg = Message::UePlayerAction::Create();
				switch(poseCurr)
				{
				case Stand:		msg->SetActionType(Message::UePlayerAction::S_Angle_R);	break;
				case Crounch:	msg->SetActionType(Message::UePlayerAction::C_Angle_R);	break;
				case Prone:		msg->SetActionType(Message::UePlayerAction::P_Angle_R);	break;
				}
				GetEntity()->SendAsync(msg);
			}
		}
		else 
		{
			if( !acceleratePrev )
			{
				Ptr<Message::UePlayerAction> msg = Message::UePlayerAction::Create();
				switch(poseCurr)
				{
				case Stand:		msg->SetActionType(Message::UePlayerAction::S_Speed);	break;
				case Crounch:	msg->SetActionType(Message::UePlayerAction::C_Speed);	break;
				case Prone:		msg->SetActionType(Message::UePlayerAction::P_Speed);	break;
				}
				GetEntity()->SendAsync(msg);
			}
		}
	}


	GetEntity()->SetInt(Attr::pose,poseCurr);
	GetEntity()->SetFloat(Attr::RelVelocity,relVelocity);

	acceleratePrev =	accelerateCurr;
	dobreakePrev =		dobreakCurr;
	leftPrev =			leftCurr;
	rightPrev =			rightCurr;
	posePrev =			poseCurr;
	poseUpPrev =		poseUpCurr;
	poseDnPrev =		poseDnCurr;
	ActorPhysicsProperty::OnBeginFrame();
}

void UePlayerPhysicsProperty::OnMoveAfter()
{



	if (respone)
	{
		GetPhysicsEntity()->SetTransform(this->lastTM);
		Ptr<Message::SetTransform> setTmMsg = Message::SetTransform::Create();				
		setTmMsg->SetMatrix(this->lastTM);
		GetEntity()->SendSync(setTmMsg);	
	
		Ptr<Message::UpdateTransform> updateTmMsg = Message::UpdateTransform::Create();			
		updateTmMsg->SetMatrix(this->lastTM);
		GetEntity()->SendSync(updateTmMsg);
		
		GetEntity()->SetFloat(Attr::RelVelocity,0.0f);
		respone =false;	
	}
	ActorPhysicsProperty::OnMoveAfter();
}

} // namespace Properties