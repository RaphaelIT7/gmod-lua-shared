#include "CLuaConVars.h"
#include <tier2/tier2.h>
#include "filesystem.h"
#include "lua_shared.h"

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

ILuaConVars* LuaConVars()
{
	if (g_pOrigLuaConVars)
		return g_pOrigLuaConVars;

	return &g_CLuaConVars;
}

CLuaConVars::~CLuaConVars()
{
	// Create some huge list. ToDo
}

void CLuaConVars::Init()
{
	DebugPrint(1, "CLuaConVars::Init\n");
	pClientCVars = new KeyValues("CVars");
	pServerCVars = new KeyValues("CVars");

	if (!pClientCVars->LoadFromFile((IBaseFileSystem*)g_pFullFileSystem, "cfg/client.vdf", "MOD"))
		DebugPrint(1, "CLuaConVars::Init Failed to load client.vdf!\n");

	if (!pServerCVars->LoadFromFile((IBaseFileSystem*)g_pFullFileSystem, "cfg/server.vdf", "MOD"))
		DebugPrint(1, "CLuaConVars::Init Failed to load server.vdf!\n");
}

char* AAllocString(const char *pStr, int nMaxChars = -1)
{
	if (pStr == NULL)
		return NULL;

	int allocLen;
	if (nMaxChars == -1)
		allocLen = strlen(pStr) + 1;
	else
		allocLen = MIN((int)strlen(pStr), nMaxChars) + 1;

	char *pOut = new char[allocLen];
	V_strncpy(pOut, pStr, allocLen);
	return pOut;
}

ConVar* CLuaConVars::CreateConVar(const char* name, const char* defaultValue, const char* helpString, int flags)
{
	DebugPrint(1, "CLuaConVars::CreateConVar %s %s %s %i\n", name, defaultValue, helpString, flags);
	char* nameStr = AAllocString(name);
	char* defaultValueStr = AAllocString(defaultValue);
	char* helpStringStr = AAllocString(helpString);

	ConVar* cvar = new ConVar(nameStr, defaultValueStr, flags, helpStringStr);

	// ToDo: Verify this -> managedConVar.push_back(ManagedConVar(cvar));
	ManagedConVar* mcvar = new ManagedConVar;
	mcvar->name = name;
	mcvar->var = cvar;
	mcvar->iscvar = true;
	mcvar->nameStr = nameStr;
	mcvar->helpStr = helpStringStr;
	mcvar->valStr = defaultValueStr;
	pManagedCVars.push_back(mcvar);

	if (flags & FCVAR_LUA_SERVER)
	{
		for (KeyValues *sub = pServerCVars->GetFirstSubKey(); sub; sub = sub->GetNextKey())
		{
			if (V_stricmp(sub->GetName(), name) == 0)
			{
				const char* str = sub->GetString();
				if (str)
					cvar->SetValue(str);

				break;
			}
		}
	}

	if (flags & FCVAR_LUA_CLIENT)
	{
		for (KeyValues *sub = pClientCVars->GetFirstSubKey(); sub; sub = sub->GetNextKey())
		{
			if (V_stricmp(sub->GetName(), name) == 0)
			{
				const char* str = sub->GetString();
				if (str)
					cvar->SetValue(str);

				break;
			}
		}
	}

	return cvar;
}

ConCommand* CLuaConVars::CreateConCommand(const char* name, const char* helpString, int flags, FnCommandCallback_t callback, FnCommandCompletionCallback completionFunc)
{
	DebugPrint( 1, "CLuaConVars::CreateConCommand %s %s %i\n", name, helpString, flags );
	char* nameStr = AAllocString(name);
	char* helpStringStr = AAllocString(helpString);

	ConCommand* ccmd = new ConCommand(nameStr, callback, helpStringStr, flags, completionFunc);
	Msg("ConCommand registered: %s\n", ccmd->IsRegistered() ? "true" : "false");

	// ToDo: Verify this -> managedCommand.push_back(ManagedConVar(ccmd));
	ManagedConVar* mcmd = new ManagedConVar;
	mcmd->name = name;
	mcmd->var = ccmd;
	mcmd->iscvar = false;
	mcmd->nameStr = nameStr;
	mcmd->helpStr = helpStringStr;
	pManagedCVars.push_back(mcmd);

	return ccmd;
}

void CLuaConVars::DestroyManaged()
{
	DebugPrint(1, "CLuaConVars::DestroyManaged\n");
	// Do some magic ToDo
	for (ManagedConVar* cvar : pManagedCVars)
	{
		g_pCVar->UnregisterConCommand(cvar->var);
		delete cvar->var;

		if (cvar->nameStr)
			delete[] cvar->nameStr;

		if (cvar->valStr)
			delete[] cvar->valStr;

		if (cvar->helpStr)
			delete[] cvar->helpStr;

		delete cvar;
	}

	pManagedCVars.clear();

	if (!pClientCVars->IsEmpty("CVars")) // ToDo find out what the input is.
	{
		pClientCVars->SaveToFile((IBaseFileSystem*)g_pFullFileSystem, "cfg/client.vdf", "MOD");
	}

	if (!pServerCVars->IsEmpty("CVars")) // ToDo find out what the input is.
	{
		pServerCVars->SaveToFile((IBaseFileSystem*)g_pFullFileSystem, "cfg/server.vdf", "MOD");
	}
}

void CLuaConVars::Cache(const char* key, const char* value)
{
	DebugPrint(1, "CLuaConVars::Cache %s %s\n", key, value);
}

void CLuaConVars::ClearCache()
{
	DebugPrint(1, "CLuaConVars::ClearCache\n");
}

EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CLuaConVars, ILuaConVars, GMOD_LUACONVARS_INTERFACE, g_CLuaConVars);