# gmod-lua-shared

My goal is to recreate the lua_shared.dll from Gmod.  
Current LUAJIT Version used for this: https://github.com/LuaJIT/LuaJIT/commit/e826d0c101d750fac8334d71e221c50d8dbe236c
> Note: This is still WIP

# ToDo

## ILuaBase

- ILuaBase::PushMetaTable(int iType)
- ILuaBase::PushUserType(void* data, int iType)
- ILuaBase::SetUserType(int iStackPos, void* data)

## ILuaInterface / CLuaInterface

Everything.

## CLuaConVars

- CLuaConVars::CreateConVar
- CLuaConVars::CreateConCommand
- CLuaConVars::DestroyManaged
- CLuaConVars::Cache
- CLuaConVars::ClearCache

## CLuaShared

Most of it.