#include "public/ILuaConVars.h"
#include <vector>
#ifndef BUILD_GMOD
#include "Platform.hpp"
#endif
#include <keyvalues.h>

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

extern ILuaConVars* LuaConVars();