#include "lua_shared.h"
#include "CLuaConVars.h"
#include "tier3/tier3.h"
#ifndef BUILD_GMOD
#include <Platform.hpp>
#endif

CLuaShared g_CLuaShared;

ILuaShared* LuaShared()
{
	return &g_CLuaShared;
}

static ConVar lua_debugmode_shared("lua_debugmode_shared", "1", 0);
static void DebugPrint(const char* fmt, ...) {
	if (!lua_debugmode_shared.GetBool())
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

	LuaConVars()->Init();

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

	if ( LuaInterface->IsServer() )
		pInterfaces[1] = NULL;

	if ( LuaInterface->IsClient() )
		pInterfaces[0] = NULL;

	if ( LuaInterface->IsMenu() )
		pInterfaces[2] = NULL;

	::CloseLuaInterface(LuaInterface);
}

ILuaInterface* CLuaShared::GetLuaInterface(unsigned char realm)
{
	DebugPrint(2, "CLuaShared::GetLuaInterface\n");

	return pInterfaces[realm];
}

File* CLuaShared::LoadFile(const std::string& path, const std::string& pathId, bool fromDatatable, bool fromFile) // BUG: On Linux, it crashes at pCache[path] = file; for some reason. Something seems really wrong.
{
	DebugPrint("CLuaShared::LoadFile: %s %s %s %s\n", path.c_str(), pathId.c_str(), fromDatatable ? "DT" : "No DT", fromFile ? "File" : "No File");

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
		DebugPrint("CLuaShared::LoadFile failed to find the file\n");
		delete file;
		return nullptr;
	}

	return file;
}

File* CLuaShared::GetCache(const std::string& unknown)
{
	DebugPrint("CLuaShared::GetCache %s\n", unknown.c_str());

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

	AddSearchPath((gamepath + "gamemodes\\").c_str(), pathID);

	if ( !pGet->IsDedicatedServer() ) // Fk this for now
	{
		IGamemodeSystem::UpdatedInformation& info = (IGamemodeSystem::UpdatedInformation&)g_pFullFileSystem->Gamemodes()->Active();
		if ( info.exists )
		{
			AddSearchPath((gamepath + "gamemodes\\" + info.name + "\\entities\\").c_str(), pathID);

			std::string nextBase = info.basename;
			while ( nextBase != "" ) // info.exists isn't available on the 64x yet.
			{
				const IGamemodeSystem::UpdatedInformation& base = (IGamemodeSystem::UpdatedInformation&)g_pFullFileSystem->Gamemodes()->FindByName( nextBase );
				if ( !base.exists )
					break;
			
				AddSearchPath((gamepath + "gamemodes\\" + base.name + "\\entities\\").c_str(), pathID);
				nextBase = base.basename;
			}
		}
	}

	// Do some stuff = Push somthing 3x & Call a function

	// Do some stuff with "lua"

	// Do some stuff with "gamemodes"

	// Do some stuff
}

void CLuaShared::MountLuaAdd(const char* file, const char* path)
{
	DebugPrint("CLuaShared::MountLuaAdd %s %s\n", file, path);
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

void CLuaShared::FindScripts(const std::string& path, const std::string& pathID, std::vector<LuaFindResult>& out)
{
	DebugPrint("CLuaShared::FindScripts %s, %s\n", path.c_str(), pathID.c_str());

	FileFindHandle_t findHandle;
	const char *pFilename = g_pFullFileSystem->FindFirstEx( path.c_str(), pathID.c_str(), &findHandle );
	while ( pFilename )
	{
		if ( V_stricmp( pFilename, "." ) == 0 || V_stricmp( pFilename, ".." ) == 0 ) // don't add ./ and ../
		{
			pFilename = g_pFullFileSystem->FindNext( findHandle );
			continue;
		}

		LuaFindResult result;
		result.fileName = pFilename;
		result.isFolder = g_pFullFileSystem->FindIsDirectory( findHandle );
		out.push_back( result );

		pFilename = g_pFullFileSystem->FindNext( findHandle );
	}

	g_pFullFileSystem->FindClose( findHandle );
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

bool CLuaShared::ScriptExists(const std::string& file, const std::string& path, bool idk)
{
	DebugPrint("CLuaShared::ScriptExists %s %s %s\n", file.c_str(), path.c_str(), idk ? "Yes" : "No");
	return false;
}

void CLuaShared::AddSearchPath(const char* path, const char* pathID)
{
	DebugPrint("CLuaShared::AddSearchPath %s %s\n", path, pathID);
	g_pFullFileSystem->AddSearchPath(path, pathID);
}

EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CLuaShared, ILuaShared, "LUASHARED003", g_CLuaShared);