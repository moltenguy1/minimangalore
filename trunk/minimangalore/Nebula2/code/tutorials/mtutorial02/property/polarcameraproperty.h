#ifndef PROPERTIES_PolarCAMERAPROPERTY_H
#define PROPERTIES_PolarCAMERAPROPERTY_H
//------------------------------------------------------------------------------
/**
    @class Properties::PolarCameraProperty

    A static camera that use the polar system
    // THIS IS A SPECIFIC VERSION FOR TUTORIAL02
    
    (C) je.a.le@wanadoo.fr
*/
#include "game/property.h"
#include "game/entity.h"
#include "properties/cameraproperty.h"

//------------------------------------------------------------------------------
namespace Attr
{
    // Polar camera specific attributes. It's more flexible than class variables
	DeclareVector3(PolarCameraLookAt);	// where to look at
    DeclareFloat(PolarCameraPhi);			// phi angle
	DeclareFloat(PolarCameraTheta);		// theta angle
	DeclareFloat(PolarCameraDistance)		// distance fron look at
};

//------------------------------------------------------------------------------
namespace Properties
{
//using namespace Attr;

class PolarCameraProperty : public CameraProperty
{
    DeclareRtti;
	DeclareFactory(PolarCameraProperty);

public:
    /// constructor
    PolarCameraProperty();
    /// destructor
    virtual ~PolarCameraProperty();
    ///// called before rendering happens
    virtual void OnRender();
    /// setup default entity attributes
    virtual void SetupDefaultAttributes();
 
private:
  	// transformation matrix
	matrix44	m;
};

RegisterFactory(PolarCameraProperty);

}; // namespace Properties
//------------------------------------------------------------------------------
#endif    
