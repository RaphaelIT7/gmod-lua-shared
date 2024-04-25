#include "lua_shared.h"
#include "CLuaConVars.h"
#include "tier3/tier3.h"
#include <Platform.hpp>

CLuaShared g_CLuaShared;

/*void GMOD_LuaPrint(const char* msg)
{
	ConColorMsg(Color(255, 241, 122, 200), "%s", msg);
}*/

ILuaShared* LuaShared()
{
	return &g_CLuaShared;
}

CLuaShared::~CLuaShared()
{

}

void CLuaShared::Init(CreateInterfaceFn interfaceFactory, bool, CSteamAPIContext*, IGet* get)
{
	ConnectTier1Libraries(&interfaceFactory, 1);
	ConnectTier2Libraries(&interfaceFactory, 1);
#ifdef ARCHITECTURE_X86
	ConnectTier3Libraries(&interfaceFactory, 1);
#endif

	ConVar_Register(0);

	LuaConVars();

	pGet = get;
}

void CLuaShared::Shutdown()
{
	ConVar_Unregister();

#ifdef ARCHITECTURE_X86
	DisconnectTier3Libraries();
#endif
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

ILuaInterface* CLuaShared::CreateLuaInterface(unsigned char realm, bool unknown)
{
	ILuaInterface* iFace = ::CreateLuaInterface(unknown);
	pInterfaces[realm] = iFace;

	return iFace;
}

void CLuaShared::CloseLuaInterface(ILuaInterface* LuaInterface)
{
	::CloseLuaInterface(LuaInterface);
}

ILuaInterface* CLuaShared::GetLuaInterface(unsigned char realm)
{
	return pInterfaces[realm];
}

File* CLuaShared::LoadFile(const std::string& path, const std::string& pathId, bool fromDatatable, bool fromFile)
{
	Msg("CLuaShared::LoadFile: %s\n", path.c_str());

	File* file = new File;
	FileHandle_t fh = g_pFullFileSystem->Open(path.c_str(), "rb", pathId.c_str());
	if(fh)
	{
		int file_len = g_pFullFileSystem->Size(fh);
		char* code = new char[file_len + 1];

		g_pFullFileSystem->Read((void*)code, file_len, fh);
		code[file_len] = 0;

		file->name = path.c_str();
		file->contents = code;
		file->time = g_pFullFileSystem->GetFileTime(path.c_str(), pathId.c_str());
		file->timesloadedclient = 0;
		file->timesloadedserver = 0;
		file->source = path.c_str();
		file->compressed = nullptr; // ToDo: Get Bootil to work and Change this to use Bootil::AutoBuffer. This probably crashes dedicated servers currently

		pCache[path] = file;

		g_pFullFileSystem->Close(fh);
	} else {
		delete file;
	}

	return file;
}

File* CLuaShared::GetCache(const std::string& unknown)
{
	Msg("CLuaShared::GetCache %s", unknown.c_str());

	auto it = pCache.find(unknown);
	if (it != pCache.end())
		return it->second;

	/*for (File* f : pCache)
	{
		if (f->name == unknown)
		{
			return f;
		}
	}*/

	return nullptr;
}

void CLuaShared::MountLua(const char* pathID)
{
	Msg("CLuaShared::MountLua %s\n", pathID);

	std::string gamepath = pGet->GameDir();
	gamepath = gamepath + '\\';

	AddSearchPath((gamepath + "lua\\").c_str(), pathID);

	AddSearchPath((gamepath + "lua\\gamemodes\\").c_str(), pathID);

	// Do some stuff = Push somthing 3x & Call a function

	// Do some stuff with "lua"

	// Do some stuff with "gamemodes"

	// Do some stuff
}

void CLuaShared::MountLuaAdd(const char* file, const char* path)
{
	Msg("CLuaShared::UnMountLua %s %s\n", file, path);
	// Fancy code

	// AddSearchPath(file, path)

	// Other Fancy code?
}

void CLuaShared::UnMountLua(const char* realm)
{
	Msg("CLuaShared::UnMountLua %s\n", realm);
	// ToDo
}

void CLuaShared::SetFileContents(const char* a, const char* b)
{
	Msg("CLuaShared::SetFileContents %s, %s\n", a, b);
	// Does nothing.
}

void CLuaShared::SetLuaFindHook(LuaClientDatatableHook* hook)
{
	Msg("CLuaShared::SetLuaFindHook\n");
}

void CLuaShared::FindScripts(const std::string& a, const std::string& b, std::vector<std::string>& out)
{
	Msg("CLuaShared::FindScripts %s, %s\n", a.c_str(), b.c_str());
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

void CLuaShared::AddSearchPath(const char* path, const char* pathID)
{
	g_pFullFileSystem->AddSearchPath(path, pathID);
}

EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CLuaShared, ILuaShared, "LUASHARED003", g_CLuaShared);