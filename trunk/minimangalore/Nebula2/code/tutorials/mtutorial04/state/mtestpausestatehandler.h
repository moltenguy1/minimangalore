#ifndef APPLICATION_mTestPAUSESTATEHANDLER_H
#define APPLICATION_mTestPAUSESTATEHANDLER_H
//------------------------------------------------------------------------------
/**
    @class Application::mTestPauseStateHandler

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
#include "gui/nguitextlabel.h" 

//------------------------------------------------------------------------------
namespace Application
{
class mTestPauseStateHandler : public GameStateHandler
{
    DeclareRtti;
	DeclareFactory(mTestPauseStateHandler);

public:
    /// constructor
    mTestPauseStateHandler();
    /// destructor
    virtual ~mTestPauseStateHandler();
    
    /// called when attached to application object
    /// Now we will create label and other data that belong to game state
    virtual void OnAttachToApplication();
   
    /// called each frame as long as state is current, return new state
    virtual nString OnFrame();

     /// called when the state represented by this state handler is entered
    virtual void OnStateEnter(const nString& prevState);
    /// called when the state represented by this state handler is left
    virtual void OnStateLeave(const nString& nextState);
 
private:   
   /// setup the gui system (for this state and only !)
    void SetupGui();
    
    // a label for information (not so much "pause")
  	nGuiTextLabel* textLabel; 
};

RegisterFactory(mTestPauseStateHandler);
}
//------------------------------------------------------------------------------
#endif
