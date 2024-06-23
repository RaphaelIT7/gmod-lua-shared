#include "CLuaConVars.h"
#include <tier2/tier2.h>

static ConVar lua_debugmode("lua_debugmode_convar", "2", 0);
static void DebugPrint(int level, const char* fmt, ...)
{
	if (lua_debugmode.GetInt() < level)
		return;

	va_list args;
	va_start(args, fmt);

	int size = vsnprintf(NULL, 0, fmt, args);
	if (size < 0) {
		va_end(args);
		return;
	}

	char* buffer = new char[size + 1];
	vsnprintf(buffer, size + 1, fmt, args);

	Msg("%s", buffer);

	delete[] buffer;
	va_end(args);
}

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
	DebugPrint( 1, "CLuaConVars::Init\n" );
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
	DebugPrint( 1, "CLuaConVars::CreateConVar %s %s %s %i\n", name, defaultValue, helpString, flags );
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
	DebugPrint( 1, "CLuaConVars::CreateConCommand %s %s %i\n", name, helpString, flags );
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
	DebugPrint( 1, "CLuaConVars::DestroyManaged\n" );
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
	DebugPrint( 1, "CLuaConVars::Cache %s %s\n", key, value );
}

void CLuaConVars::ClearCache()
{
	DebugPrint( 1, "CLuaConVars::ClearCache\n" );
}

EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CLuaConVars, ILuaConVars, "LUACONVARS001", g_CLuaConVars);