//--------------------------------------------------------------------
//  luascript.h
//
//  (C) Kim Hyoun Woo
//--------------------------------------------------------------------
#ifndef SCRIPT_LUASCRIPT_H
#define SCRIPT_LUASCRIPT_H
//--------------------------------------------------------------------
/**
    @class Script::LuaScript

*/
#include "message/port.h"
#include "message/msg.h"
#include "game/property.h"

#include "luaserver/nluaserver.h"

namespace Script
{

class LuaScript
{
public:
	enum State
	{
		WaitFrame,
		WaitTime,
		Running,
		NotLoaded,
		Done
	};

	///
    LuaScript();
	///
	virtual ~LuaScript();
    ///
	lua_State* GetContext() const;
	///
	//void CreateThread();
	///
	void Update(Game::Property* property, Message::Msg* msg);
	///
	void RunFile(const nString& filename);
	///
	int RunString(const nString& command);
	///
	bool Run(const char* cmd, nString& result);

	///
	void CallFun(const nString& fnName, int param);

	void AboartWait();

	///
	Game::Property* GetProperty() const;
	///
	Message::Msg* GetMessage() const;

	///
    static nString GenerateStackTrace(lua_State* L);

	static void SetErrorMessage(const nString& err);

private:
	///
	void ResumeScript(float param);
    ///
	bool ExecuteLuaChunk(nString& result, int errfunc);

    void reg_globalfunc(lua_CFunction func, const char* name);

private:
	///
	lua_State* threadState;

	///
	State state;
	/// current time
    float curTime;
	/// time to wake up
    float           waitTimestamp;
	/// number of frames to wait
    int             waitFrame;

	static nString outputStr;

	///
	Game::Property* property;
	///Retrieved message whenever PropertyHandleMessage is called.
	Message::Msg* message;
};
//--------------------------------------------------------------------
inline
lua_State* LuaScript::GetContext() const
{
	return this->threadState;
}
//--------------------------------------------------------------------
inline
Game::Property* 
LuaScript::GetProperty() const
{
	return this->property;
}
//--------------------------------------------------------------------
inline
Message::Msg* 
LuaScript::GetMessage() const
{
    return this->message;
}

} // namespace Script


#endif