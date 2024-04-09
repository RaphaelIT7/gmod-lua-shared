#include "CLuaInterface.h"
#include "CLuaConVars.h"
#include "lua/lj_obj.h"
#include "lua/luajit_rolling.h"
#include "lua/lauxlib.h"
#include "lua_shared.h"
#include "lua/lj_obj.h"
#include <regex>

#ifdef ARCHITECTURE_X86
#include "Color.h"
#else
#include "color.h"
#endif

int g_iTypeNum = 0;

ConVar lua_debugmode("lua_debugmode", "2", 0);
void DebugPrint(int level, const char* fmt, ...) {
	if (lua_debugmode.GetInt() < level)
		return;

	va_list args;
	va_start(args, fmt);

	int size = vsnprintf(NULL, 0, fmt, args);
	if (size < 0) {
		va_end(args);
		return;
	}

	char* buffer = new char[size + 1];
	vsnprintf(buffer, size + 1, fmt, args);

	Msg("%s", buffer);

	delete[] buffer;
	va_end(args);
}

// =================================
// First functions
// =================================
void hook_lua_init_stack_gmod(lua_State* L1, lua_State* L)
{
    if (L && L != L1)
	{
		L1->luabase = L->luabase;
		if (L->luabase)
			((ILuaBase*)L->luabase)->SetState(L);
	}
}

void hook_GMOD_LuaPrint(const char* str)
{
	Msg("%s", str);
}

void lua_run_menu_f( const CCommand &args )
{
	if ( args.ArgC() < 1 || args.Arg(1) == "" )
	{
		Msg("Usage: lua_run_menu <code>\n");
		return;
	}
 
	Msg("Code: %s\n", args.Arg(1));
	LuaShared()->GetLuaInterface(State::MENU)->RunString("RunString", "", args.Arg(1), true, true);
}
 
ConCommand lua_run_menu( "lua_run_menu", lua_run_menu_f , "lua_run_menu", 0);

std::string g_LastError;
std::vector<lua_Debug*> stackErrors;
CLuaError* ReadStackIntoError(lua_State* L)
{
	// VPROF ReadStackIntoError GLua
	lua_Debug* stack = new lua_Debug;

	int level = 0;
	lua_Debug ar;
	CLuaError* lua_error = new CLuaError;
	while (lua_getstack(L, level, &ar)) {
		lua_getinfo(L, "nSl", &ar);

		CLuaError::StackEntry entry;
		entry.source = ar.source ? ar.source : "unknown";
		entry.function = ar.name ? ar.name : "unknown";
		entry.line = ar.currentline;

		lua_error->stack.push_back(entry);

		++level;
	}

	lua_error->message = lua_tolstring(L, -1, NULL);

	return lua_error;
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
	::DebugPrint(1, "CreateLuaInterface\n");
	// ToDo: Create ILuaGameCallback and pass it to interface->Init as the first argument!

	CLuaInterface* LuaInterface = new CLuaInterface;

	return LuaInterface;
}

void CloseLuaInterface(ILuaInterface* LuaInterface)
{
	::DebugPrint(1, "CloseLuaInterface\n");
	// ToDo: Add all fancy operations and delete ILuaGameCallback and the Lua state!
	LuaInterface->Shutdown();

	delete LuaInterface;
}

// =================================
// Functions needed
// =================================

void luaL_newmetatable_type(lua_State* L, const char* strName, int iType)
{
	::DebugPrint(3, "luaL_newmetatable_type\n");
	luaL_newmetatable(L, strName);
	g_iTypeNum++;

	if (iType != -1)
	{
		lua_pushstring(L, "MetaName");
		lua_pushstring(L, strName);
		lua_settable(L, -3);

		lua_pushstring(L, "MetaID");
		lua_pushinteger(L, iType);
		lua_settable(L, -3);
	}
}

// =================================
// ILuaBase implementation
// =================================

int CLuaInterface::Top()
{
	::DebugPrint(3, "CLuaInterface::Top\n");
	return lua_gettop(state);
}

void CLuaInterface::Push(int iStackPos)
{
	::DebugPrint(3, "CLuaInterface::Push\n");
	lua_pushvalue(state, iStackPos);
}

void CLuaInterface::Pop(int iAmt)
{
	::DebugPrint(3, "CLuaInterface::Pop\n");
	lua_settop(state, iAmt);
}

void CLuaInterface::GetTable(int iStackPos)
{
	::DebugPrint(3, "CLuaInterface::GetTable\n");
	lua_gettable(state, iStackPos);
}

void CLuaInterface::GetField(int iStackPos, const char* strName)
{
	::DebugPrint(3, "CLuaInterface::GetField\n");
	lua_getfield(state, iStackPos, strName);
}

void CLuaInterface::SetField(int iStackPos, const char* strName)
{
	::DebugPrint(3, "CLuaInterface::SetField\n");
	lua_setfield(state, iStackPos, strName);
}

void CLuaInterface::CreateTable()
{
	::DebugPrint(3, "CLuaInterface::CreateTable\n");
	lua_createtable(state, 0, 0);
}

void CLuaInterface::SetTable(int iStackPos)
{
	::DebugPrint(3, "CLuaInterface::SetTable\n");
	lua_settable(state, iStackPos);
}

void CLuaInterface::SetMetaTable(int iStackPos)
{
	::DebugPrint(3, "CLuaInterface::SetMetaTable\n");
	lua_setmetatable(state, iStackPos);
}

bool CLuaInterface::GetMetaTable(int iStackPos)
{
	::DebugPrint(3, "CLuaInterface::GetMetaTable\n");
	return lua_getmetatable(state, iStackPos);
}

void CLuaInterface::Call(int iArgs, int iResults)
{
	::DebugPrint(3, "CLuaInterface::Call\n");
	lua_call(state, iArgs, iResults);
}

int CLuaInterface::PCall(int iArgs, int iResults, int iErrorFunc)
{
	::DebugPrint(2, "CLuaInterface::PCall\n");
	return lua_pcall(state, iArgs, iResults, iErrorFunc);
}

int CLuaInterface::Equal(int iA, int iB)
{
	::DebugPrint(3, "CLuaInterface::Equal\n");
	return lua_equal(state, iA, iB);
}

int CLuaInterface::RawEqual(int iA, int iB)
{
	::DebugPrint(3, "CLuaInterface::RawEqual\n");
	return lua_rawequal(state, iA, iB);
}

void CLuaInterface::Insert(int iStackPos)
{
	::DebugPrint(3, "CLuaInterface::Insert\n");
	lua_insert(state, iStackPos);
}

void CLuaInterface::Remove(int iStackPos)
{
	::DebugPrint(3, "CLuaInterface::Remove\n");
	lua_remove(state, iStackPos);
}

int CLuaInterface::Next(int iStackPos)
{
	::DebugPrint(3, "CLuaInterface::Next\n");
	return lua_next(state, iStackPos);
}

void* CLuaInterface::NewUserdata(unsigned int iSize)
{
	::DebugPrint(3, "CLuaInterface::NewUserdata\n");
	return lua_newuserdata(state, iSize);
}

void CLuaInterface::ThrowError(const char* strError)
{
	::DebugPrint(2, "CLuaInterface::ThrowError\n");
	luaL_error(state, "%s", strError);
}

void CLuaInterface::CheckType(int iStackPos, int iType)
{
	::DebugPrint(3, "CLuaInterface::CheckType\n");
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
	::DebugPrint(2, "CLuaInterface::ArgError\n");
	luaL_argerror(state, iArgNum, strMessage);
}

void CLuaInterface::RawGet(int iStackPos)
{
	::DebugPrint(3, "CLuaInterface::RawGet\n");
	lua_rawget(state, iStackPos);
}

void CLuaInterface::RawSet(int iStackPos)
{
	::DebugPrint(3, "CLuaInterface::RawSet\n");
	lua_rawset(state, iStackPos);
}

const char* CLuaInterface::GetString(int iStackPos, unsigned int* iOutLen)
{
	::DebugPrint(4, "CLuaInterface::GetString\n");
	return lua_tolstring(state, iStackPos, reinterpret_cast<std::size_t*>(iOutLen));
}

double CLuaInterface::GetNumber(int iStackPos)
{
	::DebugPrint(4, "CLuaInterface::GetNumber\n");
	return lua_tonumber(state, iStackPos);
}

bool CLuaInterface::GetBool(int iStackPos)
{
	::DebugPrint(4, "CLuaInterface::GetBool\n");
	return lua_toboolean(state, iStackPos);
}

CFunc CLuaInterface::GetCFunction(int iStackPos)
{
	::DebugPrint(4, "CLuaInterface::GetCFunction\n");
	return lua_tocfunction(state, iStackPos);
}

void* CLuaInterface::GetUserdata(int iStackPos)
{
	::DebugPrint(4, "CLuaInterface::GetUserdata\n");
	return lua_touserdata(state, iStackPos);
}

void CLuaInterface::PushNil()
{
	::DebugPrint(4, "CLuaInterface::PushNil\n");
	lua_pushnil(state);
}

void CLuaInterface::PushString(const char* val, unsigned int iLen)
{
	::DebugPrint(4, "CLuaInterface::PushString %s\n", val);
	if (iLen > 0) {
		lua_pushlstring(state, val, iLen);
	} else {
		lua_pushstring(state, val);
	}
}

void CLuaInterface::PushNumber(double val)
{
	::DebugPrint(4, "CLuaInterface::PushNumber\n");
	lua_pushnumber(state, val);
}

void CLuaInterface::PushBool(bool val)
{
	::DebugPrint(4, "CLuaInterface::PushBool\n");
	lua_pushboolean(state, val);
}

void CLuaInterface::PushCFunction(CFunc val)
{
	::DebugPrint(4, "CLuaInterface::PushCFunction\n");
	//lua_pushcfunction(state, val);
	lua_pushcclosure(state, val, 0);
}

void CLuaInterface::PushCClosure(CFunc val, int iVars)
{
	::DebugPrint(4, "CLuaInterface::PushCClosure\n");
	lua_pushcclosure(state, val, iVars);
}

void CLuaInterface::PushUserdata(void* val)
{
	::DebugPrint(4, "CLuaInterface::PushUserdata\n");
	lua_pushlightuserdata(state, val);
}

int CLuaInterface::ReferenceCreate()
{
	::DebugPrint(4, "CLuaInterface::ReferenceCreate\n");
	return luaL_ref(state, LUA_REGISTRYINDEX);
}

void CLuaInterface::ReferenceFree(int i)
{
	::DebugPrint(4, "CLuaInterface::ReferenceFree\n");
	luaL_unref(state, LUA_REGISTRYINDEX, i);
}

void CLuaInterface::ReferencePush(int i)
{
	::DebugPrint(4, "CLuaInterface::ReferencePush\n");
	lua_rawgeti(state, LUA_REGISTRYINDEX, i);
}

void CLuaInterface::PushSpecial(int iType)
{
	::DebugPrint(3, "CLuaInterface::PushSpecial\n");
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
	::DebugPrint(4, "CLuaInterface::IsType\n");
	int actualType = lua_type(state, iStackPos);
	bool isType = false;

	if (actualType == iType) {
		return true;
	}

	return false;
}

int CLuaInterface::GetType(int iStackPos)
{
	::DebugPrint(4, "CLuaInterface::GetType\n");
	return lua_type(state, iStackPos);
}

const char* CLuaInterface::GetTypeName(int iType)
{
	::DebugPrint(4, "CLuaInterface::GetTypeName\n");
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
	::DebugPrint(2, "CLuaInterface::CreateMetaTableType\n");
	luaL_newmetatable_type(state, strName, iType);

	if (iType > 254) {
		return;
	}
}

const char* CLuaInterface::CheckString(int iStackPos)
{
	::DebugPrint(4, "CLuaInterface::CheckString\n");
	return luaL_checklstring(state, iStackPos, NULL);
}

double CLuaInterface::CheckNumber(int iStackPos)
{
	::DebugPrint(4, "CLuaInterface::CheckNumber\n");
	return luaL_checknumber(state, iStackPos);
}

int CLuaInterface::ObjLen(int iStackPos)
{
	::DebugPrint(3, "CLuaInterface::ObjLen\n");
	return lua_objlen(state, iStackPos);
}

// ------------------------------------------
// ToDo: Recheck every function below!
// ------------------------------------------

QAngle DefaultAngle = QAngle(0, 0, 0);
const QAngle& CLuaInterface::GetAngle(int iStackPos)
{
	::DebugPrint(2, "CLuaInterface::GetAngle\n");
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
	::DebugPrint(2, "CLuaInterface::GetVector\n");
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
	::DebugPrint(2, "CLuaInterface::PushAngle\n");
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
	::DebugPrint(2, "CLuaInterface::PushVector\n");
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
	::DebugPrint(1, "CLuaInterface::SetState\n");
	state = L;
}

int CLuaInterface::CreateMetaTable(const char* strName)
{
	::DebugPrint(2, "CLuaInterface::CreateMetaTable\n");
	luaL_newmetatable_type(state, strName, -1);

	// Code below is probably wrong.
	lua_pushstring(state, "MetaID");
	lua_pushinteger(state, 1);
	lua_settable(state, -3);

	return 0; // ToDo: What should I return? :<
}

bool CLuaInterface::PushMetaTable(int iType)
{
	::DebugPrint(2, "CLuaInterface::PushMetaTable\n");
	// ToDo. Idk how to do it yet.
	return false;
}

void CLuaInterface::PushUserType(void* data, int iType)
{
	::DebugPrint(2, "CLuaInterface::PushUserType %i\n", iType);

	void** ud = static_cast<void**>(lua_newuserdata(state, sizeof(data)));
	*ud = data;

	luaL_newmetatable(state, "ConVar");
	SetMetaTable(-2);
}

void CLuaInterface::SetUserType(int iStackPos, void* data)
{
	::DebugPrint(2, "CLuaInterface::SetUserType\n");
	// ToDo. Same
}

// =================================
// ILuaInterface implementations
// =================================

int LuaPanic(lua_State* lua)
{
	::DebugPrint(1, "CLuaInterface::LuaPanic\n");

	Error("Lua Panic! Something went horribly wrong!\n %s", lua_tolstring(lua, -1, 0));
	return 0;
}

int func_Include(lua_State* L)
{
	CLuaInterface* LUA = (CLuaInterface*)L->luabase;

	const char* file = LUA->CheckString(1);
	LUA->FindAndRunScript(file, true, true, "!UNKNOWN", true);

	return 0;
}

bool CLuaInterface::Init( ILuaGameCallback* callback, bool bIsServer )
{
	::DebugPrint(1, "CLuaInterface::Init Server: %s\n", bIsServer ? "Yes" : "No");
	gamecallback = callback;

	state = luaL_newstate();
	luaL_openlibs(state);

	state->luabase = this;
	SetState(state);

	lua_atpanic(state, LuaPanic);

	lua_pushcclosure(state, AdvancedLuaErrorReporter, 0);
	lua_setglobal(state, "AdvancedLuaErrorReporter");

	{
		luaJIT_setmode(state, -1, LUAJIT_MODE_WRAPCFUNC|LUAJIT_MODE_ON);

		// ToDo: Find out how to check the FPU precision
		// Warning("Lua detected bad FPU precision! Prepare for weirdness!");
	}

	DoStackCheck();

	NewGlobalTable("");

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

	return true;
}

void CLuaInterface::Shutdown()
{
	::DebugPrint(1, "CLuaInterface::Shutdown\n");
	// ToDo: Do the magic

	lua_close(state);
}

void CLuaInterface::Cycle()
{
	::DebugPrint(3, "CLuaInterface::Cycle\n");
	// iLastTimeCheck

	DoStackCheck();

	RunThreadedCalls();
}

void CLuaInterface::RunThreadedCalls()
{
	::DebugPrint(3, "CLuaInterface::RunThreadedCalls\n");
	pThreadedcalls.remove_if([] (ILuaThreadedCall* call) {
		return call->Execute();
	});
}

void* CLuaInterface::AddThreadedCall(ILuaThreadedCall* call)
{
	::DebugPrint(1, "CLuaInterface::AddThreadedCall What called this?\n");
	Error("Tell me. What called this?");
	pThreadedcalls.push_back(call);
	return nullptr;
}

GarrysMod::Lua::ILuaObject* CLuaInterface::Global()
{
	::DebugPrint(4, "CLuaInterface::Global\n");
	return pGlobal;
}

GarrysMod::Lua::ILuaObject* CLuaInterface::GetObject(int index)
{
	::DebugPrint(4, "CLuaInterface::GetObject\n");
	return (GarrysMod::Lua::ILuaObject*)GetUserdata(index);
}

void CLuaInterface::PushLuaObject(GarrysMod::Lua::ILuaObject* obj)
{
	::DebugPrint(4, "CLuaInterface::PushLuaObject\n");
	obj->Push();
}

void CLuaInterface::PushLuaFunction(CFunc func)
{
	::DebugPrint(4, "CLuaInterface::PushLuaObject\n");
	lua_pushcclosure(state, func, 0);
}

void CLuaInterface::LuaError(const char* str, int iStackPos)
{
	::DebugPrint(4, "CLuaInterface::LuaError %s %i\n", str, iStackPos);
	
	if (iStackPos != -1)
	{
		luaL_argerror(state, iStackPos, str);
	} else {
		::DebugPrint(1, "CLuaInterface::LuaError_IMPORTANT %s %i\n", str, iStackPos);
	}
}

void CLuaInterface::TypeError(const char* str, int iStackPos)
{
	::DebugPrint(2, "CLuaInterface::LuaError %s %i\n", str, iStackPos);
	luaL_typerror(state, iStackPos, str);
}

void CLuaInterface::CallInternal(int args, int rets)
{
	::DebugPrint(2, "CLuaInterface::CallInternal %i %i\n", args, rets);
	// ToDo
}

void CLuaInterface::CallInternalNoReturns(int args)
{
	::DebugPrint(2, "CLuaInterface::CallInternalNoReturns %i\n", args);
	// ToDo
}

bool CLuaInterface::CallInternalGetBool(int args)
{
	::DebugPrint(2, "CLuaInterface::CallInternalGetBool %i\n", args);
	// ToDo

	return false;
}

const char* CLuaInterface::CallInternalGetString(int args)
{
	::DebugPrint(2, "CLuaInterface::CallInternalGetBool %i\n", args);
	// ToDo

	return "Nope";
}

bool CLuaInterface::CallInternalGet(int args, GarrysMod::Lua::ILuaObject* obj)
{
	::DebugPrint(2, "CLuaInterface::CallInternalGet %i\n", args);
	// ToDo

	return false;
}

void CLuaInterface::NewGlobalTable(const char* name)
{
	::DebugPrint(1, "CLuaInterface::NewGlobalTable %s\n", name);

	if (pGlobal)
		gamecallback->DestroyLuaObject(pGlobal);

	pGlobal = gamecallback->CreateLuaObject();

	if (pGlobal)
	{
		PushSpecial(SPECIAL_GLOB);
		pGlobal->SetFromStack(-1);
		Pop(1);
		::DebugPrint(1, "Global type: %i\n", pGlobal->GetType());
	}
}

GarrysMod::Lua::ILuaObject* CLuaInterface::NewTemporaryObject()
{
	::DebugPrint(2, "CLuaInterface::NewTemporaryObject\n");
	// ToDo

	return nullptr;
}

bool CLuaInterface::isUserData(int iStackPos)
{
	::DebugPrint(4, "CLuaInterface::isUserData\n");
	// ToDo

	return lua_type(state, iStackPos) == Type::UserData;
}

GarrysMod::Lua::ILuaObject* CLuaInterface::GetMetaTableObject(const char* name, int type)
{
	::DebugPrint(2, "CLuaInterface::GetMetaTableObject %s, %i\n", name, type);

	PushSpecial(SPECIAL_REG);
	GetField(-1, name);

	if (IsType(-1, Type::Nil))
	{
		Pop(1);
		CreateMetaTableType(name, type);
		Push(-1);
		SetField(-2, name);
	}

	GarrysMod::Lua::ILuaObject* obj = CreateObject();
	obj->SetFromStack(-1);
	Pop(2);

	return obj;
}

GarrysMod::Lua::ILuaObject* CLuaInterface::GetMetaTableObject(int iStackPos)
{
	::DebugPrint(2, "CLuaInterface::GetMetaTableObject\n");
	// ToDo

	return nullptr;
}

GarrysMod::Lua::ILuaObject* CLuaInterface::GetReturn(int iStackPos)
{
	::DebugPrint(2, "CLuaInterface::GetReturn\n");
	// ToDo

	return nullptr;
}

bool CLuaInterface::IsServer()
{
	::DebugPrint(3, "CLuaInterface::IsServer\n");

	return pRealm == State::SERVER;
}

bool CLuaInterface::IsClient()
{
	::DebugPrint(3, "CLuaInterface::IsClient\n");

	return pRealm == State::CLIENT;
}

bool CLuaInterface::IsMenu()
{
	::DebugPrint(3, "CLuaInterface::IsMenu\n");

	return pRealm == State::MENU;
}

void CLuaInterface::DestroyObject(GarrysMod::Lua::ILuaObject* obj)
{
	::DebugPrint(4, "CLuaInterface::DestroyObject\n");
	gamecallback->DestroyLuaObject(obj);
}

GarrysMod::Lua::ILuaObject* CLuaInterface::CreateObject()
{
	::DebugPrint(4, "CLuaInterface::CreateObject\n");
	return gamecallback->CreateLuaObject();
}

void CLuaInterface::SetMember(GarrysMod::Lua::ILuaObject* obj, GarrysMod::Lua::ILuaObject* key, GarrysMod::Lua::ILuaObject* value)
{
	::DebugPrint(3, "CLuaInterface::SetMember 1\n");
	if (obj->isTable())
	{
		obj->Push();
		key->Push();
		value->Push();
		SetTable(-3);
		Pop(1);
	}
}

void CLuaInterface::GetNewTable()
{
	::DebugPrint(2, "CLuaInterface::GetNewTable\n");
	// ToDo
}

void CLuaInterface::SetMember(GarrysMod::Lua::ILuaObject* obj, float key)
{
	::DebugPrint(3, "CLuaInterface::SetMember 2\n");
	if (obj->isTable())
	{
		obj->Push();
		PushNumber(key);
		Push(-3);
		SetTable(-3);
		Pop(2);
	}
}

void CLuaInterface::SetMember(GarrysMod::Lua::ILuaObject* obj, float key, GarrysMod::Lua::ILuaObject* value)
{
	::DebugPrint(3, "CLuaInterface::SetMember 3\n");
	if (obj->isTable())
	{
		obj->Push();
		PushNumber(key);
		value->Push();
		SetTable(-3);
		Pop(1);
	}
}

void CLuaInterface::SetMember(GarrysMod::Lua::ILuaObject* obj, const char* key)
{
	::DebugPrint(3, "CLuaInterface::SetMember 4 %s\n", key);
	if (obj->isTable())
	{
		PushString(key);
		Push(-3);
		SetTable(-3);
		Pop(1);
	}

	if (obj == pGlobal)
	{
		PushSpecial(SPECIAL_GLOB);
		PushString(key);
		Push(-3);
		SetTable(-3);
		Pop(1);
	}
	Pop(1);
}

void CLuaInterface::SetMember(GarrysMod::Lua::ILuaObject* obj, const char* key, GarrysMod::Lua::ILuaObject* value)
{
	::DebugPrint(3, "CLuaInterface::SetMember 5\n");
	if (obj->isTable())
	{
		obj->Push();
		PushString(key);
		value->Push();
		SetTable(-3);
		Pop(1);
	}
}

void CLuaInterface::SetType(unsigned char realm)
{
	::DebugPrint(1, "CLuaInterface::SetType %u\n", realm);
	pRealm = realm;
}

void CLuaInterface::PushLong(long number)
{
	::DebugPrint(4, "CLuaInterface::PushLong\n");
	lua_pushnumber(state, number);
}

int CLuaInterface::GetFlags(int iStackPos) // What da hell
{
	::DebugPrint(1, "CLuaInterface::GetFlags\n");
	// ToDo

	return 0;
}

bool CLuaInterface::FindOnObjectsMetaTable(int iStackPos, int keyIndex)
{
	::DebugPrint(2, "CLuaInterface::FindOnObjectsMetaTable\n");
	// ToDo

	return false;
}

bool CLuaInterface::FindObjectOnTable(int iStackPos, int keyIndex)
{
	::DebugPrint(2, "CLuaInterface::FindObjectOnTable\n");
	// ToDo

	return false;
}

void CLuaInterface::SetMemberFast(GarrysMod::Lua::ILuaObject* obj, int keyIndex, int valueIndex)
{
	::DebugPrint(3, "CLuaInterface::SetMemberFast\n");
	if (obj->isTable())
	{
		obj->Push();
		Push(keyIndex + 1);
		Push(valueIndex + 2);
		SetTable(-3);
		Pop(1);
	}
}

bool CLuaInterface::RunString(const char* filename, const char* path, const char* stringToRun, bool run, bool showErrors)
{
	::DebugPrint(2, "CLuaInterface::RunString\n");
	return RunStringEx(filename, path, stringToRun, run, showErrors, true, true);
}

bool CLuaInterface::IsEqual(GarrysMod::Lua::ILuaObject* objA, GarrysMod::Lua::ILuaObject* objB)
{
	::DebugPrint(3, "CLuaInterface::IsEqual\n");
	// Still ToDo
	return objA == objB;
}

void CLuaInterface::Error(const char* err)
{
	::DebugPrint(2, "CLuaInterface::Error\n");
	luaL_error(state, "%s", err);
}

const char* CLuaInterface::GetStringOrError(int index)
{
	::DebugPrint(3, "CLuaInterface::GetStringOrError\n");
	const char* string = lua_tolstring(state, index, NULL);
	if (string == NULL)
	{
		Error("You betraid me"); // ToDo: This should probably be an Arg error
	}

	return string;
}

bool CLuaInterface::RunLuaModule(const char* name)
{
	::DebugPrint(2, "CLuaInterface::RunLuaModule\n");
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
	::DebugPrint(2, "CLuaInterface::FindAndRunScript %s, %s, %s, %s, %s\n", filename, run ? "Yes" : "No", showErrors ? "Yes" : "No", stringToRun, noReturns ? "Yes" : "No");
	
	ILuaShared* shared = LuaShared();
	File* file = nullptr;//shared->LoadFile(filename, pPathID, true, true);
	if (file)
		return RunStringEx(file->name.c_str(), file->source.c_str(), file->contents.c_str(), true, showErrors, true, noReturns);

	return false;
}

void CLuaInterface::SetPathID(const char* pathID)
{
	::DebugPrint(1, "CLuaInterface::SetPathID %s\n", pathID);
	pPathID = pathID;
}

const char* CLuaInterface::GetPathID()
{
	::DebugPrint(2, "CLuaInterface::GetPathID\n");

	return pPathID;
}

void CLuaInterface::ErrorNoHalt( const char* fmt, ... )
{
	::DebugPrint(2, "CLuaInterface::ErrorNoHalt %s\n", fmt);
	// ToDo
}

void CLuaInterface::Msg( const char* fmt, ... )
{
	::DebugPrint(2, "CLuaInterface::Msg %s\n", fmt);
	// ToDo
}

void CLuaInterface::PushPath( const char* path )
{
	::DebugPrint(2, "CLuaInterface::PushPath %s\n", path);
	// ToDo
}

void CLuaInterface::PopPath()
{
	::DebugPrint(2, "CLuaInterface::PopPath\n");
	// ToDo
}

const char* CLuaInterface::GetPath()
{
	::DebugPrint(2, "CLuaInterface::GetPath\n");
	// ToDo

	return NULL;
}

int CLuaInterface::GetColor(int iStackPos) // Probably returns the StackPos
{
	::DebugPrint(2, "CLuaInterface::GetColor\n");
	// ToDo
	return 0;
}

void CLuaInterface::PushColor(Color color)
{
	::DebugPrint(2, "CLuaInterface::PushColor\n");
	// ToDo
}

int CLuaInterface::GetStack(int level, lua_Debug* dbg)
{
	::DebugPrint(2, "CLuaInterface::GetStack\n");
	// ToDo

	return 0;
}

int CLuaInterface::GetInfo(const char* what, lua_Debug* dbg)
{
	::DebugPrint(2, "CLuaInterface::GetStack\n");
	// ToDo

	return 0;
}

const char* CLuaInterface::GetLocal(lua_Debug* dbg, int n)
{
	::DebugPrint(2, "CLuaInterface::GetLocal\n");
	// ToDo

	return "";
}

const char* CLuaInterface::GetUpvalue(int funcIndex, int n)
{
	::DebugPrint(2, "CLuaInterface::GetUpvalue\n");
	// ToDo

	return "";
}

bool CLuaInterface::RunStringEx(const char *filename, const char *path, const char *stringToRun, bool run, bool printErrors, bool dontPushErrors, bool noReturns)
{
	::DebugPrint(2, "CLuaInterface::RunStringEx %s, %s\n", filename, path);
	std::string code = RunMacros(stringToRun);
	int res = luaL_loadbuffer(state, code.c_str(), code.length(), filename);
	if (res != 0)
	{
		CLuaError* err = ReadStackIntoError(state);
		if (dontPushErrors)
			Pop(1);

		if (printErrors)
			gamecallback->LuaError(err);

		delete err;

		return false;
	} else {
		return CallFunctionProtected(0, 0, printErrors);
	}
}

size_t CLuaInterface::GetDataString(int index, const char **str)
{
	::DebugPrint(2, "CLuaInterface::GetDataString\n");
	// ToDo

	return 0;
}

void CLuaInterface::ErrorFromLua(const char *fmt, ...)
{
	::DebugPrint(2, "CLuaInterface::ErrorFromLua %s\n", fmt);
	CLuaError* error = ReadStackIntoError(state);

	va_list args;
	va_start(args, fmt);

	int size = vsnprintf(NULL, 0, fmt, args);
	if (size < 0) {
		va_end(args);
		return;
	}

	char* buffer = new char[size + 1];
	vsnprintf(buffer, size + 1, fmt, args);

	error->message = buffer;

	delete[] buffer;
	va_end(args);
	
	std::string realm;
	switch(pRealm)
	{
		case 0:
			realm = "client";
			break;
		case 1:
			realm = "server";
			break;
		case 2:
			realm = "menu";
			break;
		default:
			realm = "unknown";
			break;
	}
	error->side = realm;

	gamecallback->LuaError(error);

	delete error;
}

const char* CLuaInterface::GetCurrentLocation()
{
	::DebugPrint(2, "CLuaInterface::GetCurrentLocation\n");
	// ToDo

	return "RandomLocation :D";
}

void CLuaInterface::MsgColour(const Color& col, const char* fmt, ...)
{
	::DebugPrint(2, "CLuaInterface::MsgColour\n");
	// ToDo
}

void CLuaInterface::GetCurrentFile(std::string &outStr)
{
	::DebugPrint(2, "CLuaInterface::GetCurrentFile\n");

	lua_Debug ar;
	lua_getstack(state, 1, &ar);
	lua_getinfo(state, "S", &ar);

	outStr = ar.source ? ar.source : "!UNKNOWN";
}

void CLuaInterface::CompileString(Bootil::Buffer& dumper, const std::string& stringToCompile)
{
	::DebugPrint(2, "CLuaInterface::CompileString\n");
	// ToDo
}

bool CLuaInterface::CallFunctionProtected(int iArgs, int iRets, bool showError)
{
	::DebugPrint(2, "CLuaInterface::CallFunctionProtected\n");

	int ret = PCall(iArgs, iRets, 0);
	if (ret != 0)
	{
		CLuaError* err = ReadStackIntoError(state);
		if (showError)
		{
			gamecallback->LuaError(err);
		}
		delete err;
		Pop(1);
		::Msg("Top: %i\n", Top());
	}

	return ret != 0;
}

void CLuaInterface::Require(const char* name)
{
	::DebugPrint(2, "CLuaInterface::Require %s\n", name);
	// ToDo

	RunLuaModule(name);
}

const char* CLuaInterface::GetActualTypeName(int type)
{
	::DebugPrint(2, "CLuaInterface::GetActualTypeName\n");
	// ToDo

	return "RandomType :D";
}

void CLuaInterface::PreCreateTable(int arrelems, int nonarrelems)
{
	::DebugPrint(4, "CLuaInterface::PreCreateTable %i %i\n", arrelems, nonarrelems);
	lua_createtable(state, arrelems, nonarrelems);
}

void CLuaInterface::PushPooledString(int index)
{
	::DebugPrint(2, "CLuaInterface::PushPooledString\n");
	// ToDo
}

const char* CLuaInterface::GetPooledString(int index)
{
	::DebugPrint(2, "CLuaInterface::GetPooledString\n");
	// ToDo

	return "Nope";
}

void CLuaInterface::AppendStackTrace(char *, unsigned long)
{
	::DebugPrint(2, "CLuaInterface::AppendStackTrace\n");
	// ToDo
}

void* CLuaInterface::CreateConVar(const char* name, const char* defaultValue, const char* helpString, int flags)
{
	::DebugPrint(2, "CLuaInterface::CreateConVar\n");

	return LuaConVars()->CreateConVar(name, defaultValue, helpString, flags);
}

void* CLuaInterface::CreateConCommand(const char* name, const char* helpString, int flags, FnCommandCallback_t callback, FnCommandCompletionCallback completionFunc)
{
	::DebugPrint(2, "CLuaInterface::CreateConCommand\n");

	return LuaConVars()->CreateConCommand(name, helpString, flags, callback, completionFunc);
}

std::string CLuaInterface::RunMacros(std::string code)
{
	::Msg("Top: %i\n", Top());

	::DebugPrint(2, "CLuaInterface::RunMacros\n");
	
	// ToDo Move syntax to LuaJIT

	code = std::regex_replace(code, std::regex("&&"), "and");
	code = std::regex_replace(code, std::regex("\\|\\|"), "or");
	code = std::regex_replace(code, std::regex("!="), "~=");
	code = std::regex_replace(code, std::regex("!"), "not");
	code = std::regex_replace(code, std::regex("/\\*"), "--[[");
	code = std::regex_replace(code, std::regex("\\*/"), "]]");
	code = std::regex_replace(code, std::regex("//"), "--");
	code = std::regex_replace(code, std::regex("CreateConVar"), "CCreateConVar");

	// ToDo do Baseclass macro

	return code;
}