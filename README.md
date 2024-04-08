# gmod-lua-shared

My goal is to recreate the lua_shared.dll from Gmod.  
Current LUAJIT Version used for this: https://github.com/LuaJIT/LuaJIT/commit/e826d0c101d750fac8334d71e221c50d8dbe236c
> Note: This is still WIP  

Maybe if I finish this I also try to get Lua 5.4 to work :D

# ToDo

## Current Issues
- [x] Engine Error: `Error loading lua_shared (Sys_GetFactory)`
- [x] `Segmentation fault (core dumped) ../../../source/CLuaInterface.cpp:958`
- [x] CLuaInterface::Global needs to be implemented. Or else we crash.
- [x] CLuaInterface::SetMember needs to be implemented.
- [ ] CLuaInterface::GetMetaTableObject needs to be implemented.

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

## ILuaThreadedCall
This is not even used in Gmod :|  
But the code to use it exists `AddThreadedCall` and `RunThreadedCalls`

## Collection of random Engine Errors
![image](https://github.com/RaphaelIT7/gmod-lua-shared/assets/64648134/aa4143fc-75f2-4311-b23b-d2de1394d59b)
![image](https://github.com/RaphaelIT7/gmod-lua-shared/assets/64648134/85631077-ec8d-4831-8fe1-5789f8090493)

