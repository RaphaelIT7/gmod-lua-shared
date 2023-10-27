#include "CLuaInterface.h"
#include <filesystem.h>
#include <Bootil/Bootil.h>

namespace State
{
	enum
	{
		CLIENT = 0,
		SERVER,
		MENU
	};

	static const char *Name[] = {
		"client",
		"server",
		"menu",
		nullptr
	};
}

struct File
{
	int time;
	std::string name;
	std::string source;
	std::string contents;
	Bootil::AutoBuffer compressed;
	unsigned int timesloadedserver;
	unsigned int timesloadedclient;
};

class LuaClientDatatableHook;

class ILuaShared
{
public:
	virtual ~ILuaShared() = 0;
	virtual void Init(void *(*)(const char *, int *), bool, CSteamAPIContext *, IGet *) = 0;
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
	virtual void FindScripts(const std::string &, const std::string &, std::vector<std::string> &) = 0;
	virtual const char *GetStackTraces() = 0;
	virtual void InvalidateCache(const std::string &) = 0;
	virtual void EmptyCache() = 0;
};

class CLuaShared : public ILuaShared
{
public:
	~CLuaShared();
	void Init(CreateInterfaceFn, bool, CSteamAPIContext*, IGet*);
	void Shutdown();
	void DumpStats();
	ILuaInterface* CreateLuaInterface(unsigned char, bool);
	void CloseLuaInterface(ILuaInterface*);
	ILuaInterface* GetLuaInterface(unsigned char);
	File* LoadFile(const std::string& path, const std::string& pathId, bool fromDatatable, bool fromFile);
	File* GetCache(const std::string&);
	void MountLua(const char*);
	void MountLuaAdd(const char*, const char*);
	void UnMountLua(const char*);
	void SetFileContents(const char*, const char*);
	void SetLuaFindHook(LuaClientDatatableHook*);
	void FindScripts(const std::string&, const std::string&, std::vector<std::string>&);
	const char* GetStackTraces();
	void InvalidateCache(const std::string&);
	void EmptyCache();
};