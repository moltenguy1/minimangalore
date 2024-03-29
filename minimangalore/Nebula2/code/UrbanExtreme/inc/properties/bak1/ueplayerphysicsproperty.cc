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

	acceleratePrev=	false;
	dobreakePrev=	false;
	leftPrev=		false;
	rightPrev=		false;
	poseUpPrev =	false;
	poseDnPrev =	false;
	posePrev=Stand;

	// 공기저항 흉내낸 감속 수치
	
	resistanceAir[Stand]	= -0.0080f;
	resistanceAir[Crounch]	= -0.0030f;
	resistanceAir[Prone]	= -0.0010f;

	// 땅의 저항을 흉내낸 감속 수치
	resistanceGround = -0.0020f;

	// 가속 수치.
	defVelocity[Stand]		= 0.03f;
	defVelocity[Crounch]	= 0.0020f;
	defVelocity[Prone]		= 0.0001f;		

	addVelocity[Stand]		= 0.03f;
	addVelocity[Crounch]	= 0.0020f;
	addVelocity[Prone]		= 0.0001f;		

	accVelocity[Stand]		= 0.03f;
	accVelocity[Crounch]	= 0.00008f;
	accVelocity[Prone]		= 0.00009f;	

	tempAccVelocity = accVelocity[Stand];

	// 최대속도 수치
	maxVelocity[Stand]		= 1.2f;
	maxVelocity[Crounch]	= 1.7f;
	maxVelocity[Prone]		= 3.5f;

	// 브레이크 감속 수치
	breakVelocity[Stand]	= -0.03f;
	breakVelocity[Crounch]	= -0.03f;
	breakVelocity[Prone]	= -0.03f;	
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
	
		if ( relVelocity < maxVelocity[poseCurr])
		{
			//reladdVelocity += (addVelocity[poseCurr] + accVelocity[poseCurr]);
			//relVelocity += reladdVelocity;
			
			//addVelocity[poseCurr] += accVelocity[poseCurr];
			addVelocity[poseCurr] += tempAccVelocity;
			
			relVelocity += addVelocity[poseCurr];		
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
		//
		tempAccVelocity = accVelocity[poseCurr];

		addVelocity[Stand]=		defVelocity[Stand];
		addVelocity[Crounch]=	defVelocity[Crounch];
		addVelocity[Prone]=		defVelocity[Prone];

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
	Physics::UePlayerEntity* entity=(Physics::UePlayerEntity*)GetPhysicsEntity();
	if (entity->GetOnGround())
	{
		lastPos = GetPhysicsEntity()->GetTransform().pos_component();
	}
	ActorPhysicsProperty::OnMoveAfter();
}

} // namespace Properties