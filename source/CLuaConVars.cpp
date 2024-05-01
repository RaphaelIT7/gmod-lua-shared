#include "CLuaConVars.h"
#include <tier2/tier2.h>

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

char* AAllocString( const char *pStr, int nMaxChars = -1 )
{
	if (pStr == NULL)
		return NULL;

	int allocLen;
	if ( nMaxChars == -1 )
		allocLen = strlen( pStr ) + 1;
	else
		allocLen = MIN( (int)strlen(pStr), nMaxChars ) + 1;

	char *pOut = new char[allocLen];
	V_strncpy( pOut, pStr, allocLen );
	return pOut;
}

ConVar* CLuaConVars::CreateConVar(const char* name, const char* defaultValue, const char* helpString, int flags)
{
	char* nameStr = AAllocString(name);
	char* defaultValueStr = AAllocString(defaultValue);
	char* helpStringStr = AAllocString(helpString);

	ConVar* cvar = new ConVar(nameStr, defaultValueStr, flags, helpStringStr);

	// ToDo: Verify this -> managedConVar.push_back(ManagedConVar(cvar));
	ManagedCVar* mcvar = new ManagedCVar;
	mcvar->name = name;
	mcvar->cvar = cvar;
	mcvar->iscvar = true;
	pManagedCVars.push_back(mcvar);

	return cvar;
}

ConCommand* CLuaConVars::CreateConCommand(const char* name, const char* helpString, int flags, FnCommandCallback_t callback, FnCommandCompletionCallback completionFunc)
{
	char* nameStr = AAllocString(name);
	char* helpStringStr = AAllocString(helpString);

	ConCommand* ccmd = new ConCommand(nameStr, callback, helpStringStr, flags, completionFunc);

	// ToDo: Verify this -> managedCommand.push_back(ManagedConVar(ccmd));
	ManagedCVar* mcmd = new ManagedCVar;
	mcmd->name = name;
	mcmd->cmd = ccmd;
	mcmd->iscvar = false;
	pManagedCVars.push_back(mcmd);

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

void CLuaConVars::Cache(const char* key, const char* value)
{

}

void CLuaConVars::ClearCache()
{

}

EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CLuaConVars, ILuaConVars, "LUACONVARS001", g_CLuaConVars);