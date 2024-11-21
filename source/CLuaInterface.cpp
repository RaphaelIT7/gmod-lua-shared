#include "CLuaInterface.h"
#include "lua.hpp"
#include "CLuaConVars.h"
#include "../lua/lj_obj.h"
#include "../lua/luajit_rolling.h"
#include "../lua/lauxlib.h"
#include "lua_shared.h"
#include "../lua/lj_obj.h"
#include <regex>

#ifdef ARCHITECTURE_X86
#include "Color.h"
#else
#include "color.h"
#endif


int g_iTypeNum = 0;

static ConVar lua_debugmode("lua_debugmode_interface", "1", 0);
void DebugPrint(int level, const char* fmt, ...)
{
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
// Helper functions
// =================================
void lua_run_menu_f( const CCommand &args )
{
	if ( args.ArgC() < 1 || args.Arg(1) == "" )
	{
		Msg("Usage: lua_run_menu <code>\n");
		return;
	}
 
	Msg("Code: %s\n", args.ArgS());
	LuaShared()->GetLuaInterface(State::MENU)->RunString("RunString", "", args.ArgS(), true, true);
}
 
ConCommand lua_run_menu( "lua_run_menu", lua_run_menu_f , "lua_run_menu", 0);

// =================================
// First gmod function
// =================================
std::string g_LastError;
std::vector<lua_Debug*> stackErrors;
CLuaError* ReadStackIntoError(lua_State* L)
{
	// VPROF ReadStackIntoError GLua

	int level = 0;
	lua_Debug ar;
	CLuaError* lua_error = new CLuaError;
	while (lua_getstack(L, level, &ar)) {
		lua_getinfo(L, "nSl", &ar);

		CLuaError::StackEntry& entry = lua_error->stack.emplace_back();
#ifdef WIN32
		entry.source = ar.source ? ar.source : "unknown";
		entry.function = ar.name ? ar.name : "unknown";
#else
		const char* source = ar.source ? ar.source : "unknown";
		const char* function = ar.name ? ar.name : "unknown";

		char* csource = new char[strlen(source) + 1];
		char* cfunction = new char[strlen(function) + 1];
		V_strncpy(csource, source, strlen(source) + 1);
		V_strncpy(cfunction, function, strlen(function) + 1);
		cfunction[strlen(function)] = '\0';
		csource[strlen(source)] = '\0';

		entry.source = csource;
		entry.function = cfunction;
#endif
		entry.line = ar.currentline;

		++level;
	}

	const char* str = lua_tolstring(L, -1, NULL);
	if (str != NULL) // Setting a std::string to NULL causes a crash
	{
#ifdef WIN32
		lua_error->message = str;
#else
		char* newmessage = new char[strlen(str) + 1];
		V_strncpy(newmessage, str, strlen(str) + 1);
		newmessage[strlen(str)] = '\0';

		lua_error->message = newmessage;
#endif
	}

	CLuaInterface* LUA = (CLuaInterface*)L->luabase;
#ifdef WIN32
	lua_error->side = LUA->IsClient() ? "client" : (LUA->IsMenu() ? "menu" : "server");
#else
	const char* side = LUA->IsClient() ? "client" : (LUA->IsMenu() ? "menu" : "server");
	char* newside = new char[strlen(side) + 1];
	V_strncpy(newside, side, strlen(side) + 1);
	newside[strlen(side)] = '\0';

	lua_error->side = newside;
#endif

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
	CLuaInterface* LuaInterface = new CLuaInterface;

	return LuaInterface;
}

void CloseLuaInterface(ILuaInterface* LuaInterface)
{
	::DebugPrint(1, "CloseLuaInterface\n");
	// ToDo: Add all fancy operations and delete ILuaGameCallback? (Should we really delete it? No.)
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

		lua_pushvalue(L, LUA_REGISTRYINDEX); // Should we register the metatable?
			lua_getfield(L, -1, strName);
			if (lua_type(L, -1) == Type::None || lua_type(L, -1) == Type::Nil)
			{
				lua_pop(L, 1);
				lua_pushvalue(L, -2);
				lua_setfield(L, -2, strName);
			}
		lua_pop(L, 2);
	}
}

// =================================
// ILuaBase / CBaseLuaInterface implementation
// =================================

int CLuaInterface::Top()
{
	::DebugPrint(3, "CLuaInterface::Top\n");
	return lua_gettop(state);
}

void CLuaInterface::Push(int iStackPos)
{
	::DebugPrint(3, "CLuaInterface::Push %i\n", iStackPos);
	lua_pushvalue(state, iStackPos);
}

void CLuaInterface::Pop(int iAmt)
{
	::DebugPrint(3, "CLuaInterface::Pop %i\n", iAmt);
	lua_pop(state, iAmt);
	if (lua_gettop(state) < 0)
	{
#ifdef WIN32
		__debugbreak();
#endif
		::Error("CLuaInterface::Pop -> That was too much :<");
	}
}

void CLuaInterface::GetTable(int iStackPos)
{
	::DebugPrint(3, "CLuaInterface::GetTable %i\n", iStackPos);
	lua_gettable(state, iStackPos);
}

void CLuaInterface::GetField(int iStackPos, const char* strName)
{
	::DebugPrint(3, "CLuaInterface::GetField %i, %s\n", iStackPos, strName);
	lua_getfield(state, iStackPos, strName);
}

void CLuaInterface::SetField(int iStackPos, const char* strName)
{
	::DebugPrint(3, "CLuaInterface::SetField %i, %s\n", iStackPos, strName);
	lua_setfield(state, iStackPos, strName);
}

void CLuaInterface::CreateTable()
{
	::DebugPrint(3, "CLuaInterface::CreateTable\n");
	lua_createtable(state, 0, 0);
}

void CLuaInterface::SetTable(int iStackPos)
{
	::DebugPrint(3, "CLuaInterface::SetTable %i\n", iStackPos);
	lua_settable(state, iStackPos);
}

void CLuaInterface::SetMetaTable(int iStackPos)
{
	::DebugPrint(3, "CLuaInterface::SetMetaTable %i\n", iStackPos);
	lua_setmetatable(state, iStackPos);
}

bool CLuaInterface::GetMetaTable(int iStackPos)
{
	::DebugPrint(3, "CLuaInterface::GetMetaTable %i\n", iStackPos);
	return lua_getmetatable(state, iStackPos);
}

void CLuaInterface::Call(int iArgs, int iResults)
{
	::DebugPrint(3, "CLuaInterface::Call %i %i\n", iArgs, iResults);
	lua_call(state, iArgs, iResults);
}

int CLuaInterface::PCall(int iArgs, int iResults, int iErrorFunc)
{
	::DebugPrint(2, "CLuaInterface::PCall %i %i %i\n", iArgs, iResults, iErrorFunc);
	//return lua_pcall(state, iArgs, iResults, iErrorFunc);
	int ret = lua_pcall(state, iArgs, iResults, iErrorFunc);
	if ( ret != 0 )
		::DebugPrint(2, "CLuaInterface::PCall went boom. Oh nooo\n");

	return ret;
}

int CLuaInterface::Equal(int iA, int iB)
{
	::DebugPrint(3, "CLuaInterface::Equal %i %i\n", iA, iB);
	return lua_equal(state, iA, iB);
}

int CLuaInterface::RawEqual(int iA, int iB)
{
	::DebugPrint(3, "CLuaInterface::RawEqual %i %i\n", iA, iB);
	return lua_rawequal(state, iA, iB);
}

void CLuaInterface::Insert(int iStackPos)
{
	::DebugPrint(3, "CLuaInterface::Insert %i\n", iStackPos);
	lua_insert(state, iStackPos);
}

void CLuaInterface::Remove(int iStackPos)
{
	::DebugPrint(3, "CLuaInterface::Remove %i\n", iStackPos);
	lua_remove(state, iStackPos);
}

int CLuaInterface::Next(int iStackPos)
{
	::DebugPrint(3, "CLuaInterface::Next %i\n", iStackPos);
	return lua_next(state, iStackPos);
}

ILuaBase::UserData* CLuaInterface::NewUserdata(unsigned int iSize)
{
	::DebugPrint(3, "CLuaInterface::NewUserdata\n");
	return (UserData*)lua_newuserdata(state, iSize);
}

void CLuaInterface::ThrowError(const char* strError)
{
	::DebugPrint(2, "CLuaInterface::ThrowError %s\n", strError);
	luaL_error(state, "%s", strError);
}

void CLuaInterface::CheckType(int iStackPos, int iType)
{
	::DebugPrint(3, "CLuaInterface::CheckType %i %i\n", iStackPos, iType);
	int actualType = GetType(iStackPos);
	if (actualType != iType) {
		const char* expectedType = GetTypeName(iType);
		const char* actualTypeName = GetTypeName(actualType);
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

ILuaBase::UserData* CLuaInterface::GetUserdata(int iStackPos)
{
	::DebugPrint(4, "CLuaInterface::GetUserdata %i\n", iStackPos);
	return (ILuaBase::UserData*)lua_touserdata(state, iStackPos);
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
	lua_pushcclosure(state, val, 0);
}

void CLuaInterface::PushCClosure(CFunc val, int iVars)
{
	::DebugPrint(4, "CLuaInterface::PushCClosure\n");
	lua_pushcclosure(state, val, iVars);
}

void CLuaInterface::PushUserdata(ILuaBase::UserData* val)
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
	::DebugPrint(4, "CLuaInterface::ReferencePush pushed a %i\n", GetType(-1));
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
	::DebugPrint(4, "CLuaInterface::IsType %i %i\n", iStackPos, iType);
	int actualType = lua_type(state, iStackPos);
	bool isType = false;

	if (actualType == iType) {
		return true;
	}

	if (iType > 8)
	{
		ILuaBase::UserData* udata = GetUserdata(iStackPos);
		if (udata && udata->type == iType)
		{
			::DebugPrint(4, "CLuaInterface::IsType %i %i (%i)\n", iStackPos, iType, udata->type);
			return true;
		}
	}

	::DebugPrint(4, "CLuaInterface::IsType %i %i (%i)\n", iStackPos, iType, actualType);

	return false;
}

int CLuaInterface::GetType(int iStackPos)
{
	::DebugPrint(4, "CLuaInterface::GetType %i %i\n", iStackPos, lua_type(state, iStackPos));
	int type = lua_type(state, iStackPos);

	if (type == Type::UserData)
	{
		ILuaBase::UserData* udata = GetUserdata(iStackPos);
		if (udata)
		{
			type = udata->type;
		}
	}

	::DebugPrint(4, "CLuaInterface::GetType %i %i (%i)\n", iStackPos, lua_type(state, iStackPos), type);

	return type == -1 ? Type::Nil : type;
}

const char* CLuaInterface::GetTypeName(int iType)
{
	::DebugPrint(1, "CLuaInterface::GetTypeName %i\n", iType);
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
	::DebugPrint(4, "CLuaInterface::CheckString %i %s\n", iStackPos, luaL_checklstring(state, iStackPos, NULL));

	return luaL_checklstring(state, iStackPos, NULL);
}

double CLuaInterface::CheckNumber(int iStackPos)
{
	::DebugPrint(4, "CLuaInterface::CheckNumber %i\n", iStackPos);
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

const QAngle& CLuaInterface::GetAngle(int iStackPos)
{
	::DebugPrint(2, "CLuaInterface::GetAngle\n");
	ILuaBase::UserData* udata = GetUserdata(iStackPos);
	if (!udata || !udata->data || udata->type != Type::Angle)
		return QAngle(0, 0, 0);

	return *(QAngle*)udata->data;
}

const Vector& CLuaInterface::GetVector(int iStackPos)
{
	::DebugPrint(2, "CLuaInterface::GetVector\n");
	ILuaBase::UserData* udata = GetUserdata(iStackPos);
	if (!udata || !udata->data || udata->type != Type::Vector)
		return Vector(0, 0, 0);

	return *(Vector*)udata->data;
}

void CLuaInterface::PushAngle(const QAngle& val)
{
	::DebugPrint(2, "CLuaInterface::PushAngle\n");
	
	ILuaBase::UserData* udata = NewUserdata(20); // Should we use PushUserType?
	*(QAngle*)udata->data = val;
	udata->type = Type::Angle;

	if (PushMetaTable(Type::Angle))
		SetMetaTable(-2);
}

void CLuaInterface::PushVector(const Vector& val)
{
	::DebugPrint(2, "CLuaInterface::PushVector\n");

	ILuaBase::UserData* udata = NewUserdata(20);
	*(Vector*)udata->data = val;
	udata->type = Type::Vector;

	if (PushMetaTable(Type::Vector))
		SetMetaTable(-2);
}

void CLuaInterface::SetState(lua_State* L)
{
	::DebugPrint(3, "CLuaInterface::SetState\n");
	state = L;
}

int CLuaInterface::CreateMetaTable(const char* strName) // Return value is probably a bool?
{
	::DebugPrint(2, "CLuaInterface::CreateMetaTable\n");
	//luaL_newmetatable_type(state, strName, -1);

	int ref = -1;
	PushSpecial(SPECIAL_REG);
		GetField(-1, strName);
		if (IsType(-1, Type::Table))
		{
			ref = ReferenceCreate();
		} else {
			Pop(1);
		}
	Pop(1);

	if (ref != -1)
	{
		ReferencePush(ref);
		ReferenceFree(ref);
		return 1;
	}

	return 0;
}

bool CLuaInterface::PushMetaTable(int iType)
{
	::DebugPrint(2, "CLuaInterface::PushMetaTable %i\n", iType);
	int ref = -1;
	const char* type = GetActualTypeName(iType);
	PushSpecial(SPECIAL_REG);
		GetField(-1, type);
		if (IsType(-1, Type::Table))
		{
			ref = ReferenceCreate();
		} else {
			Pop(1);
		}
	Pop(1);

	if (ref != -1)
	{
		ReferencePush(ref);
		ReferenceFree(ref);
		return true;
	}

	::Msg("I failed u :< %i\n", iType);

	return false;
}

void CLuaInterface::PushUserType(void* data, int iType)
{
	::DebugPrint(2, "CLuaInterface::PushUserType %i\n", iType);

	UserData* udata = NewUserdata(8);
	udata->data = data;
	udata->type = iType;

	if (PushMetaTable(iType))
	{
		SetMetaTable(-2);
	} else {
		::Msg("Failed to find Metatable for %i!\n", iType);
	}
}

void CLuaInterface::SetUserType(int iStackPos, void* data)
{
	::DebugPrint(2, "CLuaInterface::SetUserType\n");
	
	ILuaBase::UserData* udata = GetUserdata(iStackPos);
	if (udata)
		udata->data = data;
}

// =================================
// ILuaInterface implementations
// =================================

int LuaPanic(lua_State* lua)
{
	::DebugPrint(1, "CLuaInterface::LuaPanic\n");

	Error("Lua Panic! Something went horribly wrong!\n%s", lua_tolstring(lua, -1, 0));
	return 0;
}

bool CLuaInterface::Init( ILuaGameCallback* callback, bool bIsServer )
{
	::DebugPrint(1, "CLuaInterface::Init Server: %s\n", bIsServer ? "Yes" : "No");
	m_pGameCallback = callback;

	state = luaL_newstate();
	luaL_openlibs(state);

	state->luabase = this;
	SetState(state);

	for (int i=0; i<LUA_MAX_TEMP_OBJECTS;++i)
	{
		m_TempObjects[i] = CreateObject();
	}

	lua_atpanic(state, LuaPanic);

	lua_pushcclosure(state, AdvancedLuaErrorReporter, 0);
	lua_setglobal(state, "AdvancedLuaErrorReporter");

	{
		luaJIT_setmode(state, -1, LUAJIT_MODE_WRAPCFUNC|LUAJIT_MODE_ON);

		// ToDo: Find out how to check the FPU precision
		// Warning("Lua detected bad FPU precision! Prepare for weirdness!");
	}

	int reference = -1;
	PushSpecial(SPECIAL_GLOB);
		GetField(-1, "require"); // Keep the original require function
		if (IsType(-1, Type::Function))
		{
			reference = ReferenceCreate();
		} else {
			Pop(1);
		}
	Pop(1);

	DoStackCheck();

	NewGlobalTable("");

	if (reference != -1)
	{
		ReferencePush(reference);
		SetMember(Global(), "requiree");
		ReferenceFree(reference);
	}

	::DebugPrint(3, "Table? %s\n", m_pGlobal->isTable() ? "Yes" : "No");

	DoStackCheck();

	lua_createtable(state, 0, 0);
	
	int idx = 0;
	for(const char* str : g_PooledStrings)
	{
		++idx;
		PushNumber(idx);
		PushString(str);
		SetTable(-3);
	}

	m_pStringPool = CreateObject();
	m_pStringPool->SetFromStack(-1);
	Pop(1);

	DoStackCheck();

	// lua_getfield(state, "debug");

	// lua_pushnil(state);
	// lua_setfield(state, -2, "setlocal");

	// lua_pushnil(state);
	// lua_setfield(state, -2, "setupvalue");

	// lua_pushnil(state);
	// lua_setfield(state, -2, "upvalueid");

	// lua_pushnil(state);
	// lua_setfield(state, -2, "upvaluejoin");

	// lua_pop(state, 1);

	return true;
}

void CLuaInterface::Shutdown()
{
	::DebugPrint(1, "CLuaInterface::Shutdown\n");
	GMOD_UnloadBinaryModules(state);

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
	for (ILuaThreadedCall* call : m_pThreadedCalls)
	{
		call->Init();
	}

	for (ILuaThreadedCall* call : m_pThreadedCalls)
	{
		call->Run(this);
	}

	m_pThreadedCalls.clear();
}

int CLuaInterface::AddThreadedCall(ILuaThreadedCall* call)
{
	::DebugPrint(1, "CLuaInterface::AddThreadedCall What called this?\n");

	m_pThreadedCalls.push_back(call);

	return m_pThreadedCalls.size();
}

ILuaObject* CLuaInterface::Global()
{
	::DebugPrint(4, "CLuaInterface::Global\n");
	return m_pGlobal;
}

ILuaObject* CLuaInterface::GetObject(int index)
{
	::DebugPrint(4, "CLuaInterface::GetObject\n");
	ILuaObject* obj = CreateObject();
	obj->SetFromStack(index);

	return obj;
}

void CLuaInterface::PushLuaObject(ILuaObject* obj)
{
	::DebugPrint(4, "CLuaInterface::PushLuaObject\n");
	if (obj)
	{
		ReferencePush(obj->m_reference);
	} else {
		PushNil();
	}
}

void CLuaInterface::PushLuaFunction(CFunc func)
{
	::DebugPrint(4, "CLuaInterface::PushLuaFunction\n");
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
	::DebugPrint(2, "CLuaInterface::TypeError %s %i\n", str, iStackPos);
	luaL_typerror(state, iStackPos, str);
}

void CLuaInterface::CallInternal(int args, int rets)
{
	::DebugPrint(2, "CLuaInterface::CallInternal %i %i\n", args, rets);
	if (!ThreadInMainThread())
		Error("Calling Lua function in a thread other than main!");

	if (rets > 4)
		Error("[CLuaInterface::Call] Expecting more returns than possible\n");

	for (int i=0; i<3; ++i)
	{
		m_ProtectedFunctionReturns[i] = nullptr;
	}

	if (IsType(-(args + 1), Type::Function))
	{
		if (CallFunctionProtected(args, rets, true))
		{
			for (int i=0; i<rets; ++i)
			{
				ILuaObject* obj = NewTemporaryObject();
				obj->SetFromStack(-1);
				m_ProtectedFunctionReturns[i] = obj;
				Pop(1);
			}
		}
	} else {
		Error("Lua tried to call non functions");
	}
}

void CLuaInterface::CallInternalNoReturns(int args)
{
	::DebugPrint(2, "CLuaInterface::CallInternalNoReturns %i\n", args);
	
	CallFunctionProtected(args, 0, true);
}

bool CLuaInterface::CallInternalGetBool(int args)
{
	::DebugPrint(2, "CLuaInterface::CallInternalGetBool %i\n", args);
	
	bool ret = false;
	if (CallFunctionProtected(args, 1, 1)) {
		ret = GetBool(-1);
		Pop(1);
	}

	return ret;
}

const char* CLuaInterface::CallInternalGetString(int args)
{
	::DebugPrint(2, "CLuaInterface::CallInternalGetString %i\n", args);

	const char* ret = NULL;
	if (CallFunctionProtected(args, 1, 1)) {
		ret = GetString(-1);
		Pop(1);
	}

	return ret;
}

bool CLuaInterface::CallInternalGet(int args, ILuaObject* obj)
{
	::DebugPrint(2, "CLuaInterface::CallInternalGet %i\n", args);
	if (CallFunctionProtected(args, 1, 1)) {
		obj->SetFromStack(-1);
		Pop(1);
		return true;
	}

	return false;
}

void CLuaInterface::NewGlobalTable(const char* name)
{
	::DebugPrint(1, "CLuaInterface::NewGlobalTable %s\n", name);

	//CreateTable();
	PushSpecial(SPECIAL_GLOB);
	m_pGlobal = CreateObject();
	m_pGlobal->SetFromStack(-1);
	Pop(1);
	//SetField(LUA_GLOBALSINDEX, name);
}

ILuaObject* CLuaInterface::NewTemporaryObject()
{
	::DebugPrint(2, "CLuaInterface::NewTemporaryObject\n");

	++m_iCurrentTempObject;
	if (m_iCurrentTempObject >= LUA_MAX_TEMP_OBJECTS)
		m_iCurrentTempObject = 0;

	ILuaObject* obj = m_TempObjects[m_iCurrentTempObject];
	if (obj)
	{
		obj->UnReference();
	} else {
		obj = CreateObject();
		m_TempObjects[m_iCurrentTempObject] = obj;
	}

	return obj;
}

bool CLuaInterface::isUserData(int iStackPos)
{
	::DebugPrint(4, "CLuaInterface::isUserData %i %s\n", iStackPos, lua_type(state, iStackPos) == Type::UserData ? "Yes" : "No");

	return lua_type(state, iStackPos) == Type::UserData;
}

ILuaObject* CLuaInterface::GetMetaTableObject(const char* name, int type)
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

	ILuaObject* obj = NewTemporaryObject();
	obj->SetFromStack(-1);
	Pop(2);

	return obj;
}

ILuaObject* CLuaInterface::GetMetaTableObject(int iStackPos)
{
	::DebugPrint(2, "CLuaInterface::GetMetaTableObject\n");

	ILuaObject* obj = NewTemporaryObject();
	obj->SetFromStack(-1);
	Pop(1);

	return obj;
}

ILuaObject* CLuaInterface::GetReturn(int iStackPos)
{
	::DebugPrint(2, "CLuaInterface::GetReturn\n");
	
	int idx = abs(iStackPos);
	if (idx >= 0 && idx < 4)
	{
		if ( m_ProtectedFunctionReturns[idx] == NULL)
		{
			::DebugPrint(1, "CLuaInterface::GetReturn We could crash! (null object!)\n");
#ifdef WIN32
			__debugbreak();
#endif
		}

		return m_ProtectedFunctionReturns[idx];
	}

	::DebugPrint(1, "CLuaInterface::GetReturn We could crash! (invalid idx!)\n");
#ifdef WIN32
	__debugbreak();
#endif
	return nullptr;
}

bool CLuaInterface::IsServer()
{
	::DebugPrint(3, "CLuaInterface::IsServer\n");

	return m_iRealm == State::SERVER;
}

bool CLuaInterface::IsClient()
{
	::DebugPrint(3, "CLuaInterface::IsClient\n");

	return m_iRealm == State::CLIENT;
}

bool CLuaInterface::IsMenu()
{
	::DebugPrint(3, "CLuaInterface::IsMenu\n");

	return m_iRealm == State::MENU;
}

void CLuaInterface::DestroyObject(ILuaObject* obj)
{
	::DebugPrint(4, "CLuaInterface::DestroyObject\n");
	m_pGameCallback->DestroyLuaObject(obj);
}

ILuaObject* CLuaInterface::CreateObject()
{
	::DebugPrint(4, "CLuaInterface::CreateObject\n");
	return m_pGameCallback->CreateLuaObject();
}

void CLuaInterface::SetMember(ILuaObject* obj, ILuaObject* key, ILuaObject* value)
{
	::DebugPrint(3, "CLuaInterface::SetMember 1\n");
	if (obj->isTable() || obj->GetType() == Type::Table)
	{
		ReferencePush(obj->m_reference);
		if (!IsType(-1, Type::Table))
		{
			Warning("CLuaInterface::SetMember1 Stopped an error!\n");
			Pop(1);
			return;
		}

		ReferencePush(key->m_reference);
		ReferencePush(value->m_reference);
		SetTable(-3);
		Pop(1);
	}
}

ILuaObject* CLuaInterface::GetNewTable()
{
	::DebugPrint(2, "CLuaInterface::GetNewTable\n");
	
	CreateTable();
	ILuaObject* obj = CreateObject();
	obj->SetFromStack(-1);
	return obj;
}

void CLuaInterface::SetMember(ILuaObject* obj, float key)
{
	::DebugPrint(3, "CLuaInterface::SetMember 2\n");
	if (obj->isTable() || obj->GetType() == Type::Table)
	{
		ReferencePush(obj->m_reference);
		if (!IsType(-1, Type::Table))
		{
			Warning("CLuaInterface::SetMember2 Stopped an error! %f\n", key);
			Pop(1);
			return;
		}

		PushNumber(key);
		Push(-3);
		SetTable(-3);
		Pop(2);
	}
}

void CLuaInterface::SetMember(ILuaObject* obj, float key, ILuaObject* value)
{
	::DebugPrint(3, "CLuaInterface::SetMember 3 %f\n", key);
	if (obj->isTable() || obj->GetType() == Type::Table)
	{
		ReferencePush(obj->m_reference);
		if (!IsType(-1, Type::Table))
		{
			Warning("CLuaInterface::SetMember3 Stopped an error! %f\n", key);
			Pop(1);
			return;
		}

		PushNumber(key);
		ReferencePush(value->m_reference);
		SetTable(-3);
		Pop(1);
	}
}

void CLuaInterface::SetMember(ILuaObject* obj, const char* key)
{
	::DebugPrint(3, "CLuaInterface::SetMember 4 %s\n", key);
	if (obj->isTable() || obj->GetType() == Type::Table)
	{
		ReferencePush(obj->m_reference);
		if (!IsType(-1, Type::Table))
		{
			Warning("CLuaInterface::SetMember4 Stopped an error! %s\n", key);
			Pop(1);
			return;
		}

		PushString(key);
		Push(-3);
		SetTable(-3);
		Pop(2);
	}
}

void CLuaInterface::SetMember(ILuaObject* obj, const char* key, ILuaObject* value)
{
	::DebugPrint(3, "CLuaInterface::SetMember 5 %s\n", key);
	if (obj->isTable() || obj->GetType() == Type::Table)
	{
		ReferencePush(obj->m_reference);
		if (!IsType(-1, Type::Table))
		{
			Warning("CLuaInterface::SetMember5 Stopped an error! %s\n", key);
			Pop(1);
			return;
		}

		PushString(key);
		if (value)
		{
			ReferencePush(value->m_reference);
		} else {
			PushNil();
		}
		SetTable(-3);
		Pop(1);
	}
}

void CLuaInterface::SetType(unsigned char realm)
{
	::DebugPrint(1, "CLuaInterface::SetType %u\n", realm);
	m_iRealm = realm;
}

void CLuaInterface::PushLong(long number)
{
	::DebugPrint(4, "CLuaInterface::PushLong\n");
	lua_pushnumber(state, number);
}

int CLuaInterface::GetFlags(int iStackPos)
{
	::DebugPrint(1, "CLuaInterface::GetFlags %i %i\n", iStackPos, GetType(iStackPos));

	return (int)GetNumber(iStackPos); // ToDo: Verify this
}

bool CLuaInterface::FindOnObjectsMetaTable(int iStackPos, int keyIndex)
{
	::DebugPrint(2, "CLuaInterface::FindOnObjectsMetaTable %i %i %s\n", iStackPos, keyIndex, lua_tolstring(state, keyIndex, NULL));

	if (lua_getmetatable(state, iStackPos) == 1)
	{
		lua_pushvalue(state, keyIndex);
		lua_gettable(state, -2);
		if (lua_type(state, -1) == Type::Nil)
		{
			Pop(2);
		} else {
			Remove(-2);
			::DebugPrint(2, "CLuaInterface::FindOnObjectsMetaTable GOOD\n");
			return true;
		}
	}

	::DebugPrint(2, "CLuaInterface::FindOnObjectsMetaTable BAD\n");
	return false;
}

bool CLuaInterface::FindObjectOnTable(int iStackPos, int keyIndex)
{
	::DebugPrint(2, "CLuaInterface::FindObjectOnTable\n");
	if (IsType(iStackPos, Type::Table))
	{
		lua_pushvalue(state, iStackPos);
		lua_pushvalue(state, keyIndex);
		lua_gettable(state, -2);
		lua_remove(state, -2);
		if (GetType(-1) != Type::Nil)
		{
			return true;
		} else {
			lua_remove(state, -1);
		}
	} else {
		::DebugPrint(2, "CLuaInterface::FindObjectOnTable NOT A TABLE!\n");
	}

	return false;
}

void CLuaInterface::SetMemberFast(ILuaObject* obj, int keyIndex, int valueIndex)
{
	::DebugPrint(3, "CLuaInterface::SetMemberFast %i %i %s\n", keyIndex, valueIndex, GetType(keyIndex) == Type::String ? GetString(keyIndex) : "");
	if (obj->isTable() || obj->GetType() == Type::Table)
	{
		ReferencePush(obj->m_reference);
		Push(keyIndex);
		Push(valueIndex);
		SetTable(-3);
		Pop(1);
	}
}

bool CLuaInterface::RunString(const char* filename, const char* path, const char* stringToRun, bool run, bool showErrors)
{
	::DebugPrint(2, "CLuaInterface::RunString\n");
	return RunStringEx(filename, path, stringToRun, run, showErrors, true, true);
}

bool CLuaInterface::IsEqual(ILuaObject* objA, ILuaObject* objB)
{
	::DebugPrint(2, "CLuaInterface::IsEqual\n");
	
	ReferencePush(objA->m_reference);
	ReferencePush(objB->m_reference);
	bool ret = Equal(-1, -2);
	Pop(2);

	return ret;
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

std::string ToPath(std::string path)
{
	size_t lastSeparatorPos = path.find_last_of("/\\");

	if (lastSeparatorPos != std::string::npos) {
		return path.substr(0, lastSeparatorPos + 1);
	}

	return path;
}

bool CLuaInterface::FindAndRunScript(const char *filename, bool run, bool showErrors, const char *stringToRun, bool noReturns)
{
	::DebugPrint(2, "CLuaInterface::FindAndRunScript %s, %s, %s, %s, %s\n", filename, run ? "Yes" : "No", showErrors ? "Yes" : "No", stringToRun, noReturns ? "Yes" : "No");

	//if (true)
	//	return false;

	bool bDataTable = ((std::string)filename).rfind("!lua", 0) == 0;
	ILuaShared* shared = LuaShared();
	LuaFile* file = shared->LoadFile(filename, m_sPathID, bDataTable, true);
	bool ret = false;
	if (file)
	{
		PushPath(ToPath(file->name).c_str());
#ifdef WIN32
		ret = RunStringEx(file->name.c_str(), file->source.c_str(), file->contents.c_str(), true, showErrors, true, noReturns);
#else
		ret = RunStringEx(file->name, file->source, file->contents, true, showErrors, true, noReturns);
#endif
		PopPath();
	} else {
		std::string out;
		GetCurrentFile(out);
		if (out != "!UNKNOWN") // ToDo: Fix this mess. This will probably kill performance / loading times.
		{
			file = shared->LoadFile(ToPath(out.c_str()) + filename, m_sPathID, bDataTable, true);
			if (file)
			{
				PushPath(ToPath(file->name).c_str());
#ifdef WIN32
				ret = RunStringEx(file->name.c_str(), file->source.c_str(), file->contents.c_str(), true, showErrors, true, noReturns);
#else
				ret = RunStringEx(file->name, file->source, file->contents, true, showErrors, true, noReturns);
#endif
				PopPath();
			} else {
				lua_Debug ar;
				lua_getstack(state, 2, &ar); // Going deeper.
				lua_getinfo(state, "S", &ar);

				out = ar.source ? ar.source : "!UNKNOWN";
				if (out != "!UNKNOWN")
				{
					file = shared->LoadFile(ToPath(out.c_str()) + filename, m_sPathID, bDataTable, true);
					if (file)
					{
						PushPath(ToPath(file->name).c_str());
#ifdef WIN32
						ret = RunStringEx(file->name.c_str(), file->source.c_str(), file->contents.c_str(), true, showErrors, true, noReturns);
#else
						ret = RunStringEx(file->name, file->source, file->contents, true, showErrors, true, noReturns);
#endif
						PopPath();
					} else {
						if (strcmp(stringToRun, "") != 0)
						{
							file = shared->LoadFile(ToPath(stringToRun) + filename, m_sPathID, bDataTable, true);
							if (file)
							{
								PushPath(ToPath(file->name).c_str());
#ifdef WIN32
								ret = RunStringEx(file->name.c_str(), file->source.c_str(), file->contents.c_str(), true, showErrors, true, noReturns);
#else
								ret = RunStringEx(file->name, file->source, file->contents, true, showErrors, true, noReturns);
#endif
								PopPath();
							}
						}
					}
				}
			}
		}
	}

	if ( !file )
	{
		::DebugPrint( 1, "Failed to find Script %s!\n", filename );
#ifdef WIN32
		//__debugbreak();
#endif
	}

	return ret;
}

void CLuaInterface::SetPathID(const char* pathID)
{
	::DebugPrint(1, "CLuaInterface::SetPathID %s\n", pathID);
	V_strncpy(m_sPathID, pathID, sizeof(m_sPathID));
}

const char* CLuaInterface::GetPathID()
{
	::DebugPrint(2, "CLuaInterface::GetPathID\n");

	return m_sPathID;
}

void CLuaInterface::ErrorNoHalt( const char* fmt, ... )
{
	::DebugPrint(2, "CLuaInterface::ErrorNoHalt %s\n", fmt);

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
	buffer[size] = '\0';

#ifndef WIN32
	if (error->message)
		delete[] error->message;
#endif

	error->message = buffer;
	va_end(args);

#ifdef WIN32
		m_pGameCallback->LuaError(error);
#else
		Msg("An error ocurred! %s\n", error->message);
#endif

#ifdef WIN32
	delete[] buffer;
#endif
	//delete error; // Deconstuctor will delete our buffer
}

void CLuaInterface::Msg( const char* fmt, ... )
{
	::DebugPrint(2, "CLuaInterface::Msg %s\n", fmt);

	va_list args;
	va_start(args, fmt);

	char* buffer = new char[4096];
	V_vsnprintf(buffer, 4096, fmt, args);

	va_end(args);

	m_pGameCallback->Msg(buffer, false);

	delete[] buffer;
}

void CLuaInterface::PushPath( const char* path )
{
	::DebugPrint(2, "CLuaInterface::PushPath %s\n", path);
	char* str = new char[strlen(path)];
	V_strncpy( str, path, strlen(path) );
	str[ strlen(path) - 1 ] = '\0'; // nuke the last /
	m_sCurrentPath = str;
	m_pPaths.push_back( str );
	++m_iPushedPaths;
}

void CLuaInterface::PopPath()
{
	::DebugPrint(2, "CLuaInterface::PopPath\n");
	char* str = m_pPaths.back();
	delete[] str;
	m_pPaths.pop_back();

	--m_iPushedPaths;
	if ( m_iPushedPaths > 0 )
		m_sCurrentPath = m_pPaths.back();
	else
		m_sCurrentPath = NULL;
}

const char* CLuaInterface::GetPath()
{
	::DebugPrint(2, "CLuaInterface::GetPath\n");

	if ( m_iPushedPaths <= 0 )
		return NULL;

	::DebugPrint(2, "CLuaInterface::GetPath %s\n", (const char*)m_pPaths.back());
	return m_pPaths.back();
}

int CLuaInterface::GetColor(int iStackPos) // Probably returns the StackPos
{
	::DebugPrint(2, "CLuaInterface::GetColor\n");

	return 0;
}

void CLuaInterface::PushColor(Color color)
{
	::DebugPrint(2, "CLuaInterface::PushColor\n");
	
	CreateTable();
		PushNumber( color.r() );
		SetField( -2, "r" );

		PushNumber( color.g() );
		SetField( -2, "g" );

		PushNumber( color.b() );
		SetField( -2, "b" );

		PushNumber( color.a() );
		SetField( -2, "a" );

	if ( CreateMetaTable( "Color" ) == 1 )
		SetMetaTable( -1 );
}

int CLuaInterface::GetStack(int level, lua_Debug* dbg)
{
	::DebugPrint(2, "CLuaInterface::GetStack %i\n", level);

	return lua_getstack(state, level, dbg);
}

int CLuaInterface::GetInfo(const char* what, lua_Debug* dbg)
{
	::DebugPrint(2, "CLuaInterface::GetInfo %s\n", what);

	return lua_getinfo(state, what, dbg);
}

const char* CLuaInterface::GetLocal(lua_Debug* dbg, int n)
{
	::DebugPrint(2, "CLuaInterface::GetLocal %i\n", n);

	return lua_getlocal(state, dbg, n);
}

const char* CLuaInterface::GetUpvalue(int funcIndex, int n)
{
	::DebugPrint(2, "CLuaInterface::GetUpvalue %i %i\n", funcIndex, n);

	return lua_getupvalue(state, funcIndex, n);
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
#ifdef WIN32
			m_pGameCallback->LuaError(err);
#else
			Msg("An error ocurred! %s\n", err->message);
#endif

		//delete err;

		return false;
	} else {
		return CallFunctionProtected(0, 0, printErrors);
	}
}

size_t CLuaInterface::GetDataString(int index, const char **str)
{
	::DebugPrint(2, "CLuaInterface::GetDataString\n");
	// ToDo

	Error("CLuaInterface::GetDataString is not implemented!\n");

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
	buffer[size] = '\0';

#ifndef WIN32
	if (error->message)
		delete[] error->message;
#endif

	error->message = buffer;
	va_end(args);
	
	/* NOTE: This is already done in ReadStackIntoError
	
	const char* realm;
	switch(m_iRealm)
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
	error->side = realm;*/

#ifndef WIN32
	Msg("Error Message: %s\n", error->message);
	Msg("Error Side: %s\n", error->side);

	for ( CLuaError::StackEntry entry : error->stack )
	{
		Msg("Error Stack function: %s\n", entry.function);
		Msg("Error Stack source: %s\n", entry.source);
		Msg("Error Stack line: %i\n", entry.line);
	}
#endif

#ifdef WIN32
			m_pGameCallback->LuaError(error);
#else
			Msg("An error ocurred! %s\n", error->message);
#endif

#ifdef WIN32
	delete[] buffer;
#endif
	//delete error; // Deconstuctor will delete our buffer
}

const char* CLuaInterface::GetCurrentLocation()
{
	::DebugPrint(2, "CLuaInterface::GetCurrentLocation\n");
	
	lua_Debug ar;
	lua_getstack(state, 1, &ar);
	lua_getinfo(state, "Sl", &ar);
	if (ar.source && strcmp(ar.what, "C") != 0)
	{
		static char strOutput[511];
		V_snprintf( strOutput, 511, "%s (line %i)", ar.source, ar.currentline );

		::DebugPrint(2, "CLuaInterface::GetCurrentLocation %s\n", strOutput);
		return strOutput;
	}

	return "<nowhere>";
}

void CLuaInterface::MsgColour(const Color& col, const char* fmt, ...)
{
	::DebugPrint(2, "CLuaInterface::MsgColour\n");

	va_list args;
	va_start(args, fmt);

	int size = vsnprintf(NULL, 0, fmt, args);
	if (size < 0) {
		va_end(args);
		return;
	}

	char* buffer = new char[size + 1];
	vsnprintf(buffer, size + 1, fmt, args);

	m_pGameCallback->MsgColour(buffer, col);

	delete[] buffer;
	va_end(args);
}

void CLuaInterface::GetCurrentFile(std::string &outStr)
{
	::DebugPrint(2, "CLuaInterface::GetCurrentFile\n");

	lua_Debug ar;
	int level = 0;
	while (lua_getstack(state, level, &ar) != 0)
	{
		lua_getinfo(state, "S", &ar);
		if (ar.source && strcmp(ar.what, "C") != 0)
		{
			outStr.assign(ar.source);
			::DebugPrint(2, "CLuaInterface::GetCurrentFile %s\n", ar.source);
			return;
		}
		++level;
	}

	outStr = "!UNKNOWN";
	::DebugPrint(2, "CLuaInterface::GetCurrentFile %s\n", "!UNKNOWN (How dare you)");
}

void CLuaInterface::CompileString(Bootil::Buffer& dumper, const std::string& stringToCompile)
{
	::DebugPrint(2, "CLuaInterface::CompileString\n");
	// ToDo

	Error("CLuaInterface::CompileString is not implemented!");
}

bool CLuaInterface::CallFunctionProtected(int iArgs, int iRets, bool showError)
{
	::DebugPrint(2, "CLuaInterface::CallFunctionProtected %i %i %s\n", iArgs, iRets, showError ? "Yes" : "no");

	/*for (int i=1;i <= Top(); ++i)
	{
		::DebugPrint(4, "Stack: %i, Type: %i\n", i, GetType(i));
	}*/

	if (GetType(-(iArgs + 1)) != Type::Function)
	{
		Warning("[CLuaInterface::CallFunctionProtected] You betraid me. This is not a function :<\n");
		return false;
	}

	int ret = PCall(iArgs, iRets, 0);
	if (ret != 0)
	{
		CLuaError* err = ReadStackIntoError(state);
		if (showError)
		{
#ifdef WIN32
			m_pGameCallback->LuaError(err);
#else
			Msg("An error ocurred! %s\n", err->message);
#endif
		}
		//delete err;
		Pop(1);
	}

	return ret == 0;
}

void CLuaInterface::Require(const char* cname)
{
	::DebugPrint(2, "CLuaInterface::Require %s\n", cname);

	std::string name = cname;
	name = (IsClient() ? "gmcl_" : "gmsv_") + name + "_";

#ifdef SYSTEM_MACOSX
	name = name + "osx";
#else
	#ifdef SYSTEM_WINDOWS
		name = name + "win";
	#else
		name = name + "linux";
	#endif

	#ifdef ARCHITECTURE_X86_64
		name = name + "64";
	#else
		#ifdef SYSTEM_WINDOWS
			name = name + "32";
		#endif
	#endif
#endif
	name = name + ".dll";


	std::string path = (std::string)"lua/bin/" + name;
	if (g_pFullFileSystem->FileExists(path.c_str(), "MOD"))
	{
		char dllpath[512];
		g_pFullFileSystem->RelativePathToFullPath(path.c_str(), "MOD", dllpath, sizeof(dllpath));
		GMOD_LoadBinaryModule(state, dllpath);
		//delete[] dllpath; //NOTE: Delete this and it will cause a critical error? probably because the value is still in use by Lua
	} else {
		RunLuaModule(cname);
	}
}

const char* CLuaInterface::GetActualTypeName(int type)
{
	::DebugPrint(4, "CLuaInterface::GetActualTypeName\n");

	//lua_typename(state, lua_type(state, type));

	return Type::Name[type];
}

void CLuaInterface::PreCreateTable(int arrelems, int nonarrelems)
{
	::DebugPrint(4, "CLuaInterface::PreCreateTable %i %i\n", arrelems, nonarrelems);
	lua_createtable(state, arrelems, nonarrelems);
}

void CLuaInterface::PushPooledString(int index)
{
	::DebugPrint(2, "CLuaInterface::PushPooledString %i %s\n", index, g_PooledStrings[index]);
	
	ReferencePush(m_pStringPool->m_reference);
	PushNumber(index+1); // LUA starts at 1 so we add 1
	GetTable(-2);
	Remove(-2);

	::DebugPrint(2, "CLuaInterface::PushPooledString %i %s %s\n", index, g_PooledStrings[index], GetString(-1));
}

const char* CLuaInterface::GetPooledString(int index)
{
	::DebugPrint(2, "CLuaInterface::GetPooledString\n");

	return g_PooledStrings[index];
}

void CLuaInterface::AppendStackTrace(char *, unsigned long)
{
	::DebugPrint(2, "CLuaInterface::AppendStackTrace\n");
	// ToDo

	Error("CLuaInterface::AppendStackTrace is not implemented!");
}

void* CLuaInterface::CreateConVar(const char* name, const char* defaultValue, const char* helpString, int flags)
{
	::DebugPrint(2, "CLuaInterface::CreateConVar\n");

	if ( IsServer() )
		flags |= FCVAR_LUA_SERVER;
	else
		flags |= FCVAR_LUA_CLIENT;

	return LuaConVars()->CreateConVar(name, defaultValue, helpString, flags);
}

void* CLuaInterface::CreateConCommand(const char* name, const char* helpString, int flags, FnCommandCallback_t callback, FnCommandCompletionCallback completionFunc)
{
	::DebugPrint(2, "CLuaInterface::CreateConCommand\n");

	return LuaConVars()->CreateConCommand(name, helpString, flags, callback, completionFunc);
}

const char* CLuaInterface::CheckStringOpt( int iStackPos, const char* def )
{
	::DebugPrint(4, "CLuaInterface::CheckStringOpt %i %s\n", iStackPos, def);

	return luaL_optlstring(state, iStackPos, def, NULL);
}

double CLuaInterface::CheckNumberOpt( int iStackPos, double def )
{
	::DebugPrint(4, "CLuaInterface::CheckNumberOpt %i %f\n", iStackPos, def);

	return luaL_optnumber(state, iStackPos, def);
}

std::string CLuaInterface::RunMacros(std::string code)
{
	::DebugPrint(2, "CLuaInterface::RunMacros\n");

	code = std::regex_replace(code, std::regex("DEFINE_BASECLASS"), "local BaseClass = baseclass.Get");

	return code;
}

void CLuaInterface::RegisterMetaTable( const char* name, ILuaObject* obj )
{
	PushSpecial(SPECIAL_REG);
		GetField(-1, name);
		if (IsType(-1, Type::Nil)) // Do we allow one to override the entire table? Probably not.
		{
			ReferencePush(obj->m_reference);

			PushString("MetaName");
			PushString(name);
			SetTable(-3);

			PushString("MetaID");
			PushNumber(-1); // ToDo
			SetTable(-3);

			SetField(-3, name);
			Pop(1);
		}
	Pop(2);
}

void CLuaInterface::LuaPrint( const char* str ) // Unofficial function to get GMOD_LuaPrint to work without crashing.
{
	m_pGameCallback->Msg( str, false );
}