#include "lua_shared.h"
#include "CLuaConVars.h"
#include "tier3/tier3.h"
#ifndef BUILD_GMOD
#include "GarrysMod/FactoryLoader.hpp"
#include <Platform.hpp>
#endif

CLuaShared g_CLuaShared;

ILuaShared* g_pOrigLuaShared = NULL;
ILuaConVars* g_pOrigLuaConVars = NULL;
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


IGet* get;
CSysModule* g_pOrigLuaModule = NULL;
CSteamAPIContext* steamapicontext = NULL;
void CLuaShared::Init(CreateInterfaceFn interfaceFactory, bool magicBool, CSteamAPIContext* context, IGet* pGet)
{
	DebugPrint("CLuaShared::Init %s %p %p\n", magicBool ? "true" : "false", context, pGet);

	ConnectTier1Libraries(&interfaceFactory, 1);
	ConnectTier2Libraries(&interfaceFactory, 1);
#ifdef ARCHITECTURE_X86
	ConnectTier3Libraries(&interfaceFactory, 1);
#endif

	ConVar_Register(0);

#ifndef BUILD_GMOD
	g_pOrigLuaModule = Sys_LoadModule("garrysmod/bin/orig_lua_shared.dll"); // We need to load it or else the FactoryLoader fails because the dll isn't loaded.
	SourceSDK::FactoryLoader lua_shared_loader("orig_lua_shared");
	if (lua_shared_loader.GetFactory())
	{
		g_pOrigLuaShared = (ILuaShared*)lua_shared_loader.GetFactory()("LUASHARED003", NULL);
		g_pOrigLuaConVars = (ILuaConVars*)lua_shared_loader.GetFactory()("LUACONVARS001", NULL);
		Msg("Loading original lua_shared to not break stuff\n");
	}
#endif

	if (!g_pOrigLuaShared)
		LuaConVars()->Init();

	steamapicontext = context;

	get = pGet;

	if (g_pOrigLuaShared)
	{
		g_pOrigLuaShared->Init(interfaceFactory, magicBool, context, pGet);
		return;
	}
}

void CLuaShared::Shutdown()
{
	DebugPrint("CLuaShared::Shutdown\n");

	ConVar_Unregister();

	if (!g_pOrigLuaShared)
		LuaConVars()->DestroyManaged();

	if (g_pOrigLuaShared)
	{
		g_pOrigLuaShared->Shutdown();
		return;
	}

#ifdef ARCHITECTURE_X86
	DisconnectTier3Libraries();
#endif
	DisconnectTier2Libraries();
	DisconnectTier1Libraries();

	Sys_UnloadModule(g_pOrigLuaModule);
	g_pOrigLuaModule = NULL;
}

void CLuaShared::DumpStats()
{
	DebugPrint("CLuaShared::DumpStats\n");
	if (g_pOrigLuaShared)
	{
		g_pOrigLuaShared->DumpStats();
		return;
	}

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

	if (g_pOrigLuaShared)
		g_pOrigLuaShared->CreateLuaInterface(realm, unknown);

	return iFace;
}

void CLuaShared::CloseLuaInterface(ILuaInterface* LuaInterface)
{
	DebugPrint("CLuaShared::CloseLuaInterface\n");

	if ( LuaInterface->IsClient() )
		pInterfaces[0] = NULL;

	if ( LuaInterface->IsServer() )
		pInterfaces[1] = NULL;

	if ( LuaInterface->IsMenu() )
		pInterfaces[2] = NULL;

	::CloseLuaInterface(LuaInterface);
}

ILuaInterface* CLuaShared::GetLuaInterface(unsigned char realm)
{
	DebugPrint(2, "CLuaShared::GetLuaInterface\n");

	return pInterfaces[realm];
}

LuaFile::~LuaFile()
{
#ifndef WIN32
	if ( name )
		delete[] name;

	if ( contents )
		delete[] contents;
#endif
}

LuaFile* CLuaShared::LoadFile(const std::string& path, const std::string& pathId, bool fromDatatable, bool fromFile)
{
	DebugPrint("CLuaShared::LoadFile: %s %s (%s|%s)\n", path.c_str(), pathId.c_str(), fromDatatable ? "DT" : "No DT", fromFile ? "File" : "No File");

	LuaFile* file = NULL;
	if (g_pOrigLuaShared)
		file = g_pOrigLuaShared->LoadFile(path, pathId, fromDatatable, fromFile);

	if ( file )
		return file;

	DebugPrint("CLuaShared::LoadFile falled back!\n");
	if ( fromDatatable )
		file = LoadFile_FromDataTable( path, pathId, fromDatatable );

	if ( file )
		return file;

	if ( fromFile )
		file = LoadFile_FromFile( path, pathId, fromDatatable, fromFile );

	return file;
}

LuaFile* CLuaShared::LoadFile_FromFile(const std::string& path, const std::string& pathId, bool fromDatatable, bool fromFile) // BUG: On Linux, it crashes at pCache[path] = file; for some reason. Something seems really wrong.
{
	DebugPrint("CLuaShared::LoadFile: %s %s (%s|%s)\n", path.c_str(), pathId.c_str(), fromDatatable ? "DT" : "No DT", fromFile ? "File" : "No File");

	std::string final_path = path.c_str();
	if ( final_path.find( "lua/" ) == 0 ) // Should we use the MOD path for the DataPack? It should be faster.
		final_path.erase( 0, 4 );

	DebugPrint("CLuaShared::LoadFile: final path: %s\n", final_path.c_str());

	FileHandle_t fh = g_pFullFileSystem->Open(final_path.c_str(), "rb", pathId.c_str());
	if(fh)
	{
		LuaFile* file = new LuaFile;
		int file_len = g_pFullFileSystem->Size(fh);
		char* code = new char[file_len + 1];

		g_pFullFileSystem->Read((void*)code, file_len, fh);
		code[file_len] = 0;

		char* name = new char[strlen(path.c_str()) + 1];
		V_strncpy(name, path.c_str(), strlen(path.c_str()) + 1); // We use path here since I fear that something may break if we change it
		file->name = name;
		file->contents = code;
		file->time = g_pFullFileSystem->GetFileTime(final_path.c_str(), pathId.c_str()); // I don't like this.
		file->timesloadedclient = 0;
		file->timesloadedserver = 1;
		file->source = "!UNKNOWN";
		//Bootil::Compression::FastLZ::Compress(code, sizeof(code), file->compressed);
		DebugPrint("CLuaShared::LoadFile content size: %i\n", strlen(code));

#ifndef WIN32
		delete[] code; // file->content is a std::string which should make a copy of it.
#endif

		pCache[name] = file;

		g_pFullFileSystem->Close(fh);
		return file;
	} else {
		DebugPrint("CLuaShared::LoadFile failed to find the file\n");
		return NULL;
	}
}

LuaFile* CLuaShared::LoadFile_FromDataTable(const std::string& path, const std::string& pathId, bool fromDatatable)
{
	return NULL;
}

LuaFile* CLuaShared::GetCache(const std::string& fileName)
{
	DebugPrint("CLuaShared::GetCache %s\n", fileName.c_str());
	if (g_pOrigLuaShared)
		return g_pOrigLuaShared->GetCache(fileName);

	auto it = pCache.find(fileName);
	if (it != pCache.end())
		return it->second;

	/*for (File* f : pCache)
	{
		if (f->name == fileName)
		{
			return f;
		}
	}*/

	return nullptr;
}

void CLuaShared::MountLua(const char* pathID)
{
	DebugPrint("CLuaShared::MountLua %s\n", pathID);
	if (g_pOrigLuaShared)
	{
		g_pOrigLuaShared->MountLua(pathID);
		return;
	}

	std::string gamepath = get->GameDir();
	gamepath = gamepath + '\\';

	if ( get->IsDedicatedServer() )
	{
		AddSearchPath((gamepath).c_str(), pathID); // Next try to fix DS
	}

	AddSearchPath((gamepath + "lua\\").c_str(), pathID);

	//AddSearchPath((gamepath + "lua\\gamemodes\\").c_str(), pathID);

	AddSearchPath((gamepath + "gamemodes\\").c_str(), pathID);

	//if ( !get->IsDedicatedServer() ) // Fk this for now
	{
		IGamemodeSystem::UpdatedInformation& info = (IGamemodeSystem::UpdatedInformation&)g_pFullFileSystem->Gamemodes()->Active();
		if ( info.exists )
		{
			AddSearchPath((gamepath + "gamemodes\\" + info.name + "\\entities\\").c_str(), pathID);

			std::string nextBase = info.basename;
			while ( nextBase != "" )
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

void CLuaShared::MountLuaAdd(const char* file, const char* pathID)
{
	DebugPrint("CLuaShared::MountLuaAdd %s %s\n", file, pathID);
	if (g_pOrigLuaShared)
	{
		g_pOrigLuaShared->MountLuaAdd(file, pathID);
		return;
	}

	// Fancy code

	std::string gamepath = get->GameDir();
	gamepath = gamepath + '\\' + file;

	AddSearchPath( gamepath.c_str(), pathID );

	// Other Fancy code?
}

void CLuaShared::UnMountLua(const char* realm)
{
	DebugPrint("CLuaShared::UnMountLua %s\n", realm);
	if (g_pOrigLuaShared)
	{
		g_pOrigLuaShared->UnMountLua(realm);
		return;
	}

	// ToDo

	g_pFullFileSystem->RemoveSearchPaths( realm );
}

void CLuaShared::SetFileContents(const char* a, const char* b)
{
	DebugPrint("CLuaShared::SetFileContents %s, %s\n", a, b);
	if (g_pOrigLuaShared)
	{
		g_pOrigLuaShared->SetFileContents(a, b);
		return;
	}

	// Does nothing.
}

void CLuaShared::SetLuaFindHook(LuaClientDatatableHook* hook)
{
	DebugPrint("CLuaShared::SetLuaFindHook\n");
	if (g_pOrigLuaShared)
	{
		g_pOrigLuaShared->SetLuaFindHook(hook);
		return;
	}
}

void CLuaShared::FindScripts(const std::string& path, const std::string& pathID, std::vector<LuaFindResult>& out)
{
	DebugPrint("CLuaShared::FindScripts %s, %s\n", path.c_str(), pathID.c_str());
	if (g_pOrigLuaShared)
	{
		g_pOrigLuaShared->FindScripts(path, pathID, out);
		return;
	}

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
	if (g_pOrigLuaShared)
		return g_pOrigLuaShared->GetStackTraces();

	char* buffer = new char[1000];
	V_strncat(buffer, "	Client\n", 1000, -1);
	if (!GetLuaInterface(State::CLIENT)) // Probably another check with a stacktrace.
	{
		V_strncat(buffer, "    Lua Interface = NULL\n\n", 1000, -1);
	} else {
		// Do Magic.
		// Dump the stack by looping thru it. See GetCurrentLine or whatever the function was
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

void CLuaShared::InvalidateCache(const std::string& str)
{
	DebugPrint("CLuaShared::InvalidateCache %s\n", str.c_str());
	if (g_pOrigLuaShared)
	{
		g_pOrigLuaShared->InvalidateCache(str);
		return;
	}
}

void CLuaShared::EmptyCache()
{
	DebugPrint("CLuaShared::EmptyCache\n");
	if (g_pOrigLuaShared)
	{
		g_pOrigLuaShared->EmptyCache();
		return;
	}
}

bool CLuaShared::ScriptExists(const std::string& file, const std::string& path, bool idk)
{
	DebugPrint("CLuaShared::ScriptExists %s %s %s\n", file.c_str(), path.c_str(), idk ? "true" : "false");
	if (g_pOrigLuaShared)
		return g_pOrigLuaShared->ScriptExists(file, path, idk);

	return false;
}

void CLuaShared::AddSearchPath(const char* path, const char* pathID)
{
	DebugPrint("CLuaShared::AddSearchPath %s %s\n", path, pathID);

	std::string strPath = path;
#ifndef WIN32
	std::replace(strPath.begin(), strPath.end(), '\\', '/');
	DebugPrint("CLuaShared::AddSearchPath final path: %s\n", strPath.c_str());
#endif

	g_pFullFileSystem->AddSearchPath(strPath.c_str(), pathID);
}

EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CLuaShared, ILuaShared, GMOD_LUASHARED_INTERFACE, g_CLuaShared);