#ifndef SCRIPT_LUASERVER_H
#define SCRIPT_LUASERVER_H

//------------------------------------------------------------------------------
/**
    @class Script::LuaServer


	(C)2007 Kim Hyoun Woo
*/
#include "foundation/refcounted.h"
#include "luaserver/nluaserver.h"

class nLuaServer;

namespace Script
{
class LuaScript;

class Command;

class LuaServer : public Foundation::RefCounted
{
	DeclareRtti;
	DeclareFactory(LuaServer);

public:
	/// Constructor
	LuaServer();
	/// Destructor
	virtual ~LuaServer();
	/// Get instance pointer
	static LuaServer* Instance();
	/// Open the script server
	bool Open();
	/// Close the script server
	void Close();

    LuaScript* CreateScript();

	///
	bool IsOpen();

	///
	nScriptServer* GetScriptServer() const;

	///
	void RegisterCommand(Command *cmd);
	///
	void DeRegisterCommand(const nString& cmdName);
	///
	bool CommandExists(const nString& cmdName) const;



private:

	static LuaServer* Singleton;

	nRef<nScriptServer>     luaScriptServer;

	bool isOpen;

	nArray<LuaScript*> scriptArray;
	//nArray<Ptr<Command>> commandArray;
};

RegisterFactory(LuaServer);

//------------------------------------------------------------------------------
/**
*/
inline
LuaServer*
LuaServer::Instance()
{
	n_assert(0 != Singleton);
	return Singleton;
}
//------------------------------------------------------------------------------
/**
*/
inline
bool LuaServer::IsOpen()
{
	return this->isOpen;
}
//------------------------------------------------------------------------------
/**
*/
inline
nScriptServer* 
LuaServer::GetScriptServer() const
{
	return this->luaScriptServer;
}

}// namespace Script
//------------------------------------------------------------------------------
#endif