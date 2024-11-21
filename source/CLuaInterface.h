#ifndef GMOD_CLUAINTERFACE

#define GMOD_CLUAINTERFACE
#include <cstdarg>
#ifndef LUA_SHARED_LIB
#include <convar.h>
#include <Bootil/Bootil.h>
#endif
#include "CLuaGameCallback.h"
#include "public/ILuaObject.h"
#include "public/ILuaInterface.h"
#define GARRYSMOD_LUA_LUABASE_H 1

#define GMOD

#ifdef BUILD_GMOD
#ifdef _WIN32
#define WIN32
#else
#define WIN64
#endif
#endif

class ILuaShared;
extern ILuaShared* LuaShared();

extern void DebugPrint(int level, const char* fmt, ...);

struct lua_Debug;
extern "C"
{
	extern void GMOD_LoadBinaryModule(lua_State* L, const char*);
	extern void GMOD_UnloadBinaryModules(lua_State* L);
}

// ToDo: Verify and Add GMODLUA_GetUserType and edit lua_typename to call this function.
/*
void GMODLUA_GetUserType(lua_State* L, int index) {
    if (!lua_isuserdata(L, index)) {
        return; // Not a userdata
    }

    // Get metatable of the userdata
    lua_getmetatable(L, index);
    
    // Set "MetaName" field to "UserData"
    lua_pushstring(L, "MetaName");
    lua_pushstring(L, "UserData");
    lua_settable(L, -3);

    // Check if the metatable already has a string field
    lua_pushstring(L, "ExistingField");
    lua_gettable(L, -2);
    const char* existingField = nullptr;
    if (lua_isstring(L, -1)) {
        existingField = lua_tostring(L, -1);
    }

    // If there is an existing string field, copy it to a buffer
    char buffer[128];
    if (existingField) {
        strncpy(buffer, existingField, sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0'; // Ensure null-terminated
    } else {
        buffer[0] = '\0'; // No existing string field
    }

    // Clean up the Lua stack
    lua_pop(L, 2); // Pop the existing field and metatable

    // If you want to use the existing string field in C++, you can use the 'buffer' variable
}
*/

// ToDo: Add GMOD_LuaPrint and edit lj_cf_print to call this function.

// ToDo: Verify and Add GMOD_LuaCreateEmptyUserdata and edit lj_cf_newproxy to callt this function
/*
void GMOD_LuaCreateEmptyUserdata(LuaState* L) {
	void* userdata = lua_newuserdata(L, 0);
}
*/

extern int g_iTypeNum;

class CLuaInterface : public ILuaInterface
{
public:
	virtual int Top(void);
	virtual void Push(int iStackPos);
	virtual void Pop(int iAmt = 1);
	virtual void GetTable(int iStackPos);
	virtual void GetField(int iStackPos, const char* strName);
	virtual void SetField(int iStackPos, const char* strName);
	virtual void CreateTable();
	virtual void SetTable(int iStackPos);
	virtual void SetMetaTable(int iStackPos);
	virtual bool GetMetaTable(int i);
	virtual void Call(int iArgs, int iResults);
	virtual int PCall(int iArgs, int iResults, int iErrorFunc);
	virtual int Equal(int iA, int iB);
	virtual int RawEqual(int iA, int iB);
	virtual void Insert(int iStackPos);
	virtual void Remove(int iStackPos);
	virtual int Next(int iStackPos);
	virtual ILuaBase::UserData* NewUserdata(unsigned int iSize);
	[[noreturn]]
	virtual void ThrowError(const char* strError);
	virtual void CheckType(int iStackPos, int iType);
	[[noreturn]]
	virtual void ArgError(int iArgNum, const char* strMessage);
	virtual void RawGet(int iStackPos);
	virtual void RawSet(int iStackPos);
	virtual const char* GetString(int iStackPos = -1, unsigned int* iOutLen = nullptr);
	virtual double GetNumber(int iStackPos = -1);
	virtual bool GetBool(int iStackPos = -1);
	virtual CFunc GetCFunction(int iStackPos = -1);
	virtual ILuaBase::UserData* GetUserdata(int iStackPos = -1);
	virtual void PushNil();
	virtual void PushString(const char* val, unsigned int iLen = 0);
	virtual void PushNumber(double val);
	virtual void PushBool(bool val);
	virtual void PushCFunction(CFunc val);
	virtual void PushCClosure(CFunc val, int iVars);
	virtual void PushUserdata(ILuaBase::UserData*);
	virtual int ReferenceCreate();
	virtual void ReferenceFree(int i);
	virtual void ReferencePush(int i);
	virtual void PushSpecial(int iType);
	virtual bool IsType(int iStackPos, int iType);
	virtual int GetType(int iStackPos);
	virtual const char* GetTypeName(int iType);
	virtual void CreateMetaTableType(const char* strName, int iType);
	virtual const char* CheckString(int iStackPos = -1);
	virtual double CheckNumber(int iStackPos = -1);
	virtual int ObjLen(int iStackPos = -1);
	virtual const QAngle& GetAngle(int iStackPos = -1);
	virtual const Vector& GetVector(int iStackPos = -1);
	virtual void PushAngle(const QAngle& val);
	virtual void PushVector(const Vector& val);
	virtual void SetState(lua_State* L);
	virtual int CreateMetaTable(const char* strName);
	virtual bool PushMetaTable(int iType);
	virtual void PushUserType(void* data, int iType);
	virtual void SetUserType(int iStackPos, void* data);

public:
	virtual bool Init(ILuaGameCallback *, bool);
	virtual void Shutdown();
	virtual void Cycle();
	virtual ILuaObject *Global();
	virtual ILuaObject *GetObject(int index);
	virtual void PushLuaObject(ILuaObject *obj);
	virtual void PushLuaFunction(CFunc func);
	virtual void LuaError(const char *err, int index);
	virtual void TypeError(const char *name, int index);
	virtual void CallInternal(int args, int rets);
	virtual void CallInternalNoReturns(int args);
	virtual bool CallInternalGetBool( int args );
	virtual const char *CallInternalGetString( int args );
	virtual bool CallInternalGet( int args, ILuaObject *obj );
	virtual void NewGlobalTable( const char *name );
	virtual ILuaObject *NewTemporaryObject( );
	virtual bool isUserData( int index );
	virtual ILuaObject *GetMetaTableObject( const char *name, int type );
	virtual ILuaObject *GetMetaTableObject( int index );
	virtual ILuaObject *GetReturn( int index );
	virtual bool IsServer( );
	virtual bool IsClient( );
	virtual bool IsMenu( );
	virtual void DestroyObject( ILuaObject *obj );
	virtual ILuaObject *CreateObject( );
	virtual void SetMember( ILuaObject *table, ILuaObject *key, ILuaObject *value );
	virtual ILuaObject* GetNewTable( );
	virtual void SetMember( ILuaObject *table, float key );
	virtual void SetMember( ILuaObject *table, float key, ILuaObject *value );
	virtual void SetMember( ILuaObject *table, const char *key );
	virtual void SetMember( ILuaObject *table, const char *key, ILuaObject *value );
	virtual void SetType( unsigned char );
	virtual void PushLong( long num );
	virtual int GetFlags( int index );
	virtual bool FindOnObjectsMetaTable( int objIndex, int keyIndex );
	virtual bool FindObjectOnTable( int tableIndex, int keyIndex );
	virtual void SetMemberFast( ILuaObject *table, int keyIndex, int valueIndex );
	virtual bool RunString( const char *filename, const char *path, const char *stringToRun, bool run, bool showErrors );
	virtual bool IsEqual( ILuaObject *objA, ILuaObject *objB );
	virtual void Error( const char *err );
	virtual const char *GetStringOrError( int index );
	virtual bool RunLuaModule( const char *name );
	virtual bool FindAndRunScript( const char *filename, bool run, bool showErrors, const char *stringToRun, bool noReturns );
	virtual void SetPathID( const char *pathID );
	virtual const char *GetPathID( );
	virtual void ErrorNoHalt( const char *fmt, ... );
	virtual void Msg( const char *fmt, ... );
	virtual void PushPath( const char *path );
	virtual void PopPath( );
	virtual const char *GetPath( );
	virtual int GetColor( int index );
	virtual void PushColor( Color color );
	virtual int GetStack( int level, lua_Debug *dbg );
	virtual int GetInfo( const char *what, lua_Debug *dbg );
	virtual const char *GetLocal( lua_Debug *dbg, int n );
	virtual const char *GetUpvalue( int funcIndex, int n );
	virtual bool RunStringEx( const char *filename, const char *path, const char *stringToRun, bool run, bool printErrors, bool dontPushErrors, bool noReturns );
	virtual size_t GetDataString( int index, const char **str );
	virtual void ErrorFromLua( const char *fmt, ... );
	virtual const char *GetCurrentLocation( );
	virtual void MsgColour( const Color &col, const char *fmt, ... );
	virtual void GetCurrentFile( std::string &outStr );
	virtual void CompileString( Bootil::Buffer &dumper, const std::string &stringToCompile );
	virtual bool CallFunctionProtected( int iArgs, int iRets, bool showError );
	virtual void Require( const char *name );
	virtual const char *GetActualTypeName( int type );
	virtual void PreCreateTable( int arrelems, int nonarrelems );
	virtual void PushPooledString( int index );
	virtual const char *GetPooledString( int index );
	virtual int AddThreadedCall( ILuaThreadedCall * );
	virtual void AppendStackTrace( char *, unsigned long );
	virtual void *CreateConVar( const char *, const char *, const char *, int );
	virtual void *CreateConCommand( const char *, const char *, int, void ( * )( const CCommand & ), int ( * )( const char *, char ( * )[128] ) );
	virtual const char* CheckStringOpt( int iStackPos, const char* def );
	virtual double CheckNumberOpt( int iStackPos, double def );
	virtual void RegisterMetaTable( const char* name, ILuaObject* obj );

public:
	virtual void LuaPrint( const char* str );

public:
	std::string RunMacros(std::string script);

public:
	inline ILuaGameCallback *GetLuaGameCallback() const
	{
		return m_pGameCallback;
	}

	inline void SetLuaGameCallback( ILuaGameCallback *callback )
	{
		m_pGameCallback = callback;
	}
private:
	// vtable: 1 * sizeof(void **) = 4 (x86) or 8 (x86-64) bytes
	// luabase: 1 * sizeof(LuaBase *) = 4 (x86) or 8 (x86-64) bytes

	// These members represent nothing in particular
	// They've been selected to fill the required space between the vtable and the callback object
	int _1 = 1; // Always 1?
	char* m_sCurrentPath = new char[32]; // not how gmod does it :/
	int _3 = 0;
	int _4 = 0;
	int m_iPushedPaths = 0;
	const char* m_sLastPath = NULL;
	std::list<ILuaThreadedCall*> m_pThreadedCalls;

#ifdef __APPLE__

	size_t _7; // 1 * sizeof(size_t) = 4 (x86) or 8 (x86-64) bytes

#endif

	// x86: offset of 188 bytes
	// x86-64: offset of 368 bytes
	// macOS adds an offset of 4 bytes (total 192) on x86 and 8 bytes (total 376) on x86-64
	ILuaObject* m_ProtectedFunctionReturns[4];
	ILuaObject* m_TempObjects[LUA_MAX_TEMP_OBJECTS];
	unsigned char m_iRealm = -1; // CLIENT = 0, SERVER = 1, MENU = 2
	ILuaGameCallback* m_pGameCallback = nullptr;
	char m_sPathID[32] = "LuaMenu"; // lsv, lsc or LuaMenu
	int m_iCurrentTempObject = 0;
	ILuaObject* m_pGlobal = nullptr;
	ILuaObject* m_pStringPool = nullptr;
	// But wait, theres more. In the next fields the metatables objects are saved but idk if it just has a field for each metatable or if it uses a map.
	char _8[1];
	char _9[39];
	ILuaObject* m_pWeaponMeta = nullptr;
	ILuaObject* m_pVectorMeta = nullptr;
	ILuaObject* m_pAngleMeta = nullptr;
	ILuaObject* m_pPhysObjMeta = nullptr;
	ILuaObject* m_pISaveMeta = nullptr;
	ILuaObject* m_pIRestoreMeta = nullptr;
	ILuaObject* m_pCTakeDamageInfoMeta = nullptr;
	ILuaObject* m_pCEffectDataMeta = nullptr;
	ILuaObject* m_pCMoveDataMeta = nullptr;
	ILuaObject* m_pCRecipientFilterMeta = nullptr;
	ILuaObject* m_pCUserCmd = nullptr;
	ILuaObject* m_unknown1 = nullptr; // Probably only Menu state
	ILuaObject* m_pIMaterialMeta = nullptr;
	ILuaObject* m_pPanelMeta = nullptr;
	ILuaObject* m_pCLuaParticleMeta = nullptr;
	char _10[3];
	ILuaObject* m_pITextureMeta = nullptr;
	ILuaObject* m_pBf_readMeta = nullptr;
	ILuaObject* m_pConVarMeta = nullptr;
	ILuaObject* m_pIMeshMeta = nullptr;
	ILuaObject* m_pVMatrixMeta = nullptr;
	ILuaObject* m_pCSoundPatchMeta = nullptr;
	ILuaObject* m_pPixelvis_handle_tMeta = nullptr;
	ILuaObject* m_pDlight_tMeta = nullptr;
	ILuaObject* m_pIVideoWriterMeta = nullptr;
	ILuaObject* m_pFileMeta = nullptr;
	ILuaObject* m_pCLuaLocomotionMeta = nullptr;
	ILuaObject* m_pPathFollowerMeta = nullptr;
	ILuaObject* m_pCNavAreaMeta = nullptr;
	ILuaObject* m_pIGModAudioChannelMeta = nullptr;
	ILuaObject* m_pCNavLadderMeta = nullptr;
	ILuaObject* m_pCNewParticleEffectMeta = nullptr;
	ILuaObject* m_pProjectedTextureMeta = nullptr;
	ILuaObject* m_pPhysCollideMeta = nullptr;
	ILuaObject* m_pSurfaceInfoMeta = nullptr;
private:
	std::list<char*> m_pPaths;

public:
	void RunThreadedCalls();
	inline void DoStackCheck() {
		DebugPrint(2, "Top: %i\n", Top());
	}
};

// Some functions declared inside CLuaInterface_cpp
extern CLuaError* ReadStackIntoError(lua_State* L);
extern int AdvancedLuaErrorReporter(lua_State* L);

extern ILuaInterface* CreateLuaInterface(bool bIsServer);
extern void CloseLuaInterface(ILuaInterface*);
#endif