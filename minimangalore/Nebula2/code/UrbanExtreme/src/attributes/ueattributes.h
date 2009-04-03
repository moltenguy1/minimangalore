#ifndef ATTRIBUTES_UEATTRIBUTES_H
#define ATTRIBUTES_UEATTRIBUTES_H

#include "attr/attributeid.h"

//------------------------------------------------------------------------------
namespace Attr
{

	DeclareBool(accelerate);
	DeclareBool(dobreak);
	DeclareBool(left);
	DeclareBool(right);
	DeclareInt(pose);
	DeclareBool(poseUp);
	DeclareBool(poseDn);
	DeclareVector3(position);
	DeclareBool(onGround);
	DeclareMatrix44(lastTM);
	
} // namespace Attr
//------------------------------------------------------------------------------
#endif
