//-----------------------------------------------------------------------------
//  animscript.cc
//
//  (C)2007  Kim Hyoun Woo
//-----------------------------------------------------------------------------
#include "luaserver/nluaserver.h"
#include "msg/gfxsetanimation.h"
#include "msg/playerstatus.h"
#include "script/luascript.h"

#include "game/entity.h"

namespace Script
{
static int luaIsMoving(lua_State* ls);
static int luaGetDirection(lua_State* ls);
static int luaRequestAnimation(lua_State* ls);
static int luaGetDir(lua_State* ls);
static int luaGetSpeed(lua_State* ls);
static int luaAction(lua_State* ls);
static LuaScript* GetScriptObject(lua_State* l);

//-----------------------------------------------------------------------------
/**
*/
static const luaL_reg animfunc[] =
{
	{"RequestAnimation", luaRequestAnimation},
	{"GetDirection", luaGetDir},
	{"GetSpeed", luaGetSpeed},
	{"Action", luaAction},
	{"GetDirection", luaGetDirection},
	{"IsMoving", luaIsMoving},
	//{"", },
	//{"", },
	//{"", },
	//{"", },
	//{"", },
	//{"", },
	//{"", },
	//{"", },
	//{"", },
	//{"", },
	{NULL, NULL}
};

//-----------------------------------------------------------------------------
/**
    Register animation script interface functions.
*/
void RegisterAnimFuncntions(lua_State* ls)
{
    // register our library of routines under the table name Script
    luaL_openlib(ls, "Anim", animfunc, 0);
}

//-----------------------------------------------------------------------------
/**
*/
static
int luaIsMoving(lua_State* ls)
{
    LuaScript*  s;
    s = GetScriptObject(ls);

	Game::Property* prop = s->GetProperty();
	if (prop->GetEntity()->GetBool(Attr::Moving))
		return 1;

	return 0;
}

//-----------------------------------------------------------------------------
/**
    0 - idle
    1 - move left
	2 - move right
	3 - move forward
	4 - move back
*/
static
int luaGetDirection(lua_State* ls)
{
    LuaScript*  s;
    s = GetScriptObject(ls);

	Game::Property* prop = s->GetProperty();
	const vector3& v = prop->GetEntity()->GetVector3(Attr::VelocityVector);

	int ret = 0;

	if (v.x < 0.0f)
		ret = 1;// move left
	else
	if (v.x > 0.0f)
		ret = 2;// move right
	else
		ret = 0;// idle

	if (v.z < 0.0f)
		ret = 3;// move forward
	else
	if (v.z > 0.0f)
		ret = 4;// move back
	else
		ret = 0;// move idle

    return ret;
}

//-----------------------------------------------------------------------------
/**
*/
static
int luaRequestAnimation(lua_State* ls)
{
    LuaScript*  s;
    s = GetScriptObject(ls);

	//TODO: retrieve 3 parameters from script side!!!

    nString baseAnimation;
	nString overlayAnimation;
	float baseAnimTimeOffset = 0.0f;

    Ptr<Message::GfxSetAnimation> setAnimation = Message::GfxSetAnimation::Create();
    setAnimation->SetBaseAnimation(baseAnimation);
    setAnimation->SetOverlayAnimation(overlayAnimation);
    setAnimation->SetBaseAnimTimeOffset(baseAnimTimeOffset);
    setAnimation->SetFadeInTime(0.2);

	Game::Property* prop = s->GetProperty();
    prop->GetEntity()->SendSync(setAnimation);

	return 0;
}

//-----------------------------------------------------------------------------
/**
*/
static
int luaGetDir(lua_State* ls)
{
    LuaScript*  s;
    s = GetScriptObject(ls);


	Message::Msg* msg = s->GetMessage();
	Message::PlayerStatus* playerStatus = (Message::PlayerStatus*) msg;

	if (playerStatus->fb > 0 || playerStatus->lr > 0)
		return 0;

	return 1;
}

//-----------------------------------------------------------------------------
/**
*/
static
int luaGetSpeed(lua_State* ls)
{
	return 0;
}

//-----------------------------------------------------------------------------
/**
*/
static
int luaAction(lua_State* ls)
{
	return 0;
}

//-----------------------------------------------------------------------------
/**
*/
static 
LuaScript* GetScriptObject(lua_State* l)
{
    lua_pushlightuserdata(l, l);
    lua_gettable(l, LUA_GLOBALSINDEX );
    return((LuaScript*)lua_touserdata(l, -1));
}

} // namespace Script