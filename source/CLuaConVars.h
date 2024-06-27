#include <convar.h>
#include <vector>
#ifndef BUILD_GMOD
#include "Platform.hpp"
#endif

#ifdef ARCHITECTURE_X86_64
#include <keyvalues.h>
#else
#include <KeyValues.h>
#endif

class ILuaConVars
{
public:
	virtual ~ILuaConVars() {};
	virtual void Init( ) = 0;
	virtual ConVar *CreateConVar( const char *, const char *, const char *, int ) = 0;
	virtual ConCommand *CreateConCommand( const char *, const char *, int, FnCommandCallback_t, FnCommandCompletionCallback) = 0;
	virtual void DestroyManaged( ) = 0;
	virtual void Cache( const char *, const char * ) = 0;
	virtual void ClearCache( ) = 0;
};

struct ManagedConVar
{
	const char* name;
	bool iscvar;
	ConCommandBase* var;
	char* nameStr;
	char* valStr;
	char* helpStr;
};

class CLuaConVars : public ILuaConVars
{
public:
	virtual ~CLuaConVars();
	virtual void Init();
	virtual ConVar* CreateConVar(const char*, const char*, const char*, int);
	virtual ConCommand* CreateConCommand(const char*, const char*, int, FnCommandCallback_t, FnCommandCompletionCallback);
	virtual void DestroyManaged();
	virtual void Cache(const char*, const char*);
	virtual void ClearCache();
private:
	KeyValues* pClientCVars; // Idk the real name
	KeyValues* pServerCVars;
	std::vector<ManagedConVar*> pManagedCVars;
};

extern CLuaConVars* LuaConVars();