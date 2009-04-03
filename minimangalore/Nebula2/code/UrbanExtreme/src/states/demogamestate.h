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


class nGuiCanvas;

class nGuiTextLabel;

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

	enum STATE { READY,START,FINISH };
protected:
	bool CreateDefaultLight();
	
	bool CreatePlayer();
	bool CreateBox();
	bool CreateCamera();
	bool CreateEnvironment();
    void CreateLevel();

	void CreateGUI();

	bool IsUePlayerInGoalPos();
	void AddDebugString(nString& str);
	void SetSceneVariable();
	void CalculateRecord();

	void UI_MeterShow();
protected:

	nGuiTextLabel*		guiTL_DebugText;
	nGuiCanvas*			guiCanvasHow;
	
	nGuiCanvas*			guiReady;
	nGuiCanvas*			guiStart;
	nGuiCanvas*			guiFinish;

	nGuiCanvas*			guiEnding[4];
	nGuiCanvas*			guiNewRecord;

	nGuiCanvas*			guiMeter[11];



	

	Ptr<Game::Entity>	player;	

	nString				debugString;

	
	
	STATE				state;				//					
	bool				onShowHelp;			//	
	vector3				posGoal;

	nTime				timeReadyAcc;
	nTime				timeStartAcc;
	nTime				timeFinishAcc;
	nTime				timeFinish;

	nTime				timePrev;
	nTime				timeAcc;

	int					recordMin;
	int					recordSec;
	int					recordMs;
};

//----------------------------------------------------------------------------
#endif // DEMO_H

