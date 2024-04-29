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
- [x] CLuaInterface::GetMetaTableObject needs to be implemented.
- [x] CLuaInterface::FindAndRunScript needs to be implemented.
- [x] CLuaInterface::PushUserType needs to be implemented?
- [x] Random Crashes on Windows? (Seems to be solved???)
- [x] ILuaObjects are broken? (obj->isTable() = false, (obj->GetType() == Type::Table) = true?, obj->Push() = nothing???) Best example is Global();
NOTE: Added a workaround to fix it.  
- [x] Fixed the Stack
- [x] (IMPORTANT) Find out why functions like Material crash. Are we missing something?  
- [ ] Update the ILuaObject. It's different in the MenuSystem.dll
- [ ] Fix SetMember functions causing `[MENU ERROR] attempt to index a string value`
- [ ] Type Errors cause engine errors.

## Linux DS issues
- [ ] Crashes while loading.

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

## Collection of random Engine Errors and Warnings
![image](https://github.com/RaphaelIT7/gmod-lua-shared/assets/64648134/aa4143fc-75f2-4311-b23b-d2de1394d59b)
![image](https://github.com/RaphaelIT7/gmod-lua-shared/assets/64648134/85631077-ec8d-4831-8fe1-5789f8090493)
![image](https://github.com/RaphaelIT7/gmod-lua-shared/assets/64648134/de603ca9-d70c-4aca-96b7-f7ff39cf529e)
![image](https://github.com/RaphaelIT7/gmod-lua-shared/assets/64648134/f0df946c-fe5c-4b40-9482-099e32b5b827)
![image](https://github.com/RaphaelIT7/gmod-lua-shared/assets/64648134/f3b58551-76f5-4d12-80ea-315d613654eb)



> NOTE for myself: Go thru these [commits](https://github.com/mingodad/ljsjit) and learn how to add gmod's syntax to lua