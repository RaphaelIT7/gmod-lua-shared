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

char* AllocString( const char *pStr, int nMaxChars = -1 )
{
	int allocLen;
	if ( nMaxChars == -1 )
		allocLen = strlen( pStr ) + 1;
	else
		allocLen = min( (int)strlen(pStr), nMaxChars ) + 1;

	char *pOut = new char[allocLen];
	V_strncpy( pOut, pStr, allocLen );
	return pOut;
}

ConVar* CLuaConVars::CreateConVar(const char* name, const char* defaultValue, const char* helpString, int flags)
{
	char* nameStr = AllocString(name);
	char* defaultValueStr = AllocString(defaultValue);
	char* helpStringStr = AllocString(helpString);

	ConVar* cvar = new ConVar(nameStr, defaultValueStr, flags, helpStringStr);

	// ToDo: Verify this -> managedConVar.push_back(ManagedConVar(cvar));

	return cvar;
}

ConCommand* CLuaConVars::CreateConCommand(const char* name, const char* helpString, int flags, FnCommandCallback_t callback, FnCommandCompletionCallback completionFunc)
{
	char* nameStr = AllocString(name);
	char* helpStringStr = AllocString(helpString);

	ConCommand* ccmd = new ConCommand(nameStr, callback, helpStringStr, flags, completionFunc);

	// ToDo: Verify this -> managedCommand.push_back(ManagedConVar(ccmd));

	return ccmd;
}

void CLuaConVars::DestroyManaged()
{
	// Do some magic ToDo

	if (pClientCVars->IsEmpty("CVars")) // ToDo find out what the input is.
	{
		pClientCVars->SaveToFile((IBaseFileSystem*)g_pFullFileSystem, "cfg/client.vdf", "MOD");
	}

	if (pServerCVars->IsEmpty("CVars")) // ToDo find out what the input is.
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