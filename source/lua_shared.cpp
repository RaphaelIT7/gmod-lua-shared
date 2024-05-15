#include "lua_shared.h"
#include "CLuaConVars.h"
#include "tier3/tier3.h"
#include <Platform.hpp>

CLuaShared g_CLuaShared;

ILuaShared* LuaShared()
{
	return &g_CLuaShared;
}

ConVar lua_debugmode("lua_debugmode_shared", "1", 0);
void DebugPrint(const char* fmt, ...) {
	if (!lua_debugmode.GetBool())
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

CLuaShared::~CLuaShared()
{
	DebugPrint("CLuaShared::~CLuaShared\n");
}

void CLuaShared::Init(CreateInterfaceFn interfaceFactory, bool, CSteamAPIContext*, IGet* get)
{
	DebugPrint("CLuaShared::Init\n");

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
	DebugPrint("CLuaShared::Shutdown\n");

	ConVar_Unregister();

#ifdef ARCHITECTURE_X86
	DisconnectTier3Libraries();
#endif
	DisconnectTier2Libraries();
	DisconnectTier1Libraries();
}

void CLuaShared::DumpStats()
{
	DebugPrint("CLuaShared::DumpStats\n");

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
	DebugPrint("CLuaShared::CreateLuaInterface\n");

	ILuaInterface* iFace = ::CreateLuaInterface(unknown);
	pInterfaces[realm] = iFace;

	return iFace;
}

void CLuaShared::CloseLuaInterface(ILuaInterface* LuaInterface)
{
	DebugPrint("CLuaShared::CloseLuaInterface\n");

	::CloseLuaInterface(LuaInterface);
}

ILuaInterface* CLuaShared::GetLuaInterface(unsigned char realm)
{
	DebugPrint("CLuaShared::GetLuaInterface\n");

	return pInterfaces[realm];
}

File* CLuaShared::LoadFile(const std::string& path, const std::string& pathId, bool fromDatatable, bool fromFile)
{
	DebugPrint("CLuaShared::LoadFile: %s\n", path.c_str());

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

		Bootil::AutoBuffer buffer;
		Bootil::Compression::FastLZ::Compress(code, sizeof(code), buffer);
		file->compressed = buffer;

		pCache[path] = file;

		g_pFullFileSystem->Close(fh);
	} else {
		delete file;
		return nullptr;
	}

	return file;
}

File* CLuaShared::GetCache(const std::string& unknown)
{
	DebugPrint("CLuaShared::GetCache %s", unknown.c_str());

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
	DebugPrint("CLuaShared::MountLua %s\n", pathID);

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
	DebugPrint("CLuaShared::UnMountLua %s %s\n", file, path);
	// Fancy code

	// AddSearchPath(file, path)

	// Other Fancy code?
}

void CLuaShared::UnMountLua(const char* realm)
{
	DebugPrint("CLuaShared::UnMountLua %s\n", realm);
	// ToDo
}

void CLuaShared::SetFileContents(const char* a, const char* b)
{
	DebugPrint("CLuaShared::SetFileContents %s, %s\n", a, b);
	// Does nothing.
}

void CLuaShared::SetLuaFindHook(LuaClientDatatableHook* hook)
{
	DebugPrint("CLuaShared::SetLuaFindHook\n");
}

void CLuaShared::FindScripts(const std::string& a, const std::string& b, std::vector<std::string>& out)
{
	DebugPrint("CLuaShared::FindScripts %s, %s\n", a.c_str(), b.c_str());
}

const char* CLuaShared::GetStackTraces()
{
	DebugPrint("CLuaShared::GetStackTraces\n");

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
	DebugPrint("CLuaShared::InvalidateCache\n");
}

void CLuaShared::EmptyCache()
{
	DebugPrint("CLuaShared::EmptyCache\n");
}

void CLuaShared::AddSearchPath(const char* path, const char* pathID)
{
	DebugPrint("CLuaShared::AddSearchPath %s %s\n", path, pathID);
	g_pFullFileSystem->AddSearchPath(path, pathID);
}

EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CLuaShared, ILuaShared, "LUASHARED003", g_CLuaShared);