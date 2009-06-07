#ifndef mTest_mTestAPP_H
#define mTest_mTestAPP_H
//------------------------------------------------------------------------------
/**
    @class mTest::mTestApp

    A generic Mangalore mTest application, similar to the Nebula2 mTest,
    but which also supports Physics.
    
    (C) 2005 RadonLabs GmbH
*/
#include "application/app.h"
#include "gui/nguitextlabel.h"

//------------------------------------------------------------------------------
namespace mTutorial
{
class mTestApp : public Application::App
{
public:
    /// constructor
    mTestApp();
    /// destructor
    virtual ~mTestApp();
	/// get instance pointer
	static mTestApp* Instance();
    /// returns the application name (override in subclass)
    
    /// open application
    virtual bool Open();
    /// close the application
    virtual void Close();

    /// returns the application name (override in subclass)
    virtual nString GetAppName() const;
    /// returns the version string (override in subclass)
    virtual nString GetAppVersion() const;
    /// returns vendor name (override in subclass)
    virtual nString GetVendorName() const;
  
protected:
	/// setup the app's input mapping (called in SetupSubsystems())
    virtual void SetupDefaultInputMapping();

private:
    /// setup application defaults, override in subclass
    virtual void SetupFromDefaults();
	/// setup application state handlers
    virtual void SetupStateHandlers();

    /// called per frame by Run(), override in subclass as needed
    virtual void OnFrame();
 
     /// create viewed entity (not mandatory but create a code cleaner...)
    void CreateViewedEntity();
    /// set lights in scene
    void SetupLightsInScene();
    /// set camera in scene
    void SetupCameraAndInput();
   	
    static mTestApp* Singleton;
 };

//------------------------------------------------------------------------------
/**
*/
inline mTestApp* mTestApp::Instance()
{
    n_assert(0 != Singleton);
    return Singleton;
}

}; // end namspace
//------------------------------------------------------------------------------
#endif

    