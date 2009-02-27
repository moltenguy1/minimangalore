//--------------------------------------------------------------------
//  luascript.cc
//
//  (C) Kim Hyoun Woo
//--------------------------------------------------------------------
#include "script/luaserver.h"
#include "script/luascript.h"

#include "kernel/nfileserver2.h"
#include "kernel/ntimeserver.h"

namespace Script
{

nString LuaScript::outputStr;

//--------------------------------------------------------------------
//  luacmd_StackDump()
//--------------------------------------------------------------------
int lua_StackDump(lua_State* L)
{
	nString result = Script::LuaScript::GenerateStackTrace(L);
	Script::LuaScript::SetErrorMessage(result.Get());
    lua_settop(L, 0);
    return 0;
}

void LuaScript::reg_globalfunc(lua_CFunction func, const char* name)
{
    lua_pushstring(this->threadState, name);
    lua_pushlightuserdata(this->threadState, this);
    lua_pushcclosure(this->threadState, func, 1);
    lua_settable(this->threadState, LUA_GLOBALSINDEX);
    lua_settop(this->threadState, 0);
}


void LuaScript::SetErrorMessage(const nString& err)
{
	outputStr = err;
}

//--------------------------------------------------------------------
/**
*/
LuaScript::LuaScript() :
    curTime(0.0f)
{
	//((nTclServer *)(Foundation::Server::Instance()->GetScriptServer()))->GetInterp();
	nLuaServer* luaServer = ((nLuaServer*)(LuaServer::Instance()->GetScriptServer()));
	lua_State* ls =luaServer->GetContext();

	// create a thread/state for this object
	this->threadState = lua_newthread(ls);

    // save a pointer to the thread manager object in the global table
    // using the new thread's vm pointer as a key
    lua_pushlightuserdata(ls, this->threadState);
    lua_pushlightuserdata(ls, this);
    lua_settable(ls, LUA_GLOBALSINDEX );

    reg_globalfunc(lua_StackDump, "_LUASERVER_STACKDUMP");
}

//--------------------------------------------------------------------
/**
*/
LuaScript::~LuaScript()
{
}

//--------------------------------------------------------------------
/**
*/
void LuaScript::RunFile(const nString& filename)
{
	const nString filepath = nFileServer2::Instance()->ManglePath(filename);

    int status = luaL_loadfile(this->threadState, filepath.Get());
    if (status == 0)
    {
        ResumeScript(0.0f);
    }
    else 
    {
        //error
		nString result;
		nLuaServer::StackToString(this->threadState, lua_gettop(this->threadState) - 1, result);
    }
}

//--------------------------------------------------------------------
/**
*/
int LuaScript::RunString(const nString& command)
{
    
    int status = luaL_loadbuffer(this->threadState, command.Get(), command.Length(), "Console");
    if (status == 0)
        status = lua_pcall(this->threadState, lua_gettop(threadState)-1, 0, 0);

    if (status)
	{
		// error!
	}

    return(0);
}

//--------------------------------------------------------------------
/**
*/
bool LuaScript::Run(const char* cmd, nString& result)
{
    //nLuaServer* luaServer = ((nLuaServer*)(LuaServer::Instance()->GetScriptServer()));

    // push the error handler on stack
    lua_pushstring(this->threadState, "_LUASERVER_STACKDUMP");
    lua_gettable(this->threadState, LUA_GLOBALSINDEX);
    n_assert2(lua_isfunction(this->threadState, -1), "Error handler not registered!");
    int errfunc = lua_gettop(this->threadState);
    // load chunk
    int status = luaL_loadbuffer(this->threadState, cmd, strlen(cmd), cmd);
    if (0 == status) // parse OK?
    {
        return this->ExecuteLuaChunk(result, errfunc);
    }
    // pop error message from the stack
    result.Clear();
	nLuaServer::StackToString(this->threadState, lua_gettop(this->threadState) - 1, result);

    return false;
}

//--------------------------------------------------------------------
/**
    @brief Executes the chunk at the top of the Lua stack.
*/
bool LuaScript::ExecuteLuaChunk(nString& result, int errfunc)
{
    n_assert2(errfunc > 0, "Error function stack index must be absolute!");
    
    // call chunk main
    int status = lua_pcall(this->threadState,
                           0 /* no args */,
                           LUA_MULTRET,
                           errfunc /* stack index of error handler */);
    if (0 != status) // error occurred
    {
        result = outputStr; // contains the error info
        n_message(result.Get());
        lua_settop(this->threadState, 0); // clear stack
    }
    else
    {
        result.Clear();
		nLuaServer::StackToString(this->threadState, 0, result);
    }
    
    return (0 == status);
}

//--------------------------------------------------------------------
/**
    UPDATE
    The purpose of update is basically to check if any suspended scripts need
    to be turned back on again!

    Returns the next script in the list.  This assists in linked list traversal
    where this script may delete it's next sibling.

*/
void LuaScript::Update(Game::Property* property, Message::Msg* msg)
{
	n_assert(0 != property);
	n_assert(0 != msg);

    this->curTime += nTimeServer::Instance()->GetTime();;

	this->message = msg;
	this->property = property;

    switch(state)
    {
    case WaitTime:
        if (this->curTime >= this->waitTimestamp)
            ResumeScript(0.0f);
        break;

    case WaitFrame:
        this->waitFrame--;
        if (this->waitFrame <= 0)
            ResumeScript(0.0f);
        break;

    case NotLoaded:
        break;

//  case LSS_DONE:
    default :
        break;
    }
}

//--------------------------------------------------------------------
/**
*/
void LuaScript::CallFun(const nString& fnName, int param)
{
}

//--------------------------------------------------------------------
/**
*/
void LuaScript::AboartWait()
{
	ResumeScript(1.0f);
}

//--------------------------------------------------------------------
/**
*/
void LuaScript::ResumeScript(float param)
{
    int         status;

    // we're about to run/resume the thread, so set the global
    state  = Running;

    // param is treated as a return value from the function that yielded
    lua_pushnumber(this->threadState, param);

    status = lua_resume(this->threadState, 1);

    if (status)
	{
		// error
	}
}

//------------------------------------------------------------------------------
/**
    @brief Generates a stack trace.
    @return A pointer to the string containing the stack trace.
    
    @warning The pointer points to an internal buffer that is likely to change,
             therefore you should copy the string before any further Lua 
             server methods are called.
*/
nString LuaScript::GenerateStackTrace(lua_State* L)
{
    n_assert(L);
    n_assert2(1 == lua_gettop(L), "Only error message should be on stack!");

	nString result;
    result.Set("nLuaServer encountered a problem...\n");
    result.Append(lua_tostring(L, -1));
    result.Append("\n\n-- Stack Trace --\n");
    
    lua_Debug debugInfo;
    int level = 0;
    char buffer[1024];
    buffer[0] = 0;
    const char* namewhat = 0;
    const char* funcname = 0;
    while (lua_getstack(L, level, &debugInfo))
    {
        if (lua_getinfo(L, "nSl", &debugInfo))
        {
            if (0 == debugInfo.namewhat[0])
                namewhat = "???";
            else
                namewhat = debugInfo.namewhat;
                
            if (0 == debugInfo.name)
                funcname = "???";
            else
                funcname = debugInfo.name;
                    
            snprintf(buffer, sizeof(buffer), 
                     "%s - #%d: %s (%s/%s)\n", 
                     debugInfo.short_src,
                     debugInfo.currentline,
                     funcname,
                     namewhat,
                     debugInfo.what);
            buffer[sizeof(buffer)-1] = 0; // null terminate in case snprintf doesn't!
            result.Append(buffer);
        }
        else
            result.Append("Failed to generate stack trace.\n");
        level++;
    }
    return result;
}


}// namespace Script