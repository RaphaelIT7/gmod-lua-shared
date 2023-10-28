#include "CLuaInterface.h"

// =================================
// First functions
// =================================

std::string g_LastError;
std::vector<lua_Debug*> stackErrors;
int ReadStackIntoError(lua_State* L)
{
	// VPROF ReadStackIntoError GLua
	lua_Debug* stack = new lua_Debug;
	if (lua_getstack(L, 0, stack))
	{
		if (lua_getinfo(L, "Sln", stack))
		{
			stackErrors.push_back(stack);
		}
	}

	return 0;
}

int AdvancedLuaErrorReporter(lua_State *L) 
{
	// VPROF AdvancedLuaErrorReporter GLua

	if (lua_isstring(L, 0)) {
        const char* str = lua_tostring(L, 0);

        g_LastError.assign(str);

        ReadStackIntoError(L);

        lua_pushstring(L, g_LastError.c_str());
    }

	return 0;
}

ILuaInterface* CreateLuaInterface(bool bIsServer)
{
	// ToDo: Create ILuaGameCallback and pass it to interface->Init as the first argument!
	CLuaGameCallback* callback = new CLuaGameCallback;

	CLuaInterface* LuaInterface = new CLuaInterface;
	LuaInterface->Init(callback, bIsServer);

	return LuaInterface;
}

void CloseLuaInterface(ILuaInterface* LuaInterface)
{
	// ToDo: Add all fancy operations and delete ILuaGameCallback and the Lua state!
	LuaInterface->Shutdown();

	delete LuaInterface;
}

// =================================
// Functions needed
// =================================

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

// =================================
// ILuaBase implementation
// =================================

int CLuaInterface::Top()
{
	return lua_gettop(state);
}

void CLuaInterface::Push(int iStackPos)
{
	lua_pushvalue(state, iStackPos);
}

void CLuaInterface::Pop(int iAmt)
{
	lua_settop(state, iAmt);
}

void CLuaInterface::GetTable(int iStackPos)
{
	lua_gettable(state, iStackPos);
}

void CLuaInterface::GetField(int iStackPos, const char* strName)
{
	lua_getfield(state, iStackPos, strName);
}

void CLuaInterface::SetField(int iStackPos, const char* strName)
{
	lua_setfield(state, iStackPos, strName);
}

void CLuaInterface::CreateTable()
{
	lua_createtable(state, 0, 0);
}

void CLuaInterface::SetTable(int iStackPos)
{
	lua_settable(state, iStackPos);
}

void CLuaInterface::SetMetaTable(int iStackPos)
{
	lua_setmetatable(state, iStackPos);
}

bool CLuaInterface::GetMetaTable(int iStackPos)
{
	return lua_getmetatable(state, iStackPos);
}

void CLuaInterface::Call(int iArgs, int iResults)
{
	lua_call(state, iArgs, iResults);
}

int CLuaInterface::PCall(int iArgs, int iResults, int iErrorFunc)
{
	return lua_pcall(state, iArgs, iResults, iErrorFunc);
}

int CLuaInterface::Equal(int iA, int iB)
{
	return lua_equal(state, iA, iB);
}

int CLuaInterface::RawEqual(int iA, int iB)
{
	return lua_rawequal(state, iA, iB);
}

void CLuaInterface::Insert(int iStackPos)
{
	lua_insert(state, iStackPos);
}

void CLuaInterface::Remove(int iStackPos)
{
	lua_remove(state, iStackPos);
}

int CLuaInterface::Next(int iStackPos)
{
	return lua_next(state, iStackPos);
}

void* CLuaInterface::NewUserdata(unsigned int iSize)
{
	return lua_newuserdata(state, iSize);
}

void CLuaInterface::ThrowError(const char* strError)
{
	luaL_error(state, "%s", strError);
}

void CLuaInterface::CheckType(int iStackPos, int iType)
{
	int actualType = lua_type(state, iStackPos);
	if (actualType != iType) {
		const char* expectedType = lua_typename(state, iType);
		const char* actualTypeName = lua_typename(state, actualType);
		const char* errorMessage = lua_pushfstring(state, "Expected type %s at stack position %d, but got type %s.", expectedType, iStackPos, actualTypeName);
		lua_error(state);
	}
}

void CLuaInterface::ArgError(int iArgNum, const char* strMessage)
{
	luaL_argerror(state, iArgNum, strMessage);
}

void CLuaInterface::RawGet(int iStackPos)
{
	lua_rawget(state, iStackPos);
}

void CLuaInterface::RawSet(int iStackPos)
{
	lua_rawset(state, iStackPos);
}

const char* CLuaInterface::GetString(int iStackPos, unsigned int* iOutLen)
{
	return lua_tolstring(state, iStackPos, iOutLen);
}

double CLuaInterface::GetNumber(int iStackPos)
{
	return lua_tonumber(state, iStackPos);
}

bool CLuaInterface::GetBool(int iStackPos)
{
	return lua_toboolean(state, iStackPos);
}

CFunc CLuaInterface::GetCFunction(int iStackPos)
{
	return lua_tocfunction(state, iStackPos);
}

void* CLuaInterface::GetUserdata(int iStackPos)
{
	return lua_touserdata(state, iStackPos);
}

void CLuaInterface::PushNil()
{
	lua_pushnil(state);
}

void CLuaInterface::PushString(const char* val, unsigned int iLen)
{
	if (iLen > 0) {
		lua_pushlstring(state, val, iLen);
	} else {
		lua_pushstring(state, val);
	}
}

void CLuaInterface::PushNumber(double val)
{
	lua_pushnumber(state, val);
}

void CLuaInterface::PushBool(bool val)
{
	lua_pushboolean(state, val);
}

void CLuaInterface::PushCFunction(CFunc val)
{
	//lua_pushcfunction(state, val);
	lua_pushcclosure(state, val, 0);
}

void CLuaInterface::PushCClosure(CFunc val, int iVars)
{
	lua_pushcclosure(state, val, iVars);
}

void CLuaInterface::PushUserdata(void* val)
{
	lua_pushlightuserdata(state, val);
}

int CLuaInterface::ReferenceCreate()
{
	return luaL_ref(state, LUA_REGISTRYINDEX);
}

void CLuaInterface::ReferenceFree(int i)
{
	luaL_unref(state, LUA_REGISTRYINDEX, i);
}

void CLuaInterface::ReferencePush(int i)
{
	lua_rawgeti(state, LUA_REGISTRYINDEX, i);
}

void CLuaInterface::PushSpecial(int iType)
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

bool CLuaInterface::IsType(int iStackPos, int iType)
{
	int actualType = lua_type(state, iStackPos);
	bool isType = false;

	if (actualType == iType) {
		return true;
	}

	return false;
}

int CLuaInterface::GetType(int iStackPos)
{
	return lua_type(state, iStackPos);
}

const char* CLuaInterface::GetTypeName(int iType)
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

void CLuaInterface::CreateMetaTableType(const char* strName, int iType)
{
	luaL_newmetatable_type(state, strName, iType);

	if (iType > 254) {
		return;
	}
}

const char* CLuaInterface::CheckString(int iStackPos)
{
	return luaL_checklstring(state, iStackPos, nullptr);
}

double CLuaInterface::CheckNumber(int iStackPos)
{
	return luaL_checknumber(state, iStackPos);
}

int CLuaInterface::ObjLen(int iStackPos)
{
	return lua_objlen(state, iStackPos);
}

// ------------------------------------------
// ToDo: Recheck every function below!
// ------------------------------------------

QAngle DefaultAngle = QAngle(0, 0, 0);
const QAngle& CLuaInterface::GetAngle(int iStackPos)
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
const Vector& CLuaInterface::GetVector(int iStackPos)
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

void CLuaInterface::PushAngle(const QAngle& val)
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

void CLuaInterface::PushVector(const Vector& val)
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

void CLuaInterface::SetState(lua_State* L)
{
	state = L;
}

int CLuaInterface::CreateMetaTable(const char* strName)
{
	luaL_newmetatable_type(state, strName, 0);

	// Code below is probably wrong.
	lua_pushstring(state, "MetaID");
	lua_pushinteger(state, 1);
	lua_settable(state, -3);

	return 0; // ToDo: What should I return? :<
}

bool CLuaInterface::PushMetaTable(int iType)
{
	// ToDo. Idk how to do it yet.
	return false;
}

void CLuaInterface::PushUserType(void* data, int iType)
{
	// ToDo. Same
}

void CLuaInterface::SetUserType(int iStackPos, void* data)
{
	// ToDo. Same
}

// =================================
// ILuaInterface implementations
// =================================

int LuaPanic(lua_State* lua)
{
	//lua_tolstring ToDo: Get the exact Lua and JIT Build gmod uses.

	Error("Lua Panic! Something went horribly wrong!\n %s", lua_tolstring(lua, 0, 0));
	return 0;
}

bool CLuaInterface::Init( ILuaGameCallback* callback, bool bIsServer )
{
	gamecallback = callback;

	state = luaL_newstate();

	lua_atpanic(state, LuaPanic);

	lua_pushcclosure(state, AdvancedLuaErrorReporter, 0);
	lua_setglobal(state, "AdvancedLuaErrorReporter");

	{
		luaJIT_setmode(state, -1, LUAJIT_MODE_WRAPCFUNC|LUAJIT_MODE_ON);

		// ToDo: Find out how to check the FPU precision
		// Warning("Lua detected bad FPU precision! Prepare for weirdness!");
	}

	DoStackCheck();

    lua_pushinteger(state, 1);
    lua_setglobal(state, "AcceptInput");

    lua_pushinteger(state, 2);
    lua_setglobal(state, "AdjustMouseSensitivity");

    lua_pushinteger(state, 3);
    lua_setglobal(state, "AllowPlayerPickup");

    lua_pushinteger(state, 4);
    lua_setglobal(state, "CalcMainActivity");

    lua_pushinteger(state, 5);
    lua_setglobal(state, "CalcView");

    lua_pushinteger(state, 6);
    lua_setglobal(state, "CalcViewModelView");

    lua_pushinteger(state, 7);
    lua_setglobal(state, "CanExitVehicle");

	return 0;
}

void CLuaInterface::Shutdown()
{
	// ToDo: Do the magic

	lua_close(state);
}

void CLuaInterface::Cycle()
{
	// iLastTimeCheck

	DoStackCheck();

	RunThreadedCalls();
}

void CLuaInterface::RunThreadedCalls()
{

}