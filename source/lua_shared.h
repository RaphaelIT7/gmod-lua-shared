#include "CLuaInterface.h"
#include <filesystem.h>
#include <GarrysMod/IGet.h>
#include <unordered_map>

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

struct File
{
	int time;
#ifdef WIN32
	std::string name;
	std::string source;
	std::string contents;
#else
	const char* name;
	const char* source;
	const char* contents;
#endif
	Bootil::AutoBuffer compressed;
	unsigned int timesloadedserver;
	unsigned int timesloadedclient;
};

struct LuaFindResult
{
	std::string fileName;
	bool isFolder;
};

class LuaClientDatatableHook;

class ILuaShared
{
public:
	virtual ~ILuaShared() {};
	virtual void Init(CreateInterfaceFn, bool, CSteamAPIContext *, IGet *) = 0;
	virtual void Shutdown() = 0;
	virtual void DumpStats() = 0;
	virtual ILuaInterface *CreateLuaInterface(unsigned char, bool) = 0;
	virtual void CloseLuaInterface(ILuaInterface *) = 0;
	virtual ILuaInterface *GetLuaInterface(unsigned char) = 0;
	virtual File *LoadFile(const std::string &path, const std::string &pathId, bool fromDatatable, bool fromFile) = 0;
	virtual File *GetCache(const std::string &) = 0;
	virtual void MountLua(const char *) = 0;
	virtual void MountLuaAdd(const char *, const char *) = 0;
	virtual void UnMountLua(const char *) = 0;
	virtual void SetFileContents(const char *, const char *) = 0;	
	virtual void SetLuaFindHook(LuaClientDatatableHook *) = 0;
	virtual void FindScripts(const std::string &, const std::string &, std::vector<LuaFindResult> &) = 0;
	virtual const char *GetStackTraces() = 0;
	virtual void InvalidateCache(const std::string &) = 0;
	virtual void EmptyCache() = 0;
	virtual bool ScriptExists(const std::string &, const std::string &, bool) = 0;
};

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
	virtual File* LoadFile(const std::string& path, const std::string& pathId, bool fromDatatable, bool fromFile);
	virtual File* GetCache(const std::string&);
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
	IGet* GetIGet() { return pGet; };
	void AddSearchPath(const char*, const char*);
private: // ToDo: Get the real structure below
	IGet* pGet;
	ILuaInterface* pInterfaces[3];
	//std::vector<File*> pCache;
	std::unordered_map<std::string, File*> pCache;
	std::unordered_map<std::string, std::vector<std::string>> pMount;
};