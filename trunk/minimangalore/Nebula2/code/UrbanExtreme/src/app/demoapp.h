//-----------------------------------------------------------------------------
//  demoapp.h
//
//  (C)2007 Kim Hyoun Woo
//-----------------------------------------------------------------------------
#ifndef _DEMOAPP_H
#define _DEMOAPP_H
//-----------------------------------------------------------------------------
/**
    @class DemoApp
    @ingroup DemoStarter

    @brief

*/
#include "application/app.h"


class DemoGameState;

//-----------------------------------------------------------------------------
class ChaseCamDemoApp : public Application::App
{
public:
    ChaseCamDemoApp();
    virtual ~ChaseCamDemoApp();

    // @name nApplication override functions
    // @{
    /// open application
    virtual bool Open();
    /// close the application
    virtual void Close();

    /// run the application
    virtual void Run();

    /// returns the application name (override in subclass)
    virtual nString GetAppName() const;
    /// returns the version string (override in subclass)
    virtual nString GetAppVersion() const;
    /// returns vendor name (override in subclass)
    virtual nString GetVendorName() const;

	virtual void SetupFromDefaults();

    // @}

protected:
    /// setup the Game subsystem
    virtual void SetupGameSubsystem();
	virtual void SetupStateHandlers();

};
//------------------------------------------------------------------------------
#endif // _DemoApp_H

