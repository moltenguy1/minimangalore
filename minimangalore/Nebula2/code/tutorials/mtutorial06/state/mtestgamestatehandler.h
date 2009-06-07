#ifndef APPLICATION_mTestGAMESTATEHANDLER_H
#define APPLICATION_mTestGAMESTATEHANDLER_H
//------------------------------------------------------------------------------
/**
    @class Application::mTestGameStateHandler

    The game state handler sets up the game world and runs the game loop.
    Setup can happen in one of several modes: 

    * EMPTY WORLD: doesn't require a world database
    * NEW GAME: game starts from a fresh copy of the world database
    * CONTINUE GAME: game continues at the last valid position
    * LOAD LEVEL: game starts at the given level
    * LOAD SAVE GAME: game starts with the given save game
    
    (C) 2006 je.a.le@wanadoo.fr for nebula2
*/
#include "application/gamestatehandler.h"


//------------------------------------------------------------------------------
namespace Application
{
class mTestGameStateHandler : public GameStateHandler
{
    DeclareRtti;
	DeclareFactory(mTestGameStateHandler);

public:
        /// constructor
    mTestGameStateHandler();
    /// destructor
    virtual ~mTestGameStateHandler();
    /// set the setup mode
     /// called each frame as long as state is current, return new state
    virtual nString OnFrame();
};

RegisterFactory(mTestGameStateHandler);
}
//------------------------------------------------------------------------------
#endif
