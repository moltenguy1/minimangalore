#include "attributes/ueattributes.h"

namespace Attr
{  
	DefineBool(accelerate);
	DefineBool(dobreak);
	DefineBool(left);
	DefineBool(right);
	DefineInt(pose);
	DefineBool(poseUp);
	DefineBool(poseDn);
	DefineFloat(standRotateAngle);
	DefineFloat(crounchRotateAngle);
	DefineFloat(proneRotateAngle);
	DefineFloat(stopRotateAngle);
	DefineVector3(position);
	DefineBool(onGround);
	DefineMatrix44(lastTM);	
}