#ifndef GAMEMENU_H
#define GAMEMENU_H

#include "application/app.h"
#include "application/gamestatehandler.h"
#include "gui/nguieventhandler.h"
#include "gui/nguicanvas.h"

class nGuiBrush;
class nGuiTextLabel;
class nGuiButton;
class FinishState :
 	public Application::GameStateHandler,
 	public nGuiEventHandler
{
public:
	FinishState();
	virtual ~FinishState();

	virtual void OnStateEnter(const nString& prevState);
	virtual void OnStateLeave(const nString& nextState);
	virtual void OnAttachToApplication();
	virtual nString OnFrame();

	void		 SetupGui();

	virtual void HandleEvent(const nGuiEvent& e);

	nGuiCanvas*			guiCanvas;
	nGuiButton*			guibutton;
	nGuiTextLabel*		guiTextLabel;
	nGuiBrush*			guiBrush;
};

#endif