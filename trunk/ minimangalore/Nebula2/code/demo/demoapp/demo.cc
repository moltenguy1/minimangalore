//----------------------------------------------------------------------------
//  demo.cc
//
//  (C)2007 Kim Hyoun Woo
//----------------------------------------------------------------------------
#ifdef __WIN32__
#include "tools/nwinmaincmdlineargs.h"
#else
#include "tools/ncmdlineargs.h"
#endif
#include "demoapp/demoapp.h"

//----------------------------------------------------------------------------
/**
*/
#ifdef __WIN32__
int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    nWinMainCmdLineArgs args(lpCmdLine);
#else
int
main(int argc, const char** argv)
{
    nCmdLineArgs args(argc, argv);
#endif

    // create the application.
    ChaseCamDemoApp *demoApp = n_new(ChaseCamDemoApp);

    // Uncomment and add your own project directory if it is necessary.
    //demoApp->SetProjectDirectory(projDir);
    // Uncomment and add your own startup script if it is necessary.
    //demoApp->SetStartupScript("home:<your own startup script>");

    demoApp->SetCmdLineArgs(args);

    if (demoApp->Open())
    {
        demoApp->Run();
    }

    demoApp->Close();
    n_delete(demoApp);

    return 0;
}

