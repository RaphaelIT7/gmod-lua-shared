#include <convar.h>
#include <KeyValues.h>
#include <vector>

class ILuaConVars
{
public:
	//virtual ~ILuaConVars( ) = 0;
	virtual void Init( ) = 0;
	virtual ConVar *CreateConVar( const char *, const char *, const char *, int ) = 0;
	virtual ConCommand *CreateConCommand( const char *, const char *, int, FnCommandCallback_t, FnCommandCompletionCallback) = 0;
	virtual void DestroyManaged( ) = 0;
	virtual void Cache( const char *, const char * ) = 0;
	virtual void ClearCache( ) = 0;
};

struct ManagedCVar
{
	const char* name;
	bool iscvar;
	ConCommand* cmd;
	ConVar* cvar;
};

class CLuaConVars : public ILuaConVars
{
public:
	~CLuaConVars();
	void Init();
	ConVar* CreateConVar(const char*, const char*, const char*, int);
	ConCommand* CreateConCommand(const char*, const char*, int, FnCommandCallback_t, FnCommandCompletionCallback);
	void DestroyManaged();
	void Cache(const char*, const char*);
	void ClearCache();
private:
	KeyValues* pClientCVars; // Idk the real name
	KeyValues* pServerCVars;
	std::vector<ManagedCVar*> pManagedCVars;
};

extern CLuaConVars* LuaConVars();