#include "interface.h"
#include "lua_shared.h"
#include "color.h"
#include "tier3.h"
#include "CLuaConVars.h"

CLuaShared g_CLuaShared;

/*void _GMOD_LuaPrint(const char* msg)
{
	ConColorMsg(Color(255, 241, 122, 200), "%s", msg);
}*/

CLuaShared LuaShared()
{
	return g_CLuaShared;
}

CLuaShared::~CLuaShared()
{

}

void CLuaShared::Init(CreateInterfaceFn interfaceFactory, bool, CSteamAPIContext*, IGet*)
{
	ConnectTier1Libraries(&interfaceFactory, 1);
	ConnectTier2Libraries(&interfaceFactory, 1);
	ConnectTier3Libraries(&interfaceFactory, 1);

	ConVar_Register(0);

	LuaConVars();
}

void CLuaShared::Shutdown()
{
	ConVar_Unregister();

	DisconnectTier3Libraries();
	DisconnectTier2Libraries();
	DisconnectTier1Libraries();
}

void CLuaShared::DumpStats()
{
	Msg("Lua File Stats ------\n");
	Msg("Files In Cache: %i\n", 0);
	Msg("m_iCacheHints: %i\n", 0);
	Msg("m_iFailedLoads: %i\n", 0);
	Msg("m_iSuccessfulLoads: %i\n", 0);
	Msg("m_iDiskLoads: %i\n", 0);
	Msg("Time Spent Loading Files: %f seconds (%f milliseconds)\n", 0.0f, 0.0f);
	Msg("Time Spent Working Out CRCs: %f seconds (%f milliseconds)\n", 0.0f, 0.0f);
	Msg("Time Spent Checking Filestamps: %f seconds (%f milliseconds)", 0.0f, 0.0f);
}

ILuaInterface* CLuaShared::CreateLuaInterface(unsigned char, bool)
{

}

void CLuaShared::CloseLuaInterface(ILuaInterface*)
{

}

ILuaInterface* CLuaShared::GetLuaInterface(unsigned char)
{

}

File* CLuaShared::LoadFile(const std::string& path, const std::string& pathId, bool fromDatatable, bool fromFile)
{

}

File* CLuaShared::GetCache(const std::string&)
{

}

void CLuaShared::MountLua(const char*)
{

}

void CLuaShared::MountLuaAdd(const char*, const char*)
{

}

void CLuaShared::UnMountLua(const char*)
{

}

void CLuaShared::SetFileContents(const char*, const char*)
{

}

void CLuaShared::SetLuaFindHook(LuaClientDatatableHook*)
{

}

void CLuaShared::FindScripts(const std::string&, const std::string&, std::vector<std::string>&)
{

}

const char* CLuaShared::GetStackTraces()
{
	char* buffer = new char[1000];

	V_strncat(buffer, "	Client\n", 1000, -1);
	if (!GetLuaInterface(State::CLIENT)) // Probably another check with a stacktrace.
	{
		V_strncat(buffer, "    Lua Interface = NULL\n\n", 1000, -1);
	} else {
		// Do Magic.
	}

	V_strncat(buffer, "	Server\n", 1000, -1);
	if (!GetLuaInterface(State::SERVER)) // Probably another check with a stacktrace.
	{
		V_strncat(buffer, "    Lua Interface = NULL\n\n", 1000, -1);
	} else {
		// Do Magic.
	}

	V_strncat(buffer, "	MenuSystem\n", 1000, -1);
	if (!GetLuaInterface(State::MENU)) // Probably another check with a stacktrace.
	{
		V_strncat(buffer, "    Lua Interface = NULL\n\n", 1000, -1);
	} else {
		// Do Magic.
	}

	return buffer;
}

void CLuaShared::InvalidateCache(const std::string&)
{

}

void CLuaShared::EmptyCache()
{

}

EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CLuaShared, ILuaShared, "LUASHARED003", g_CLuaShared);