#include "CLuaInterface.h"
#include "lua.hpp"

#ifdef WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

extern "C"
{
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
		//lua_pop(L, 1);

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
			FreeLibrary(hDll);
			return;
		}

		udata->data = hDll;

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
			dlclose(handle);
			return;
		}

		udata->data = handle;

		lua_pushcclosure(L, gmod13_open, 0);
		lua_call(L, 0, 0);
	#endif
	}

	void GMOD_UnloadBinaryModule(lua_State* L, const char* module, ILuaBase::UserData* udata)
	{
		#ifdef WIN32
			if (udata->data != NULL)
			{
				CFunc gmod13_close = (CFunc)GetProcAddress((HMODULE)udata->data, "gmod13_close");
				if (gmod13_close != NULL) {
					lua_pushcclosure(L, gmod13_close, 0);
					lua_call(L, 0, 0);
				}

				FreeLibrary((HMODULE)udata->data);
			}
		#else
			if (udata->data)
			{
				CFunc gmod13_close = (CFunc)dlsym(udata->data, "gmod13_close");
				if (gmod13_close) {
					lua_pushcclosure(L, gmod13_close, 0);
					lua_call(L, 0, 0);
				}

				dlclose(udata->data);
			}
		#endif

		lua_pushvalue(L, LUA_REGISTRYINDEX);
		lua_pushnil(L);
		lua_setfield(L, -2, module);
		lua_pop(L, 1);
	}

	void GMOD_UnloadBinaryModules(lua_State* L)
	{
		lua_pushvalue(L, LUA_REGISTRYINDEX);
		lua_pushnil(L);

		while (lua_next(L, -2) != 0) {
			if(lua_type(L, -2) == Type::String && lua_type(L, -1) == Type::UserData)
			{
				const char* module = lua_tolstring(L, -2, NULL);
				if (strncmp(module, "LOADLIB: ", 8) == 0)
				{
					Msg("Unloading %s\n", module);
					ILuaBase::UserData* udata = (ILuaBase::UserData*)lua_touserdata(L, -1);
					GMOD_UnloadBinaryModule(L, module, udata);
				}
			}

			lua_pop(L, 1);
		}

		lua_pop(L, 1);
	}
}