#ifndef PROPERTIES_PLAYERPHYSICSPROPERTY_H
#define PROPERTIES_PLAYERPHYSICSPROPERTY_H
//------------------------------------------------------------------------------
/**
    @class Properties::PlayerPhysicsProperty

    PlayerInputProperty 에서 broadcasting한 메시지를 후킹,
	PlayerEntity에 필요한 값들을 설정한다.
*/
#include "properties/abstractphysicsproperty.h"
#include "msg/playerstatus.h"
#include "game/entity.h"
#include "physics/playerentity.h"

namespace Properties
{
class Message::PlayerStatus;

class PlayerPhysicsProperty : public AbstractPhysicsProperty
{
    DeclareRtti;
    DeclareFactory(PlayerPhysicsProperty);

public:
    /// constructor
    PlayerPhysicsProperty();
    /// destructor
    virtual ~PlayerPhysicsProperty();

    /// setup default entity attributes
    virtual void SetupDefaultAttributes();

    /// called before movement has happened
    virtual void OnMoveBefore();
    /// called after movement has happened
    virtual void OnMoveAfter();
    /// called on debug visualization
    virtual void OnRenderDebug();

    /// get a pointer to the physics entity
    virtual Physics::Entity* GetPhysicsEntity() const;

    /// return true if message is accepted by controller
    virtual bool Accepts(Message::Msg* msg);
    /// handle a single message
    virtual void HandleMessage(Message::Msg* msg);

protected:
    /// enable and activate the physics
    virtual void EnablePhysics();
    /// disable and cleanup the physics
    virtual void DisablePhysics();

	void HandlePlayerAction(Message::PlayerStatus* msg);

protected:

    Ptr<Physics::PlayerEntity> playerPhysicsEntity;

};

RegisterFactory(PlayerPhysicsProperty);

} // namespace Properties
#endif
