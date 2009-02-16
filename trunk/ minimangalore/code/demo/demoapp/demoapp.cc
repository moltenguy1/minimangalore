//----------------------------------------------------------------------------
//   demoapp.cc
//
//  (C)2006
//----------------------------------------------------------------------------
#include "demoapp/demoapp.h"
#include "demoapp/demogamestate.h"

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
    SetProjectDirectory("home:code/demo");

    if (!Application::App::Open())
    {
        return false;
    }

    //TODO: Add code for opening the application.

    // create edit state and specifies it to application.
    Ptr<DemoGameState> demostate = n_new(DemoGameState);
    demostate->SetName("demogamestate");
    demostate->SetSetupMode(Application::GameStateHandler::EmptyWorld);

    this->AddStateHandler(demostate);
    this->SetState("demogamestate");

    return true;
}

//----------------------------------------------------------------------------
/**
*/
void ChaseCamDemoApp::Close()
{
    //TODO: Add code for closing the application.

    Application::App::Close();

    this->luaScriptServer->Close();
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
    return "Demo";
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
    return "NebulaDevice";
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

    // setup the lua script subsystem
    this->luaScriptServer = Script::LuaServer::Create();
    this->luaScriptServer->Open();
}

//----------------------------------------------------------------------------
// EOF
//----------------------------------------------------------------------------
