#include "CLuaInterface.h"

int ILuaBase::Top()
{
	lua_gettop(state);
}

void ILuaBase::Push(int iStackPos)
{
	lua_pushvalue(state, iStackPos);
}

void ILuaBase::Pop(int iAmt)
{
	lua_settop(state, iAmt);
}

void ILuaBase::GetTable(int iStackPos)
{
	lua_gettable(state, iStackPos);
}

void ILuaBase::GetField(int iStackPos, const char* strName)
{
	lua_getfield(state, iStackPos, strName);
}

void ILuaBase::SetField(int iStackPos, const char* strName)
{
	lua_setfield(state, iStackPos, strName);
}

void ILuaBase::CreateTable()
{
	lua_createtable(state, 0, 0);
}

void ILuaBase::SetTable(int iStackPos)
{
	lua_settable(state, iStackPos);
}

void ILuaBase::SetMetaTable(int iStackPos)
{
	lua_setmetatable(state, iStackPos);
}

bool ILuaBase::GetMetaTable(int iStackPos)
{
	return lua_getmetatable(state, iStackPos);
}

void ILuaBase::Call(int iArgs, int iResults)
{
	lua_call(state, iArgs, iResults);
}

int ILuaBase::PCall(int iArgs, int iResults, int iErrorFunc)
{
	return lua_pcall(state, iArgs, iResults, iErrorFunc);
}

int ILuaBase::Equal(int iA, int iB)
{
	return lua_equal(state, iA, iB);
}

int ILuaBase::RawEqual(int iA, int iB)
{
	return lua_rawequal(state, iA, iB);
}

void ILuaBase::Insert(int iStackPos)
{
	lua_insert(state, iStackPos);
}

void ILuaBase::Remove(int iStackPos)
{
	lua_remove(state, iStackPos);
}

int ILuaBase::Next(int iStackPos)
{
	return lua_next(state, iStackPos);
}

void* ILuaBase::NewUserdata(unsigned int iSize)
{
	return lua_newuserdata(state, iSize);
}

void ILuaBase::ThrowError(const char* strError)
{
	luaL_error(state, "%s", strError);
}

void ILuaBase::CheckType(int iStackPos, int iType)
{
	int actualType = lua_type(state, iStackPos);
	if (actualType != iType) {
		const char* expectedType = lua_typename(state, iType);
		const char* actualTypeName = lua_typename(state, actualType);
		const char* errorMessage = lua_pushfstring(state, "Expected type %s at stack position %d, but got type %s.", expectedType, iStackPos, actualTypeName);
		lua_error(state);
	}
}

void ILuaBase::ArgError(int iArgNum, const char* strMessage)
{
	luaL_argerror(state, iArgNum, strMessage);
}

void ILuaBase::RawGet(int iStackPos)
{
	lua_rawget(state, iStackPos);
}

void ILuaBase::RawSet(int iStackPos)
{
	lua_rawset(state, iStackPos);
}

const char* ILuaBase::GetString(int iStackPos, unsigned int* iOutLen)
{
	return lua_tolstring(state, iStackPos, iOutLen);
}

double ILuaBase::GetNumber(int iStackPos)
{
	return lua_tonumber(state, iStackPos);
}

bool ILuaBase::GetBool(int iStackPos)
{
	return lua_toboolean(state, iStackPos);
}

CFunc ILuaBase::GetCFunction(int iStackPos)
{
	return lua_tocfunction(state, iStackPos);
}

void* ILuaBase::GetUserdata(int iStackPos)
{
	return lua_touserdata(state, iStackPos);
}

void ILuaBase::PushNil()
{
	lua_pushnil(state);
}

void ILuaBase::PushString(const char* val, unsigned int iLen)
{
	if (iLen > 0) {
		lua_pushlstring(state, val, iLen);
	} else {
		lua_pushstring(state, val);
	}
}

void ILuaBase::PushNumber(double val)
{
	lua_pushnumber(state, val);
}

void ILuaBase::PushBool(bool val)
{
	lua_pushboolean(state, val);
}

void ILuaBase::PushCFunction(CFunc val)
{
	//lua_pushcfunction(state, val);
	lua_pushcclosure(state, val, 0);
}

void ILuaBase::PushCClosure(CFunc val, int iVars)
{
	lua_pushcclosure(state, val, iVars);
}

void ILuaBase::PushUserdata(void* val)
{
	lua_pushlightuserdata(state, val);
}

int ILuaBase::ReferenceCreate()
{
	return luaL_ref(state, LUA_REGISTRYINDEX);
}

void ILuaBase::ReferenceFree(int i)
{
	luaL_unref(state, LUA_REGISTRYINDEX, i);
}

void ILuaBase::ReferencePush(int i)
{
	lua_rawgeti(state, LUA_REGISTRYINDEX, i);
}

void ILuaBase::PushSpecial(int iType)
{
	switch (iType) {
		case SPECIAL_GLOB:
			lua_pushvalue(state, LUA_GLOBALSINDEX);
			break;
		case SPECIAL_ENV:
			lua_pushvalue(state, LUA_ENVIRONINDEX);
			break;
		case SPECIAL_REG:
			lua_pushvalue(state, LUA_REGISTRYINDEX);
			break;
		default:
			lua_pushnil(state);
			break;
	}
}

bool ILuaBase::IsType(int iStackPos, int iType)
{
	int actualType = lua_type(state, iStackPos);
	bool isType = false;

	if (actualType == iType) {
		return true;
	}

	return false;
}

int ILuaBase::GetType(int iStackPos)
{
	return lua_type(state, iStackPos);
}

const char* ILuaBase::GetTypeName(int iType)
{
	if (iType < 0) {
		return "none";
	} else {
		const char* pName = Type::Name[iType];
		if (!pName) {
			return "unknown";
		} else {
			return pName;
		}
	}
}

void luaL_newmetatable_type(lua_State* L, const char* strName, int iType)
{
	luaL_newmetatable(L, strName);
	g_iTypeNum++;

	lua_pushstring(L, "MetaName");
	lua_pushstring(L, strName);
	lua_settable(L, -3);

	//lua_pushstring(L, "MetaID");
	//lua_pushinteger(L, iType);
	//lua_settable(L, -3);
}

void ILuaBase::CreateMetaTableType(const char* strName, int iType)
{
	luaL_newmetatable_type(state, strName, iType);

	if (iType > 254) {
		return;
	}
}

const char* ILuaBase::CheckString(int iStackPos)
{
	return luaL_checklstring(state, iStackPos, nullptr);
}

double ILuaBase::CheckNumber(int iStackPos)
{
	return luaL_checknumber(state, iStackPos);
}

int ILuaBase::ObjLen(int iStackPos)
{
	return lua_objlen(state, iStackPos);
}

// ------------------------------------------
// ToDo: Recheck every function below!
// ------------------------------------------

QAngle DefaultAngle = QAngle(0, 0, 0);
const QAngle& ILuaBase::GetAngle(int iStackPos)
{
	if (IsType(iStackPos, Type::Angle)) {
		//luaL_checktype(state, iStackPos, LUA_TTABLE);

		lua_rawgeti(state, iStackPos, 1);
		float pitch = static_cast<float>(luaL_optnumber(state, -1, 0));
		lua_pop(state, 1);

		lua_rawgeti(state, iStackPos, 2);
		float yaw = static_cast<float>(luaL_optnumber(state, -1, 0));
		lua_pop(state, 1);

		lua_rawgeti(state, iStackPos, 3);
		float roll = static_cast<float>(luaL_optnumber(state, -1, 0));
		lua_pop(state, 1);

		return QAngleByValue(pitch, yaw, roll);
	} else {
		return DefaultAngle;
	}
}

Vector DefaultVec = Vector(0, 0, 0);
const Vector& ILuaBase::GetVector(int iStackPos)
{
	if (IsType(iStackPos, Type::Vector)) {
		//luaL_checktype(state, iStackPos, LUA_TTABLE);

		lua_rawgeti(state, iStackPos, 1);
		float pitch = static_cast<float>(luaL_optnumber(state, -1, 0));
		lua_pop(state, 1);

		lua_rawgeti(state, iStackPos, 2);
		float yaw = static_cast<float>(luaL_optnumber(state, -1, 0));
		lua_pop(state, 1);

		lua_rawgeti(state, iStackPos, 3);
		float roll = static_cast<float>(luaL_optnumber(state, -1, 0));
		lua_pop(state, 1);

		return VectorByValue(pitch, yaw, roll);
	} else {
		return DefaultVec;
	}
}

void ILuaBase::PushAngle(const QAngle& val)
{
	lua_newtable(state);

	lua_pushnumber(state, val.x);
	lua_setfield(state, -2, "1");

	lua_pushnumber(state, val.y);
	lua_setfield(state, -2, "2");

	lua_pushnumber(state, val.z);
	lua_setfield(state, -2, "3");

	//lua_setmetatable(state, -2);
}

void ILuaBase::PushVector(const Vector& val)
{
	lua_newtable(state);

	lua_pushnumber(state, val.x);
	lua_setfield(state, -2, "1");

	lua_pushnumber(state, val.y);
	lua_setfield(state, -2, "2");

	lua_pushnumber(state, val.z);
	lua_setfield(state, -2, "3");

	//lua_setmetatable(state, -2);
}

void ILuaBase::SetState(lua_State* L)
{
	state = L;
}

int ILuaBase::CreateMetaTable(const char* strName)
{
	luaL_newmetatable_type(state, strName, 0);

	// Code below is probably wrong.
	lua_pushstring(state, "MetaID");
	lua_pushinteger(state, 1);
	lua_settable(state, -3);
}

bool ILuaBase::PushMetaTable(int iType)
{
	// ToDo. Idk how to do it yet.
}

void ILuaBase::PushUserType(void* data, int iType)
{
	// ToDo. Same
}

void ILuaBase::SetUserType(int iStackPos, void* data)
{
	// ToDo. Same
}

int LuaPanic(lua_State* lua)
{
	//lua_tolstring ToDo: Get the exact Lua and JIT Build gmod uses.

	Error("Lua Panic! Something went horribly wrong!\n %s", lua_tolstring(lua, 0, 0));
	return 0;
}

int AdvancedLuaErrorReporter(lua_State *L) {

}



bool CLuaInterface::Init( ILuaGameCallback* callback, bool bIsServer )
{
	gamecallback = callback;

	SetState(luaL_newstate());
	lua_State* L = GetState();

	lua_atpanic(L, LuaPanic);

	lua_pushcclosure(L, AdvancedLuaErrorReporter, 0);
	lua_setglobal(L, "AdvancedLuaErrorReporter");

	{
		luaJIT_setmode(L, -1, LUAJIT_MODE_WRAPCFUNC|LUAJIT_MODE_ON);

		// ToDo: Find out how to check the FPU precision
		// Warning("Lua detected bad FPU precision! Prepare for weirdness!");
	}

	//DoStackCheck();

    lua_pushinteger(L, 1);
    lua_setglobal(L, "AcceptInput");

    lua_pushinteger(L, 2);
    lua_setglobal(L, "AdjustMouseSensitivity");

    lua_pushinteger(L, 3);
    lua_setglobal(L, "AllowPlayerPickup");

    lua_pushinteger(L, 4);
    lua_setglobal(L, "CalcMainActivity");

    lua_pushinteger(L, 5);
    lua_setglobal(L, "CalcView");

    lua_pushinteger(L, 6);
    lua_setglobal(L, "CalcViewModelView");

    lua_pushinteger(L, 7);
    lua_setglobal(L, "CanExitVehicle");
}