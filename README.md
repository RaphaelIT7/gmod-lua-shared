# gmod-lua-shared

My goal is to recreate the lua_shared.dll from Gmod.  
Current LUAJIT Version used for this: https://github.com/LuaJIT/LuaJIT/commit/a4f56a459a588ae768801074b46ba0adcfb49eb1
> Note: This is still WIP  

Maybe if I finish this I also try to get Lua 5.4 to work :D

# ToDo

## Current Issues
- [x] Engine Error: `Error loading lua_shared (Sys_GetFactory)`<br>
- [x] `Segmentation fault (core dumped) ../../../source/CLuaInterface.cpp:958`<br>
- [x] CLuaInterface::Global needs to be implemented. Or else we crash.<br>
- [x] CLuaInterface::SetMember needs to be implemented.<br>
- [x] CLuaInterface::GetMetaTableObject needs to be implemented.<br>
- [x] CLuaInterface::FindAndRunScript needs to be implemented.<br>
- [x] CLuaInterface::PushUserType needs to be implemented?<br>
- [x] Random Crashes on Windows? (Seems to be solved???)<br>
- [x] ILuaObjects are broken? (obj->isTable() = false, (obj->GetType() == Type::Table) = true?, obj->Push() = nothing???) Best example is Global();<br>
NOTE: Added a workaround to fix it.<br>
- [x] Fixed the Stack<br>
- [x] (IMPORTANT) Find out why functions like Material crash. Are we missing something?<br>
- [x] Implement continue<br>
- [x] What makes this call? What is broken? `CLuaInterface::CheckString 1068146288<br>
NOTE: I should actual give Msg the Value to ACTUALLY print the REAL value and not some memory.<br>
- [x] What causes a stack leak? A bunch of nil values end up on the stack.<br>
- [x] Fix SetMember functions causing `[MENU ERROR] attempt to index a string value`<br>
- [x] Find another stack leak.<br>
NOTE: The leak was cased by `CallFunctionProtected` returning false instead of true.<br>
- [x] Fix `/* */` and `//` in LuaJIT and remove the Workarounds from `CLuaInterface::RunMacros`<br>
BUG: `[MENU ERROR] [string "includes/util/color.lua"]:81: ')' expected near '~'`<br>
- [ ] Fix some random error (`the file "(null)"" couldn't be opened`)<br>
- [x] Fix the random crash when loading into a game<br>
- - [x] Fix CallFunctionProtected & GetReturn<br>
- [x] Type Errors cause engine errors.<br>
- [x] Fix a crash when closing the game.<br>
- [x] Fix `/* */`. it ignores the full line.<br>
- - [x] Find out why the local build lib works but the one from the compiler fails<br>
- [x] Fix `print` breaking with numbers.<br>
- [x] Find out why you cannot press a VGUI Button.<br>
- [x] Fix a crash on level shutdown (menusystem.dll crashes)<br>
- [ ] Implement Managed Convars properly -> Save and load their values.<br>
- [ ] Implement the cache properly<br>
- [x] Why is the player NULL in concommand.Run?<br>
NOTE: This was caused by the ILuaObjects being broken since on windows the vtable is offset.<br>
- [x] Fixed FindAndRunScript function being utter garbage<br>
- [ ] Figure out why the player and weapon color aren't set on spawn properly.<br>


## Linux DS issues
- [ ] Crashes while loading.
- - [x] Implement Bootil to fill the compressed field to not cause a crash.
- - [x] Fix a crash in CLuaShared::LoadFile
- - [ ] Get the proper CLuaError struct
- - [ ] Find out why `GModDataPack::GetHashFromString` eats up all our memory.

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