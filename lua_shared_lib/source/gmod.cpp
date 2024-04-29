#include "CLuaInterface.h"
#include "Platform"

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
	lua_pop(L, 1);

#ifdef SYSTEM_WINDOWS
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
		FreeLibrary(hDll);
		return;
	}

	//udata->data = hDll;

	lua_pushcclosure(L, gmod13_open, 0);
	lua_call(L, 0, 0);
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

	udata->data = handle;

	lua_pushcclosure(L, gmod13_open, 0);
	lua_call(L, 0, 0);
#endif
}

void GMOD_UnloadBinaryModules(lua_State* L)
{
	lua_pushvalue(L, LUA_REGISTRYINDEX);
	lua_pushnil(L);

	bool isModule = false;
	while (lua_next(L, -2) != 0)
	{
		lua_getmetatable(L, -2);
		if (lua_type(L, -1) == Type::Table)
		{
			lua_getfield(L, -1, "MetaName");
			if (lua_type(L, -1) == Type::String)
			{
				const char* type = lua_tostring(L, -1);
				if (strcmp(type, "_LOADLIB") == 0)
				{
					isModule = true;
				}
			}
			lua_pop(L, 1);
		}

		lua_pop(L, 1);
		if (isModule)
		{
			Msg("Unloading %s\n", lua_tostring(L, -2));
			ILuaBase::UserData* udata = (ILuaBase::UserData*)lua_touserdata(L, -1);
			#ifdef WIN32
				if (udata->data != NULL)
					FreeLibrary((HMODULE)udata->data);
			#else
				if (udata->data)
					dlclose(udata->data);
			#endif
			isModule = false;
		}

		lua_pop(L, 1);
	}
}