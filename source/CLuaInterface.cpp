#include "CLuaInterface.h"
#include "CLuaConVars.h"
#include "lua/lj_obj.h"
#include "lua/luajit_rolling.h"
#include "lua/lauxlib.h"

#ifdef ARCHITECTURE_X86
#include "Color.h"
#else
#include "color.h"
#endif

int g_iTypeNum = 0;

// =================================
// First functions
// =================================
void lua_init_stack_gmod(lua_State* L1, lua_State* L)
{
    if (L && L != L1)
	{
		L1->luabase = L->luabase;
		if (L->luabase)
			((ILuaBase*)L->luabase)->SetState(L);
	}
}

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
	Msg("CreateLuaInterface\n");
	// ToDo: Create ILuaGameCallback and pass it to interface->Init as the first argument!
	CLuaGameCallback* callback = new CLuaGameCallback;

	CLuaInterface* LuaInterface = new CLuaInterface;
	LuaInterface->Init(callback, bIsServer);

	return LuaInterface;
}

void CloseLuaInterface(ILuaInterface* LuaInterface)
{
	Msg("CloseLuaInterface\n");
	// ToDo: Add all fancy operations and delete ILuaGameCallback and the Lua state!
	LuaInterface->Shutdown();

	delete LuaInterface;
}

// =================================
// Functions needed
// =================================

void luaL_newmetatable_type(lua_State* L, const char* strName, int iType)
{
	Msg("luaL_newmetatable_type\n");
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
	Msg("CLuaInterface::Top\n");
	return lua_gettop(state);
}

void CLuaInterface::Push(int iStackPos)
{
	Msg("CLuaInterface::Push\n");
	lua_pushvalue(state, iStackPos);
}

void CLuaInterface::Pop(int iAmt)
{
	Msg("CLuaInterface::Pop\n");
	lua_settop(state, iAmt);
}

void CLuaInterface::GetTable(int iStackPos)
{
	Msg("CLuaInterface::GetTable\n");
	lua_gettable(state, iStackPos);
}

void CLuaInterface::GetField(int iStackPos, const char* strName)
{
	Msg("CLuaInterface::GetField\n");
	lua_getfield(state, iStackPos, strName);
}

void CLuaInterface::SetField(int iStackPos, const char* strName)
{
	Msg("CLuaInterface::SetField\n");
	lua_setfield(state, iStackPos, strName);
}

void CLuaInterface::CreateTable()
{
	Msg("CLuaInterface::CreateTable\n");
	lua_createtable(state, 0, 0);
}

void CLuaInterface::SetTable(int iStackPos)
{
	Msg("CLuaInterface::SetTable\n");
	lua_settable(state, iStackPos);
}

void CLuaInterface::SetMetaTable(int iStackPos)
{
	Msg("CLuaInterface::SetMetaTable\n");
	lua_setmetatable(state, iStackPos);
}

bool CLuaInterface::GetMetaTable(int iStackPos)
{
	Msg("CLuaInterface::GetMetaTable\n");
	return lua_getmetatable(state, iStackPos);
}

void CLuaInterface::Call(int iArgs, int iResults)
{
	Msg("CLuaInterface::Call\n");
	lua_call(state, iArgs, iResults);
}

int CLuaInterface::PCall(int iArgs, int iResults, int iErrorFunc)
{
	Msg("CLuaInterface::PCall\n");
	return lua_pcall(state, iArgs, iResults, iErrorFunc);
}

int CLuaInterface::Equal(int iA, int iB)
{
	Msg("CLuaInterface::Equal\n");
	return lua_equal(state, iA, iB);
}

int CLuaInterface::RawEqual(int iA, int iB)
{
	Msg("CLuaInterface::RawEqual\n");
	return lua_rawequal(state, iA, iB);
}

void CLuaInterface::Insert(int iStackPos)
{
	Msg("CLuaInterface::Insert\n");
	lua_insert(state, iStackPos);
}

void CLuaInterface::Remove(int iStackPos)
{
	Msg("CLuaInterface::Remove\n");
	lua_remove(state, iStackPos);
}

int CLuaInterface::Next(int iStackPos)
{
	Msg("CLuaInterface::Next\n");
	return lua_next(state, iStackPos);
}

void* CLuaInterface::NewUserdata(unsigned int iSize)
{
	Msg("CLuaInterface::NewUserdata\n");
	return lua_newuserdata(state, iSize);
}

void CLuaInterface::ThrowError(const char* strError)
{
	Msg("CLuaInterface::ThrowError\n");
	luaL_error(state, "%s", strError);
}

void CLuaInterface::CheckType(int iStackPos, int iType)
{
	Msg("CLuaInterface::CheckType\n");
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
	Msg("CLuaInterface::ArgError\n");
	luaL_argerror(state, iArgNum, strMessage);
}

void CLuaInterface::RawGet(int iStackPos)
{
	Msg("CLuaInterface::RawGet\n");
	lua_rawget(state, iStackPos);
}

void CLuaInterface::RawSet(int iStackPos)
{
	Msg("CLuaInterface::RawSet\n");
	lua_rawset(state, iStackPos);
}

const char* CLuaInterface::GetString(int iStackPos, unsigned int* iOutLen)
{
	Msg("CLuaInterface::GetString\n");
	return lua_tolstring(state, iStackPos, reinterpret_cast<std::size_t*>(iOutLen));
}

double CLuaInterface::GetNumber(int iStackPos)
{
	Msg("CLuaInterface::GetNumber\n");
	return lua_tonumber(state, iStackPos);
}

bool CLuaInterface::GetBool(int iStackPos)
{
	Msg("CLuaInterface::GetBool\n");
	return lua_toboolean(state, iStackPos);
}

CFunc CLuaInterface::GetCFunction(int iStackPos)
{
	Msg("CLuaInterface::GetCFunction\n");
	return lua_tocfunction(state, iStackPos);
}

void* CLuaInterface::GetUserdata(int iStackPos)
{
	Msg("CLuaInterface::GetUserdata\n");
	return lua_touserdata(state, iStackPos);
}

void CLuaInterface::PushNil()
{
	Msg("CLuaInterface::PushNil\n");
	lua_pushnil(state);
}

void CLuaInterface::PushString(const char* val, unsigned int iLen)
{
	Msg("CLuaInterface::PushString\n");
	if (iLen > 0) {
		lua_pushlstring(state, val, iLen);
	} else {
		lua_pushstring(state, val);
	}
}

void CLuaInterface::PushNumber(double val)
{
	Msg("CLuaInterface::PushNumber\n");
	lua_pushnumber(state, val);
}

void CLuaInterface::PushBool(bool val)
{
	Msg("CLuaInterface::PushBool\n");
	lua_pushboolean(state, val);
}

void CLuaInterface::PushCFunction(CFunc val)
{
	Msg("CLuaInterface::PushCFunction\n");
	//lua_pushcfunction(state, val);
	lua_pushcclosure(state, val, 0);
}

void CLuaInterface::PushCClosure(CFunc val, int iVars)
{
	Msg("CLuaInterface::PushCClosure\n");
	lua_pushcclosure(state, val, iVars);
}

void CLuaInterface::PushUserdata(void* val)
{
	Msg("CLuaInterface::PushUserdata\n");
	lua_pushlightuserdata(state, val);
}

int CLuaInterface::ReferenceCreate()
{
	Msg("CLuaInterface::ReferenceCreate\n");
	return luaL_ref(state, LUA_REGISTRYINDEX);
}

void CLuaInterface::ReferenceFree(int i)
{
	Msg("CLuaInterface::ReferenceFree\n");
	luaL_unref(state, LUA_REGISTRYINDEX, i);
}

void CLuaInterface::ReferencePush(int i)
{
	Msg("CLuaInterface::ReferencePush\n");
	lua_rawgeti(state, LUA_REGISTRYINDEX, i);
}

void CLuaInterface::PushSpecial(int iType)
{
	Msg("CLuaInterface::PushSpecial\n");
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
	Msg("CLuaInterface::IsType\n");
	int actualType = lua_type(state, iStackPos);
	bool isType = false;

	if (actualType == iType) {
		return true;
	}

	return false;
}

int CLuaInterface::GetType(int iStackPos)
{
	Msg("CLuaInterface::GetType\n");
	return lua_type(state, iStackPos);
}

const char* CLuaInterface::GetTypeName(int iType)
{
	Msg("CLuaInterface::GetTypeName\n");
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
	Msg("CLuaInterface::CreateMetaTableType\n");
	luaL_newmetatable_type(state, strName, iType);

	if (iType > 254) {
		return;
	}
}

const char* CLuaInterface::CheckString(int iStackPos)
{
	Msg("CLuaInterface::CheckString\n");
	return luaL_checklstring(state, iStackPos, nullptr);
}

double CLuaInterface::CheckNumber(int iStackPos)
{
	Msg("CLuaInterface::CheckNumber\n");
	return luaL_checknumber(state, iStackPos);
}

int CLuaInterface::ObjLen(int iStackPos)
{
	Msg("CLuaInterface::ObjLen\n");
	return lua_objlen(state, iStackPos);
}

// ------------------------------------------
// ToDo: Recheck every function below!
// ------------------------------------------

QAngle DefaultAngle = QAngle(0, 0, 0);
const QAngle& CLuaInterface::GetAngle(int iStackPos)
{
	Msg("CLuaInterface::GetAngle\n");
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
	Msg("CLuaInterface::GetVector\n");
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
	Msg("CLuaInterface::PushAngle\n");
	lua_newtable(state);

	lua_pushnumber(state, val.x);
	lua_setfield(state, -2, "1");

	lua_pushnumber(state, val.y);
	lua_setfield(state, -2, "2");

	lua_pushnumber(state, val.z);
	lua_setfield(state, -2, "3");

	PushMetaTable(Type::Angle);
	SetMetaTable(-2);
}

void CLuaInterface::PushVector(const Vector& val)
{
	Msg("CLuaInterface::PushVector\n");
	lua_newtable(state);

	lua_pushnumber(state, val.x);
	lua_setfield(state, -2, "1");

	lua_pushnumber(state, val.y);
	lua_setfield(state, -2, "2");

	lua_pushnumber(state, val.z);
	lua_setfield(state, -2, "3");

	PushMetaTable(Type::Vector);
	SetMetaTable(-2);
}

void CLuaInterface::SetState(lua_State* L)
{
	Msg("CLuaInterface::SetState\n");
	state = L;
}

int CLuaInterface::CreateMetaTable(const char* strName)
{
	Msg("CLuaInterface::CreateMetaTable\n");
	luaL_newmetatable_type(state, strName, 0);

	// Code below is probably wrong.
	lua_pushstring(state, "MetaID");
	lua_pushinteger(state, 1);
	lua_settable(state, -3);

	return 0; // ToDo: What should I return? :<
}

bool CLuaInterface::PushMetaTable(int iType)
{
	Msg("CLuaInterface::PushMetaTable\n");
	// ToDo. Idk how to do it yet.
	return false;
}

void CLuaInterface::PushUserType(void* data, int iType)
{
	Msg("CLuaInterface::PushUserType\n");
	// ToDo. Same
}

void CLuaInterface::SetUserType(int iStackPos, void* data)
{
	Msg("CLuaInterface::SetUserType\n");
	// ToDo. Same
}

// =================================
// ILuaInterface implementations
// =================================

int LuaPanic(lua_State* lua)
{
	Msg("CLuaInterface::LuaPanic\n");
	//lua_tolstring ToDo: Get the exact Lua and JIT Build gmod uses.

	Error("Lua Panic! Something went horribly wrong!\n %s", lua_tolstring(lua, 0, 0));
	return 0;
}

bool CLuaInterface::Init( ILuaGameCallback* callback, bool bIsServer )
{
	Msg("CLuaInterface::Init\n");
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
	Msg("CLuaInterface::Shutdown\n");
	// ToDo: Do the magic

	lua_close(state);
}

void CLuaInterface::Cycle()
{
	Msg("CLuaInterface::Cycle\n");
	// iLastTimeCheck

	DoStackCheck();

	RunThreadedCalls();
}

void CLuaInterface::RunThreadedCalls()
{
	Msg("CLuaInterface::RunThreadedCalls\n");
	pThreadedcalls.remove_if([] (ILuaThreadedCall* call) {
		return call->Execute();
	});
}

void* CLuaInterface::AddThreadedCall(ILuaThreadedCall* call)
{
	Msg("CLuaInterface::AddThreadedCall What called this?\n");
	pThreadedcalls.push_back(call);
	return nullptr;
}

GarrysMod::Lua::ILuaObject* CLuaInterface::Global()
{
	Msg("CLuaInterface::Global\n");
	return pGlobal;
}

GarrysMod::Lua::ILuaObject* CLuaInterface::GetObject(int index)
{
	Msg("CLuaInterface::GetObject\n");
	return (GarrysMod::Lua::ILuaObject*)GetUserdata(index);
}

void CLuaInterface::PushLuaObject(GarrysMod::Lua::ILuaObject* obj)
{
	Msg("CLuaInterface::PushLuaObject\n");
	obj->Push(); // It could be that CLuaObject calls this. Then we got a loop. ToDo: Verify and fix this.
}

void CLuaInterface::PushLuaFunction(CFunc func)
{
	Msg("CLuaInterface::PushLuaObject\n");
	lua_pushcclosure(state, func, 0);
}

void CLuaInterface::LuaError(const char* str, int iStackPos)
{
	Msg("CLuaInterface::LuaError %s %i\n", str, iStackPos);
	
	if (iStackPos != -1)
	{
		luaL_argerror(state, iStackPos, str);
	} else {
		Msg("CLuaInterface::LuaError_IMPORTANT %s %i\n", str, iStackPos);
	}
}

void CLuaInterface::TypeError(const char* str, int iStackPos)
{
	Msg("CLuaInterface::LuaError %s %i\n", str, iStackPos);
	luaL_typerror(state, iStackPos, str);
}

void CLuaInterface::CallInternal(int args, int rets)
{
	Msg("CLuaInterface::CallInternal %i %i\n", args, rets);
	// ToDo
}

void CLuaInterface::CallInternalNoReturns(int args)
{
	Msg("CLuaInterface::CallInternalNoReturns %i\n", args);
	// ToDo
}

bool CLuaInterface::CallInternalGetBool(int args)
{
	Msg("CLuaInterface::CallInternalGetBool %i\n", args);
	// ToDo

	return false;
}

const char* CLuaInterface::CallInternalGetString(int args)
{
	Msg("CLuaInterface::CallInternalGetBool %i\n", args);
	// ToDo

	return "Nope";
}

bool CLuaInterface::CallInternalGet(int args, GarrysMod::Lua::ILuaObject* obj)
{
	Msg("CLuaInterface::CallInternalGet %i\n", args);
	// ToDo

	return false;
}

void CLuaInterface::NewGlobalTable(const char* name)
{
	Msg("CLuaInterface::NewGlobalTable %s\n", name);
	// ToDo
}

GarrysMod::Lua::ILuaObject* CLuaInterface::NewTemporaryObject()
{
	Msg("CLuaInterface::NewTemporaryObject\n");
	// ToDo

	return nullptr;
}

bool CLuaInterface::isUserData(int iStackPos)
{
	Msg("CLuaInterface::isUserData\n");
	// ToDo

	return lua_type(state, iStackPos) == Type::UserData;
}

GarrysMod::Lua::ILuaObject* CLuaInterface::GetMetaTableObject(const char* name, int type)
{
	Msg("CLuaInterface::GetMetaTableObject\n");
	// ToDo

	return nullptr;
}

GarrysMod::Lua::ILuaObject* CLuaInterface::GetMetaTableObject(int iStackPos)
{
	Msg("CLuaInterface::GetMetaTableObject\n");
	// ToDo

	return nullptr;
}

GarrysMod::Lua::ILuaObject* CLuaInterface::GetReturn(int iStackPos)
{
	Msg("CLuaInterface::GetReturn\n");
	// ToDo

	return nullptr;
}

bool CLuaInterface::IsServer()
{
	Msg("CLuaInterface::IsServer\n");
	// ToDo

	return false;
}

bool CLuaInterface::IsClient()
{
	Msg("CLuaInterface::IsClient\n");
	// ToDo

	return false;
}

bool CLuaInterface::IsMenu()
{
	Msg("CLuaInterface::IsMenu\n");
	// ToDo

	return true;
}

void CLuaInterface::DestroyObject(GarrysMod::Lua::ILuaObject* obj)
{
	Msg("CLuaInterface::DestroyObject\n");
	// ToDo
}

GarrysMod::Lua::ILuaObject* CLuaInterface::CreateObject()
{
	Msg("CLuaInterface::CreateObject\n");
	// ToDo

	return nullptr;
}

void CLuaInterface::SetMember(GarrysMod::Lua::ILuaObject* obj, GarrysMod::Lua::ILuaObject* key, GarrysMod::Lua::ILuaObject* value)
{
	Msg("CLuaInterface::SetMember 1\n");
	// ToDo
}

void CLuaInterface::GetNewTable()
{
	Msg("CLuaInterface::GetNewTable\n");
	// ToDo
}

void CLuaInterface::SetMember(GarrysMod::Lua::ILuaObject* obj, float key)
{
	Msg("CLuaInterface::SetMember 2\n");
	// ToDo
}

void CLuaInterface::SetMember(GarrysMod::Lua::ILuaObject* obj, float key, GarrysMod::Lua::ILuaObject* value)
{
	Msg("CLuaInterface::SetMember 3\n");
	// ToDo
}

void CLuaInterface::SetMember(GarrysMod::Lua::ILuaObject* obj, const char* key)
{
	Msg("CLuaInterface::SetMember 4\n");
	// ToDo
}

void CLuaInterface::SetMember(GarrysMod::Lua::ILuaObject* obj, const char* key, GarrysMod::Lua::ILuaObject* value)
{
	Msg("CLuaInterface::SetMember 5\n");
	// ToDo
}

void CLuaInterface::SetType(unsigned char unknown)
{
	Msg("CLuaInterface::SetType %s\n", unknown);
	// ToDo
}

void CLuaInterface::PushLong(long number)
{
	Msg("CLuaInterface::PushLong\n");
	lua_pushnumber(state, number);
}

int CLuaInterface::GetFlags(int iStackPos) // What da hell
{
	Msg("CLuaInterface::GetFlags\n");
	// ToDo

	return 0;
}

bool CLuaInterface::FindOnObjectsMetaTable(int iStackPos, int keyIndex)
{
	Msg("CLuaInterface::FindOnObjectsMetaTable\n");
	// ToDo

	return false;
}

bool CLuaInterface::FindObjectOnTable(int iStackPos, int keyIndex)
{
	Msg("CLuaInterface::FindObjectOnTable\n");
	// ToDo

	return false;
}

void CLuaInterface::SetMemberFast(GarrysMod::Lua::ILuaObject* table, int keyIndex, int valueIndex)
{
	Msg("CLuaInterface::SetMemberFast\n");
	// ToDo
}

bool CLuaInterface::RunString(const char* filename, const char* path, const char* stringToRun, bool run, bool showErrors)
{
	Msg("CLuaInterface::RunString\n");
	return RunStringEx(filename, path, stringToRun, run, showErrors, true, true);
}

bool CLuaInterface::IsEqual(GarrysMod::Lua::ILuaObject* objA, GarrysMod::Lua::ILuaObject* objB)
{
	Msg("CLuaInterface::IsEqual\n");
	return false;
}

void CLuaInterface::Error(const char* err)
{
	Msg("CLuaInterface::IsEqual\n");
	luaL_error(state, "%s", err);
}

const char* CLuaInterface::GetStringOrError(int index)
{
	Msg("CLuaInterface::GetStringOrError\n");
	const char* string = lua_tolstring(state, index, NULL);
	if (string == NULL)
	{
		Error("You betraid me"); // ToDo: This should probably be an Arg error
	}

	return string;
}

bool CLuaInterface::RunLuaModule(const char* name)
{
	Msg("CLuaInterface::RunLuaModule\n");
	// ToDo
	char* dest = new char[511];
	V_snprintf(dest, 511, "includes/modules/%s.lua", name);

	// NOTE: Why does it use !MODULE ?
	bool found = FindAndRunScript(dest, true, true, "", true);

	delete[] dest;

	return found;
}

bool CLuaInterface::FindAndRunScript(const char *filename, bool run, bool showErrors, const char *stringToRun, bool noReturns)
{
	Msg("CLuaInterface::FindAndRunScript\n");
	// ToDo

	return false;
}

void CLuaInterface::SetPathID(const char* pathID)
{
	Msg("CLuaInterface::SetPathID %s\n", pathID);
	// ToDo
}

const char* CLuaInterface::GetPathID()
{
	Msg("CLuaInterface::GetPathID\n");
	// ToDo
	return "";
}

void CLuaInterface::ErrorNoHalt( const char* fmt, ... )
{
	Msg("CLuaInterface::ErrorNoHalt %s\n", fmt);
	// ToDo
}

void CLuaInterface::Msg( const char* fmt, ... )
{
	::Msg("CLuaInterface::Msg %s\n", fmt);
	// ToDo
}

void CLuaInterface::PushPath( const char* path )
{
	Msg("CLuaInterface::PushPath %s\n", path);
	// ToDo
}

void CLuaInterface::PopPath()
{
	Msg("CLuaInterface::PopPath\n");
	// ToDo
}

const char* CLuaInterface::GetPath()
{
	Msg("CLuaInterface::GetPath\n");
	// ToDo

	return "";
}

int CLuaInterface::GetColor(int iStackPos) // Probably returns the StackPos
{
	Msg("CLuaInterface::GetColor\n");
	// ToDo
	return 0;
}

void CLuaInterface::PushColor(Color color)
{
	Msg("CLuaInterface::PushColor\n");
	// ToDo
}

int CLuaInterface::GetStack(int level, lua_Debug* dbg)
{
	Msg("CLuaInterface::GetStack\n");
	// ToDo

	return 0;
}

int CLuaInterface::GetInfo(const char* what, lua_Debug* dbg)
{
	Msg("CLuaInterface::GetStack\n");
	// ToDo

	return 0;
}

const char* CLuaInterface::GetLocal(lua_Debug* dbg, int n)
{
	Msg("CLuaInterface::GetLocal\n");
	// ToDo

	return "";
}

const char* CLuaInterface::GetUpvalue(int funcIndex, int n)
{
	Msg("CLuaInterface::GetUpvalue\n");
	// ToDo

	return "";
}

bool CLuaInterface::RunStringEx(const char *filename, const char *path, const char *stringToRun, bool run, bool printErrors, bool dontPushErrors, bool noReturns)
{
	Msg("CLuaInterface::RunStringEx\n");
	// ToDo

	return false;
}

size_t CLuaInterface::GetDataString(int index, const char **str)
{
	Msg("CLuaInterface::GetDataString\n");
	// ToDo

	return 0;
}

void CLuaInterface::ErrorFromLua(const char *fmt, ...)
{
	Msg("CLuaInterface::ErrorFromLua\n");
	// ToDo
}

const char* CLuaInterface::GetCurrentLocation()
{
	Msg("CLuaInterface::GetCurrentLocation\n");
	// ToDo

	return "RandomLocation :D";
}

void CLuaInterface::MsgColour(const Color& col, const char* fmt, ...)
{
	Msg("CLuaInterface::MsgColour\n");
	// ToDo
}

void CLuaInterface::GetCurrentFile(std::string &outStr)
{
	Msg("CLuaInterface::GetCurrentFile\n");
	// ToDo
}

void CLuaInterface::CompileString(Bootil::Buffer& dumper, const std::string& stringToCompile)
{
	Msg("CLuaInterface::CompileString\n");
	// ToDo
}

bool CLuaInterface::CallFunctionProtected(int iArgs, int iRets, bool showError)
{
	Msg("CLuaInterface::CallFunctionProtected\n");
	// ToDo
	return false;
}

void CLuaInterface::Require(const char* name)
{
	Msg("CLuaInterface::Require\n");
	// ToDo
}

const char* CLuaInterface::GetActualTypeName(int type)
{
	Msg("CLuaInterface::GetActualTypeName\n");
	// ToDo

	return "RandomType :D";
}

void CLuaInterface::PreCreateTable(int arrelems, int nonarrelems)
{
	Msg("CLuaInterface::PreCreateTable\n");
	// ToDo
}

void CLuaInterface::PushPooledString(int index)
{
	Msg("CLuaInterface::PushPooledString\n");
	// ToDo
}

const char* CLuaInterface::GetPooledString(int index)
{
	Msg("CLuaInterface::GetPooledString\n");
	// ToDo

	return "Nope";
}

void CLuaInterface::AppendStackTrace(char *, unsigned long)
{
	Msg("CLuaInterface::AppendStackTrace\n");
	// ToDo
}

void* CLuaInterface::CreateConVar(const char* name, const char* defaultValue, const char* helpString, int flags)
{
	Msg("CLuaInterface::CreateConVar\n");
	// ToDo

	return LuaConVars()->CreateConVar(name, defaultValue, helpString, flags);
}

void* CLuaInterface::CreateConCommand(const char* name, const char* helpString, int flags, FnCommandCallback_t callback, FnCommandCompletionCallback completionFunc)
{
	Msg("CLuaInterface::CreateConCommand\n");
	// ToDo

	return LuaConVars()->CreateConCommand(name, helpString, flags, callback, completionFunc);
}