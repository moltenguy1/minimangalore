#ifndef SCRIPT_ANIMCOMMAND_H
#define SCRIPT_ANIMCOMMAND_H

#include "foundation/refcounted.h"
#include "util/cmdlineargs.h"

namespace Script
{
class AnimCommand : public Command
{
	DeclareRtti;
	DeclareFactory(AnimCommand);

public:
	/// constructor
	AnimCommand();
	/// destructor
	virtual ~AnimCommand();
	///
	static AnimCommand* Instance();

	virtual int Execute(const

private:
	static AnimCommand* Singleton;

};

} // namespace Script