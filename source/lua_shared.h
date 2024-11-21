#include "CLuaInterface.h"
#include <filesystem.h>
#include <GarrysMod/IGet.h>
#include <unordered_map>
#include "public/ILuaShared.h"

namespace IGamemodeSystem
{
	struct UpdatedInformation
	{
		bool exists;
		bool menusystem;
#ifdef WIN32
		std::string title;
		std::string name;
		std::string maps;
		std::string basename;
		std::string category;
#else
		const char* title;
		const char* name;
		const char* maps;
		const char* basename;
		const char* category;
#endif
		uint64_t workshopid;
	};
}

class IGet;
class ILuaConVars;
class ILuaShared;

extern IGet* get;
extern ILuaConVars* g_pOrigLuaConVars;
extern ILuaShared* g_pOrigLuaShared;

class CLuaShared : public ILuaShared
{
public:
	virtual ~CLuaShared();
	virtual void Init(CreateInterfaceFn, bool, CSteamAPIContext*, IGet*);
	virtual void Shutdown();
	virtual void DumpStats();
	virtual ILuaInterface* CreateLuaInterface(unsigned char, bool);
	virtual void CloseLuaInterface(ILuaInterface*);
	virtual ILuaInterface* GetLuaInterface(unsigned char);
	virtual LuaFile* LoadFile(const std::string& path, const std::string& pathId, bool fromDatatable, bool fromFile);
	virtual LuaFile* GetCache(const std::string&);
	virtual void MountLua(const char*);
	virtual void MountLuaAdd(const char*, const char*);
	virtual void UnMountLua(const char*);
	virtual void SetFileContents(const char*, const char*);
	virtual void SetLuaFindHook(LuaClientDatatableHook*);
	virtual void FindScripts(const std::string&, const std::string&, std::vector<LuaFindResult>&);
	virtual const char* GetStackTraces();
	virtual void InvalidateCache(const std::string&);
	virtual void EmptyCache();
	virtual bool ScriptExists(const std::string &, const std::string &, bool);
public:
	void AddSearchPath(const char*, const char*);
	LuaFile* LoadFile_FromFile(const std::string& path, const std::string& pathId, bool fromDatatable, bool fromFile);
	LuaFile* LoadFile_FromDataTable(const std::string& path, const std::string& pathId, bool fromDatatable);
private: // ToDo: Get the real structure below
	ILuaInterface* pInterfaces[3];
	//std::vector<File*> pCache;
	std::unordered_map<std::string, LuaFile*> pCache;
	std::unordered_map<std::string, std::vector<std::string>> pMount;
};