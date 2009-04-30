#include "startmenustate.h"
#include "gui/nguiserver.h"
#include "gui/nguibutton.h"
#include "gui/nguitextlabel.h"
#include "gui/nguibrush.h"
#include "gui/nguiresource.h"
#include "gui/nguiwindow.h"
#include "gui/nguievent.h"
#include "ui/factorymanager.h"
#include "ui/element.h"
#include "app/demoapp.h"
#include "audio/server.h" 

//////////////////////////////////////////////////////////////////////////
// GameMenuState()
//////////////////////////////////////////////////////////////////////////
GameMenuState::GameMenuState()
{
	this->SetName("StartMenuState");
	this->SetSetupMode(EmptyWorld);
}

//////////////////////////////////////////////////////////////////////////
// ~GameMenuState()
//////////////////////////////////////////////////////////////////////////
GameMenuState::~GameMenuState()
{

}

//////////////////////////////////////////////////////////////////////////
// virtual void virtual void OnAttachToApplication()
//////////////////////////////////////////////////////////////////////////
void GameMenuState::OnAttachToApplication()
{
	SetupGui();
}

//////////////////////////////////////////////////////////////////////////
// virtual void OnStateEnter(const nString& prevState)
//////////////////////////////////////////////////////////////////////////
void GameMenuState::OnStateEnter(const nString& prevState)
{	
	guiCanvas->Show();
 	guiBtGameStart->Show();	
	guiCredit->Show();	
 	guiBtExit->Show();	

	Audio::Server::Instance()->PlaySoundEffect("BGM", vector3(0.0f, 0.0f, 0.0f), vector3(0.0f, 0.0f, 0.0f), 1.0f); 
	GameStateHandler::OnStateEnter(prevState);
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void GameMenuState::OnStateLeave(const nString& nextState)
{
	guiCanvas->Hide();
 	guiBtGameStart->Hide();	
 	guiCredit->Hide();	
 	guiBtExit->Hide();	

	GameStateHandler::OnStateLeave(nextState);
}

//////////////////////////////////////////////////////////////////////////
// virtual nString OnFrame()
//////////////////////////////////////////////////////////////////////////
nString GameMenuState::OnFrame()
{	
	 Input::Server* inputServer = Input::Server::Instance();
	 
	vector2 mousePos = inputServer->GetMousePos();

	if ((mousePos.y > 251.0f/768.0f)&&(mousePos.y < 283.0f/768.0f))
	{
		this->guiCredit->Show();
	}
	else
	{
		this->guiCredit->Hide();
	}

	return GameStateHandler::OnFrame();
}

//////////////////////////////////////////////////////////////////////////
// void SetupGui()
//////////////////////////////////////////////////////////////////////////
void GameMenuState::SetupGui()
{
	nGuiServer* guiServer = nGuiServer::Instance();
	nKernelServer* kernelserver = nKernelServer::Instance();

	kernelserver->PushCwd((nRoot*)nGuiServer::Instance()->GetRootWindowPointer());

	// Canvas
	this->guiCanvas = (nGuiCanvas*) nKernelServer::Instance()->New("nguicanvas", "MainBG");
	guiServer->GetSkin()->AddBrush("MainBG", "../urbanextreme/main", vector2(0.0f, 0.0f), vector2(1024.0f,768.0f), vector4(0.0f, 0.0f, 0.0f, 0.0f));
	this->guiCanvas->SetDefaultBrush("MainBG");
	this->guiCanvas->SetRect(rectangle(vector2(0.0f, 0.0f), vector2(1.0f, 1.0f)));

 	// GameStart
 	this->guiBtGameStart = (nGuiButton*) nKernelServer::Instance()->New("nguibutton", "Obj_BTGameStart");
 	guiServer->GetSkin()->AddBrush("Brush_GameStart", "../urbanextreme/main1",
 		vector2(0.0f, 0.0f), vector2(1024.0f, 32.0f), vector4(0.0f, 0.0f, 0.0f, 0.0f));
 	this->guiBtGameStart->SetHighlightBrush("Brush_GameStart");
	this->guiBtGameStart->SetRect(rectangle(vector2(0.0f, 219.0f/768.0f), vector2(1.0f,251.0f/768.0f)));
	this->guiBtGameStart->SetEventHandler(this);
	guiServer->RegisterEventListener(this->guiBtGameStart);
	this->guiBtGameStart->Hide();
 
 	// Exit
 	this->guiBtExit = (nGuiButton*) nKernelServer::Instance()->New("nguibutton", "Obj_BTExit");
 	guiServer->GetSkin()->AddBrush("Brush_Exit", "../urbanextreme/main3",
 		vector2(0.0f, 0.0f), vector2(1024.0f, 32.0f), vector4(0.0f, 0.0f, 0.0f, 0.0f));
 	this->guiBtExit->SetHighlightBrush("Brush_Exit");
	this->guiBtExit->SetRect(rectangle(vector2(0.0f, 283.0f/768.0f), vector2(1.0f,315.0f/768.0f)));
	this->guiBtExit->SetEventHandler(this);
	guiServer->RegisterEventListener(this->guiBtExit);
	this->guiBtExit->Hide();

	// Credit
	this->guiCredit = (nGuiCanvas*) nKernelServer::Instance()->New("nguicanvas", "MainCredit");
	guiServer->GetSkin()->AddBrush("MainCredit", "../urbanextreme/credit", vector2(0.0f, 0.0f), vector2(1024.0f,768.0f), vector4(0.0f, 0.0f, 0.0f, 0.0f));
	this->guiCredit->SetDefaultBrush("MainCredit");
	this->guiCredit->SetRect(rectangle(vector2(0.0f, 0.0f), vector2(1.0f, 1.0f))); 
	this->guiCredit->Hide();


 	guiServer->GetRootWindowPointer()->OnObtainFocus();
	kernelserver->PopCwd();	
}

//////////////////////////////////////////////////////////////////////////
// virtual void HandleEvent(const nGuiEvent& e)
//////////////////////////////////////////////////////////////////////////
void GameMenuState::HandleEvent(const nGuiEvent& e)
{
	if( e.GetWidget() == this->guiBtGameStart )
	{
		if( e.GetType() == nGuiEvent::Action )
		{
			ChaseCamDemoApp::Instance()->RequestState("GameState");
		}
	}	
	else if( e.GetWidget() == this->guiBtExit )
	{
		if( e.GetType() == nGuiEvent::Action )
		{
			ChaseCamDemoApp::Instance()->RequestState("GameState");
		}
	}

}