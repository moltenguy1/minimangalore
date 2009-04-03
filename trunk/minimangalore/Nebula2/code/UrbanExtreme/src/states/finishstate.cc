#include "finishstate.h"
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

//////////////////////////////////////////////////////////////////////////
// FinishState()
//////////////////////////////////////////////////////////////////////////
FinishState::FinishState()
{
	this->SetName("FinishState");
	this->SetSetupMode(EmptyWorld);
}

//////////////////////////////////////////////////////////////////////////
// ~FinishState()
//////////////////////////////////////////////////////////////////////////
FinishState::~FinishState()
{

}

//////////////////////////////////////////////////////////////////////////
// virtual void virtual void OnAttachToApplication()
//////////////////////////////////////////////////////////////////////////
void FinishState::OnAttachToApplication()
{
	SetupGui();
}

//////////////////////////////////////////////////////////////////////////
// virtual void OnStateEnter(const nString& prevState)
//////////////////////////////////////////////////////////////////////////
void FinishState::OnStateEnter(const nString& prevState)
{
	guibutton->Show();	
	GameStateHandler::OnStateEnter(prevState);
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void FinishState::OnStateLeave(const nString& nextState)
{
	guibutton->Hide();
	GameStateHandler::OnStateLeave(nextState);
}

//////////////////////////////////////////////////////////////////////////
// virtual nString OnFrame()
//////////////////////////////////////////////////////////////////////////
nString FinishState::OnFrame()
{	
	return GameStateHandler::OnFrame();
}

//////////////////////////////////////////////////////////////////////////
// void SetupGui()
//////////////////////////////////////////////////////////////////////////
void FinishState::SetupGui()
{
	nGuiServer* guiServer = nGuiServer::Instance();

	nKernelServer* kernelserver = nKernelServer::Instance();

	kernelserver->PushCwd((nRoot*)nGuiServer::Instance()->GetRootWindowPointer());

	this->guibutton = (nGuiButton*) nKernelServer::Instance()->New("nguibutton", "TestButton");

	guiServer->GetSkin()->AddBrush("Start", "n2toolkit",
		vector2(0.0f, 0.0f), vector2(64.0f, 64.0f), vector4(0.0f, 0.0f, 0.0f, 0.0f));

	guiServer->GetSkin()->AddBrush("Push", "n2logo",
		vector2(0.0f, 0.0f), vector2(64.0f, 64.0f), vector4(0.0f, 0.0f, 0.0f, 0.0f));

 	this->guibutton->SetDefaultBrush("Start");
 	this->guibutton->SetHighlightBrush("Start");
 	this->guibutton->SetPressedBrush("Push");

	rectangle temp(vector2(0.5f, 0.5f), vector2(1.0f, 1.0f));
	this->guibutton->SetRect(temp);
	this->guibutton->SetEventHandler(this);
	guiServer->RegisterEventListener(this->guibutton);
 	guiServer->GetRootWindowPointer()->OnObtainFocus();

	kernelserver->PopCwd();	


/*
		this->guiCanvas = (nGuiCanvas*) nKernelServer::Instance()->New("nguicanvas", "canvasBack");
		guiServer->GetSkin()->AddBrush("MainBG", "../wing/bg/main", vector2(0.0f, 0.0f), vector2(800.0f, 600.0f), vector4(0.0f, 0.0f, 0.0f, 0.0f));
		this->guiCanvas->SetDefaultBrush("MainBG");
		this->guiCanvas->SetRect(rectangle(vector2(0.0f, 0.0f), vector2(1.0f, 1.0f)));
		this->guiCanvas->Hide();*/
	

/*

	this->canvasMain = (nGuiCanvas*) nKernelServer::Instance()->New("nguicanvas", "canvasMain");
	guiServer->GetSkin()->AddBrush("MainBG", "../wing/bg/main", vector2(0.0f, 0.0f), vector2(800.0f, 600.0f), vector4(0.0f, 0.0f, 0.0f, 0.0f));
	this->canvasMain->SetDefaultBrush("MainBG");
	this->canvasMain->SetRect(rectangle(vector2(0.0f, 0.0f), vector2(1.0f, 1.0f)));
	this->canvasMain->Hide();
*/
}

//////////////////////////////////////////////////////////////////////////
// virtual void HandleEvent(const nGuiEvent& e)
//////////////////////////////////////////////////////////////////////////
void FinishState::HandleEvent(const nGuiEvent& e)
{
	if( e.GetWidget() == this->guibutton )
	{
		if( e.GetType() == nGuiEvent::Action )
		{
			ChaseCamDemoApp::Instance()->RequestState("GameState");
		}
	}

}