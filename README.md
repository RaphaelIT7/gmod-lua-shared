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
- [x] Implement continue  
- [x] What makes this call? What is broken? `CLuaInterface::CheckString 1068146288`  
NOTE: I should actual give Msg the Value to ACTUALLY print the REAL value and not some memory.
- [x] What causes a stack leak? A bunch of nil values end up on the stack.  
- [x] Fix SetMember functions causing `[MENU ERROR] attempt to index a string value`
- [x] Find another stack leak.  
NOTE: The leak was cased by `CallFunctionProtected` returning false instead of true.  
- [x] Fix `/* */` and `//` in LuaJIT and remove the Workarounds from `CLuaInterface::RunMacros`  
BUG: `[MENU ERROR] [string "includes/util/color.lua"]:81: ')' expected near '~'`  
- [ ] Fix some random error (`the file "(null)"" couldn't be opened`)
- [x] Fix the random crash when loading into a game  
- - [x] Fix CallFunctionProtected & GetReturn
- [ ] Type Errors cause engine errors.
- [ ] Fix a crash when closing the game.
- [x] Fix `/* */`. it ignores the full line.
- - [ ] Find out why the local build lib works but the one from the compiler fails
- [x] Fix `print` breaking with numbers.
- [ ] Find out why you cannot press a VGUI Button.

## Linux DS issues
- [ ] Crashes while loading.
- - [x] Implement Bootil to fill the compressed field to not cause a crash.
- - [ ] Fix a crash in CLuaShared::LoadFile

## Collection of random Engine Errors and Warnings or other things
![image](https://github.com/RaphaelIT7/gmod-lua-shared/assets/64648134/aa4143fc-75f2-4311-b23b-d2de1394d59b)
![image](https://github.com/RaphaelIT7/gmod-lua-shared/assets/64648134/85631077-ec8d-4831-8fe1-5789f8090493)
![image](https://github.com/RaphaelIT7/gmod-lua-shared/assets/64648134/de603ca9-d70c-4aca-96b7-f7ff39cf529e)
![image](https://github.com/RaphaelIT7/gmod-lua-shared/assets/64648134/f0df946c-fe5c-4b40-9482-099e32b5b827)
![image](https://github.com/RaphaelIT7/gmod-lua-shared/assets/64648134/f3b58551-76f5-4d12-80ea-315d613654eb)
![image](https://github.com/RaphaelIT7/gmod-lua-shared/assets/64648134/6a1f8287-c7c5-41dc-868c-3a5bfbc1fa17)
![image](https://github.com/RaphaelIT7/gmod-lua-shared/assets/64648134/a35a26bf-5dab-4809-b572-fc56ae2f1bea)
![image](https://github.com/RaphaelIT7/gmod-lua-shared/assets/64648134/e86fea60-73d4-412a-af30-f3c1f53ddb9c)
![image](https://github.com/RaphaelIT7/gmod-lua-shared/assets/64648134/34406eed-5e6c-4d01-94c9-1ab1075416c8)
![image](https://github.com/RaphaelIT7/gmod-lua-shared/assets/64648134/b6052e0b-7555-41d7-896d-1df0f333aa4a)