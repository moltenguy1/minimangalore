
#ifndef N_AMESTATE_H
#define N_AMESTATE_H
//----------------------------------------------------------------------------
/**
    @class GameState
    @ingroup TruckingWay
    @brief a brief description of the class

*/
//----------------------------------------------------------------------------
#include "application/app.h"
#include "application/gamestatehandler.h"
#include "message/port.h"
#include "properties/chasecameraproperty.h"

//------------------------------------------------------------------------------
namespace Properties
{
class UePlayerCameraProperty : public ChaseCameraProperty
{
    DeclareRtti;
    DeclareFactory(UePlayerCameraProperty);
public:
	///
	UePlayerCameraProperty();
	///
	virtual ~UePlayerCameraProperty();
   
protected:
	virtual void SetupDefaultAttributes();
    virtual void UpdateCamera();

    matrix44 poseCamera[3];
	bool onlyStandPose;
};

RegisterFactory(UePlayerCameraProperty);
}

//----------------------------------------------------------------------------
#endif // N_AMESTATE_H

