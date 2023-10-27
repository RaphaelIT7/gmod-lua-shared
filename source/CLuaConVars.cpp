#include "CLuaConVars.h"
#include "tier2/tier2.h"

CLuaConVars g_CLuaConVars;

CLuaConVars* LuaConVars()
{
	return &g_CLuaConVars;
}

CLuaConVars::~CLuaConVars()
{
	// Create some huge list. ToDo
}

void CLuaConVars::Init()
{
	pClientCVars = new KeyValues("CVars");
	pServerCVars = new KeyValues("CVars");

	pClientCVars->LoadFromFile((IBaseFileSystem*)g_pFullFileSystem, "cfg/server.vdf", "MOD");
	pServerCVars->LoadFromFile((IBaseFileSystem*)g_pFullFileSystem, "cfg/client.vdf", "MOD");
}

ConVar* CLuaConVars::CreateConVar(const char*, const char*, const char*, int)
{

}

ConCommand* CLuaConVars::CreateConCommand(const char*, const char*, int, void (*)(const CCommand&), int (*)(const char*, char(*)[128]))
{

}

void CLuaConVars::DestroyManaged()
{
	// Do some magic ToDo

	if (pClientCVars->IsEmpty("")) // ToDo find out what the input is.
	{
		pClientCVars->SaveToFile((IBaseFileSystem*)g_pFullFileSystem, "cfg/client.vdf", "MOD");
	}

	if (pServerCVars->IsEmpty("")) // ToDo find out what the input is.
	{
		pServerCVars->SaveToFile((IBaseFileSystem*)g_pFullFileSystem, "cfg/server.vdf", "MOD");
	}
}

void CLuaConVars::Cache(const char*, const char*)
{

}

void CLuaConVars::ClearCache()
{

}

EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CLuaConVars, ILuaConVars, "LUACONVARS001", g_CLuaConVars);