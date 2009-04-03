//----------------------------------------------------------------------------
//   demoapp.cc
//
//  (C)2006
//----------------------------------------------------------------------------
#include "app/demoapp.h"
#include "states/demogamestate.h"
#include "states/startmenustate.h"
#include "scene/nsceneserver.h"
#include "kernel/nfileserver2.h"

//----------------------------------------------------------------------------
/**
*/
ChaseCamDemoApp::ChaseCamDemoApp()
{
}

//----------------------------------------------------------------------------
/**
*/
ChaseCamDemoApp::~ChaseCamDemoApp()
{
}

//----------------------------------------------------------------------------
/**
*/
bool ChaseCamDemoApp::Open()
{
	nString path_home;
	nString path_proj;
	nString path_curr;



	this->SetProjectDirectory("home:");


    if (!Application::App::Open())
    {
		nSceneServer::Instance()->SetOcclusionQuery(true);

        return false;
    }

    //TODO: Add code for opening the application.
	path_home= nFileServer2::Instance()->ManglePath("home:");
	path_proj= nFileServer2::Instance()->ManglePath("proj:");

	nString path_export=	nFileServer2::Instance()->ManglePath("export:");
	nString path_gfx=		nFileServer2::Instance()->ManglePath("gfxlib:");
	nString path_textures=	nFileServer2::Instance()->ManglePath("textures:");



	OutputDebugString(path_home.Get());
	OutputDebugString(path_proj.Get());
	OutputDebugString(path_textures.Get());
    

    return true;
}

//----------------------------------------------------------------------------
/**
*/
void ChaseCamDemoApp::Close()
{
    //TODO: Add code for closing the application.

	
    Application::App::Close();
}

//----------------------------------------------------------------------------
/**
*/
void ChaseCamDemoApp::Run()
{
    //TODO: Add code for running the application.

    Application::App::Run();
}

//------------------------------------------------------------------------------
/**
    Override this method in subclasses to return a different application name.
*/
nString
ChaseCamDemoApp::GetAppName() const
{
    return "UrbanExtreme";
}

//------------------------------------------------------------------------------
/**
    Override this method in subclasses to return a different version string.
*/
nString
ChaseCamDemoApp::GetAppVersion() const
{
    return "0.1.0";
}

//------------------------------------------------------------------------------
/**
    Get the application vendor. This is usually the publishers company name.
*/
nString
ChaseCamDemoApp::GetVendorName() const
{
    return "";
}

//------------------------------------------------------------------------------
/**
    Add OperateManager to game subsystem
*/
void
ChaseCamDemoApp::SetupGameSubsystem()
{
    App::SetupGameSubsystem();

    //Ptr<Managers::OperateManager> operateMgr = Managers::OperateManager::Create();
    //this->gameServer->AttachManager(operateMgr);

  
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void ChaseCamDemoApp::SetupStateHandlers()
{
	Ptr<Application::GameStateHandler> mStateHandler;

	// create edit state and specifies it to application.
	mStateHandler = n_new(DemoGameState);
	this->AddStateHandler(mStateHandler);

	mStateHandler = n_new(GameMenuState);    
	this->AddStateHandler(mStateHandler);

	//this->SetState("GameState");
	this->SetState("StartMenuState");
}

void ChaseCamDemoApp::SetupFromDefaults()
{

		// setup display mode
		nDisplayMode2 mode;
		nString windowTitle = this->GetVendorName() + " - " + this->GetAppName() + " - " + this->GetAppVersion();
		mode.SetWindowTitle(windowTitle.Get());
		mode.SetXPos(0);
		mode.SetYPos(0);
		mode.SetWidth(1024);
		mode.SetHeight(768);
	
#ifdef _DEBUG
		mode.SetType(nDisplayMode2::Windowed);
#else		
		mode.SetType(nDisplayMode2::Fullscreen);
#endif

		mode.SetVerticalSync(false);
		mode.SetDialogBoxMode(true);
		mode.SetIcon("Icon");
		this->SetDisplayMode(mode);
	
}
//----------------------------------------------------------------------------
// EOF
//----------------------------------------------------------------------------

