//------------------------------------------------------------------------------
//  viewer.cc
//
//  A generic Mangalore viewer.
//  with physics enabled.
//
//  (C) 2005 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "mtutorial05/mtestapp.h"
#include "tools/nwinmaincmdlineargs.h"
#include <windows.h>

//------------------------------------------------------------------------------
int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    nWinMainCmdLineArgs args(lpCmdLine);

    // initialize the Mangalore application
	mTutorial::mTestApp app;
	
    if (app.Open())
    {
		app.Run();
		app.Close();
    }
    return 0;
}
