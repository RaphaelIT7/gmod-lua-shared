#include "CLuaInterface.h"

#ifdef WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

void GMOD_LoadBinaryModule(lua_State* L, const char* name)
{
	lua_pushfstring(L, "LOADLIB: %s", name);
	ILuaBase::UserData* udata = (ILuaBase::UserData*)lua_newuserdata(L, sizeof(ILuaBase::UserData));
	udata->data = nullptr;
	udata->type = Type::UserData;

	lua_pushvalue(L, LUA_REGISTRYINDEX);
	lua_getfield(L, -1, "_LOADLIB");
	lua_setmetatable(L, -3);

	lua_pushvalue(L, -3);
	lua_pushvalue(L, -3);
	lua_settable(L, -3);

#ifdef WIN32
	HMODULE hDll = LoadLibrary(name);
	if (hDll == NULL) {
		lua_pushliteral(L, "Failed to load dll!");
		lua_error(L);
		return;
	}

	CFunc gmod13_open = (CFunc)GetProcAddress(hDll, "gmod13_open");
	if (gmod13_open == NULL) {
		lua_pushliteral(L, "Failed to get gmod13_open!");
		lua_error(L);
		return;
	}

	lua_pushcclosure(L, gmod13_open, 0);
	lua_call(L, 0, 0);

	FreeLibrary(hDll);
#else
	void* handle = dlopen(name, RTLD_LAZY);
	if (!handle) {
		lua_pushliteral(L, "Failed to load dll!");
		lua_error(L);
		return;
	}

	CFunc gmod13_open = (CFunc)dlsym(handle, "gmod13_open");
	if (!gmod13_open) {
		lua_pushliteral(L, "Failed to get gmod13_open!");
		lua_error(L);
		return;
	}

	lua_pushcclosure(L, gmod13_open, 0);
	lua_call(L, 0, 0);

	dlclose(handle);
#endif
}