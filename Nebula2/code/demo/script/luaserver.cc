//------------------------------------------------------------------------------
//
//
//
//------------------------------------------------------------------------------

#include "script/luascript.h"
#include "script/luaserver.h"
#include "script/command.h"
#include "util/cmdlineargs.h"

nNebulaUsePackage(nlua);

namespace Script
{
ImplementRtti(Script::LuaServer, Foundation::RefCounted);
ImplementFactory(Script::LuaServer);

LuaServer* LuaServer::Singleton = 0;

//------------------------------------------------------------------------------
/**
*/
LuaServer::LuaServer() :
    isOpen(false)
    //commandArray(128, 128)
{
    n_assert(0 == Singleton);
    Singleton = this;
}

//------------------------------------------------------------------------------
/**
*/
LuaServer::~LuaServer()
{
    n_assert(0 != Singleton);
    Singleton = 0;
}

//------------------------------------------------------------------------------
/**
*/
bool
LuaServer::Open()
{
	n_assert(!this->IsOpen());

    nKernelServer::Instance()->AddPackage(nlua);

	// create nluaserver object instance
	this->luaScriptServer = (nScriptServer*)nKernelServer::Instance()->New("nluaserver", "/sys/servers/lua");

	this->isOpen = true;

	

	return true;
}

//------------------------------------------------------------------------------
/**
*/
LuaScript*
LuaServer::CreateScript()
{
	LuaScript* s = n_new(LuaScript);

	if (s)
	{
		scriptArray.Append(s);
		return s;
	}

	return 0;	
}

//------------------------------------------------------------------------------
/**
*/
void
LuaServer::Close()
{
	n_assert(this->IsOpen());

	this->isOpen = false;

	LuaScript* s;
	for (int i=0;i<scriptArray.Size(); i++)
	{
	    s = scriptArray[i];
		n_delete (s);
	}
}

//------------------------------------------------------------------------------
/**
*/
//bool
//LuaServer::CommandExists(const nString& cmdName) const
//{
//	for (int i = 0; i < this->commandArray.Size(); i++)
//	{
//		if (this->commandArray[i]->GetClassName() == cmdName)
//		{
//			return true;
//		}
//	}
//	return false;
//}

//------------------------------------------------------------------------------
/**
*/
//void
//LuaServer::RegisterCommand(Command *cmd)
//{
//	n_assert(this->IsOpen());
//	n_assert(cmd);
//	n_assert(!this->CommandExists(cmd->GetClassName()));
//
//	// register command to lua
//
//	this->commandArray.Append(cmd);
//}

//------------------------------------------------------------------------------
/**
*/
//void
//Server::DeRegisterCommand(const nString& name)
//{
//	n_assert(this->IsOpen());
//	n_assert(this->commandExists(name));
//
//
//	for (int i = 0; i < this->commandArray.Size(); i++)
//	{
//		if (this->commandArray[i]->GetClassName() == name)
//		{
//			// deregister command
//
//
//			this->commandArray.Erase(i);
//			break;
//		}
//	}
//}

} // namespace Script
