#ifndef PHYSICS_PLAYERENTITY_H
#define PHYSICS_PLAYERENTITY_H
//------------------------------------------------------------------------------
/**
*/
#include "physics/entity.h"
#include "physics/materialtable.h"
#include "physics/filterset.h"
#include "character/ncharacter2.h"

//------------------------------------------------------------------------------
namespace Physics
{
class RigidBody;
class BoxShape;
class Composite;

class PlayerEntity : public Entity
{
    DeclareRtti;
    DeclareFactory(PlayerEntity);

public:
    /// constructor
    PlayerEntity(); 
    /// destructor.
    virtual ~PlayerEntity();
    /// called when attached to game entity
    virtual void OnActivate();
    /// called when removed from game entity
    virtual void OnDeactivate();
    /// invoked before stepping the simulation
    virtual void OnStepBefore();
    /// called on collision; decide if it is valid
    virtual bool OnCollide(Shape* collidee);
    /// set the current world space transformation
    virtual void SetTransform(const matrix44& m);
    /// get the current world space transformation
    virtual matrix44 GetTransform() const;
    /// return true if transformation has changed between OnFrameBefore() and OnFrameAfter()
    virtual bool HasTransformChanged() const;

    /// set pointer to Nebula2 character object
    void SetCharacter(nCharacter2* chr);
    /// get pointer to Nebula2 character
    nCharacter2* GetCharacter() const;

	RigidBody* GetRigidBody() const;
    
    ///
	void SetForwardForce(float);
	///
	void SetLeftwardForce(float);
	///
	void SetStraft(float);
	///
	void SetTurnTorque(float);
    ///
	void SetJump(bool);
	///
	void SetTimeToJump(float);
	///
	void SetUpForce(float);

protected:
	bool CheckGround(float& dist, float& depth);

protected:
    Ptr<RigidBody> baseBody;
    Ptr<Composite> defaultComposite;
    float radius;
    float height;
    float hover;
    nCharacter2* nebCharacter;
    FilterSet groundExcludeSet;
    MaterialType groundMaterial;

    vector3 lookatDirection;    // looks into last valid desiredVelocity direction

    bool touched;
    bool floorContact;

	float forwardForce;
	float leftwardForce;
	float strafing;
	float turnTorque;
	bool jump;
    float timeToJump;
	float upForce;

};

RegisterFactory(PlayerEntity);


//------------------------------------------------------------------------------
/**
*/
inline
void
PlayerEntity::SetCharacter(nCharacter2* chr)
{
    if (this->nebCharacter)
    {
        this->nebCharacter->Release();
        this->nebCharacter = 0;
    }
    if (chr)
    {
        this->nebCharacter = chr;
        this->nebCharacter->AddRef();
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
nCharacter2*
PlayerEntity::GetCharacter() const
{
    return this->nebCharacter;
}

//------------------------------------------------------------------------------
/**
*/
inline
RigidBody* 
PlayerEntity::GetRigidBody() const
{
	return this->baseBody;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
PlayerEntity::SetForwardForce(float fForce)
{
	this->forwardForce = fForce;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
PlayerEntity::SetLeftwardForce(float lForce)
{
	this->leftwardForce = lForce;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
PlayerEntity::SetStraft(float straft)
{
	this->strafing = straft;
}

//------------------------------------------------------------------------------
/**
*/
inline
void 
PlayerEntity::SetTurnTorque(float rTurn)
{
	this->turnTorque = rTurn;
}
//------------------------------------------------------------------------------
/**
*/
inline
void
PlayerEntity::SetJump(bool j)
{
	this->jump = j;
}
//------------------------------------------------------------------------------
/**
*/
inline
void 
PlayerEntity::SetTimeToJump(float tJump)
{
	this->timeToJump = tJump;
}

inline
void
PlayerEntity::SetUpForce(float uForce)
{
	this->upForce = uForce;
}

}
#endif
