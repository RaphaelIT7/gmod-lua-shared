DDLL_EXPORT void lua_init_stack_gmod(lua_State* L1, lua_State* L);
DDLL_EXPORT void GMOD_LuaPrint(const char* str, lua_State* L);
DDLL_EXPORT void* GMOD_LuaCreateEmptyUserdata(lua_State* L);
DDLL_EXPORT const char* GMODLUA_GetUserType(lua_State* L, int iType);