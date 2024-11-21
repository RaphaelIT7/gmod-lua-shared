#ifndef GMOD_ILUACONVARS_H
#define GMOD_ILUACONVARS_H
#include <convar.h>

#define FCVAR_LUA_CLIENT			(1<<18)
#define FCVAR_LUA_SERVER			(1<<19)

#define GMOD_LUACONVARS_INTERFACE "LUACONVARS001"
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
#endif