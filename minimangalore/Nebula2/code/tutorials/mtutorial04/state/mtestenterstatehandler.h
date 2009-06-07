#ifndef APPLICATION_mTestENTERSTATEHANDLER_H
#define APPLICATION_mTestENTERSTATEHANDLER_H
//------------------------------------------------------------------------------
/**
    @class Application::mTestEnterStateHandler

    The game state handler sets up the game world and runs the game loop.
    Setup can happen in one of several modes: 

    * EMPTY WORLD: doesn't require a world database
    * NEW GAME: game starts from a fresh copy of the world database
    * CONTINUE GAME: game continues at the last valid position
    * LOAD LEVEL: game starts at the given level
    * LOAD SAVE GAME: game starts with the given save game
    
    * the only purpose of this state is to load and set up level, db, etc...    
    
    (C) 2006 je.a.le@wanadoo.fr for nebula2
*/
#include "application/gamestatehandler.h"

//------------------------------------------------------------------------------
namespace Application
{
class mTestEnterStateHandler : public GameStateHandler
{
    DeclareRtti;
	DeclareFactory(mTestEnterStateHandler);

public:
    /// constructor
    mTestEnterStateHandler();
    /// destructor
    virtual ~mTestEnterStateHandler();
  
    /// called each frame as long as state is current, return new state
    virtual nString OnFrame();

     /// called when the state represented by this state handler is entered
    virtual void OnStateEnter(const nString& prevState);
    /// called when the state represented by this state handler is left
    virtual void OnStateLeave(const nString& nextState);
 
private:   
};

RegisterFactory(mTestEnterStateHandler);
}
//------------------------------------------------------------------------------
#endif
