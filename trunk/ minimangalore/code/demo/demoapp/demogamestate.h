//----------------------------------------------------------------------------
//  demogamestate.h
//
//  (C)2007 Kim Hyoun Woo
//----------------------------------------------------------------------------
#ifndef DEMO_H
#define DEMO_H
//----------------------------------------------------------------------------
/**
    @class DemoGameState
    @ingroup 
    @brief a brief description of the class

*/
//----------------------------------------------------------------------------
#include "application/app.h"
#include "application/gamestatehandler.h"

//----------------------------------------------------------------------------
class DemoGameState : public Application::GameStateHandler
{
public:
    /// constructor
    DemoGameState();
    /// destructor
    virtual ~DemoGameState();

    /// @name Overrided functions from nAppState
    /// @{
    /// called when the state represented by this state handler is entered
    virtual void OnStateEnter(const nString& prevState);
    /// called when the state represented by this state handler is left
    virtual void OnStateLeave(const nString& nextState);
    /// called each frame as long as state is current, return new state
    virtual nString OnFrame();
    /// @}

protected:
	bool CreateDefaultLight();
	bool CreatePlayer();
	bool CreateBox();
	bool CreateCamera();
	bool CreateEnvironment();
    void CreateLevel();

protected:
	Ptr<Game::Entity> player;
    Ptr<Physics::Entity> playerPhysics;
};

//----------------------------------------------------------------------------
#endif // DEMO_H

