#ifndef GMOD_CLUAINTERFACE

#define GMOD_CLUAINTERFACE
#include <cstdarg>
#ifndef LUA_SHARED_LIB
#include <convar.h>
#include <Bootil/Bootil.h>
#endif
#include "CLuaGameCallback.h"
#include "ILuaObject.h"

#include <lua.hpp>
#define GARRYSMOD_LUA_LUABASE_H 1

#define GMOD
#include "Types.h"

class ILuaShared;
extern ILuaShared* LuaShared();

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


struct lua_Debug;
extern void GMOD_LoadBinaryModule(lua_State* L, const char*);
extern void GMOD_UnloadBinaryModules(lua_State* L);

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

// For use with ILuaBase::PushSpecial
enum
{
	SPECIAL_GLOB,       // Global table
	SPECIAL_ENV,        // Environment table
	SPECIAL_REG,        // Registry table
};

// Use these when calling ILuaBase::GetField or ILuaBase::SetField for example,
// instead of pushing the specified table
enum
{
	INDEX_GLOBAL = -10002,  // Global table
	INDEX_ENVIRONMENT,      // Environment table
	INDEX_REGISTRY,         // Registry table
};

#ifndef CFunc
typedef int ( *CFunc )( lua_State* L );
#endif

class ILuaBase
{
public:
	// You shouldn't need to use this struct
	// Instead, use the UserType functions
	struct UserData
	{
		void* data;
		unsigned char type; // Change me to a uint32 one day
	};

protected:
	template <class T>
	struct UserData_Value : UserData
	{
		T value;
	};

public:
	// Returns the amount of values on the stack
	virtual int         Top(void) = 0;

	// Pushes a copy of the value at iStackPos to the top of the stack
	virtual void        Push(int iStackPos) = 0;

	// Pops iAmt values from the top of the stack
	virtual void        Pop(int iAmt = 1) = 0;

	// Pushes table[key] on to the stack
	// table = value at iStackPos
	// key   = value at top of the stack
	// Pops the key from the stack
	virtual void        GetTable(int iStackPos) = 0;

	// Pushes table[key] on to the stack
	// table = value at iStackPos
	// key   = strName
	virtual void        GetField(int iStackPos, const char* strName) = 0;

	// Sets table[key] to the value at the top of the stack
	// table = value at iStackPos
	// key   = strName
	// Pops the value from the stack
	virtual void        SetField(int iStackPos, const char* strName) = 0;

	// Creates a new table and pushes it to the top of the stack
	virtual void        CreateTable() = 0;

	// Sets table[key] to the value at the top of the stack
	// table = value at iStackPos
	// key   = value 2nd to the top of the stack
	// Pops the key and the value from the stack
	virtual void        SetTable(int iStackPos) = 0;

	// Sets the metatable for the value at iStackPos to the value at the top of the stack
	// Pops the value off of the top of the stack
	virtual void        SetMetaTable(int iStackPos) = 0;

	// Pushes the metatable of the value at iStackPos on to the top of the stack
	// Upon failure, returns false and does not push anything
	virtual bool        GetMetaTable(int i) = 0;

	// Calls a function
	// To use it: Push the function on to the stack followed by each argument
	// Pops the function and arguments from the stack, leaves iResults values on the stack
	// If this function errors, any local C values will not have their destructors called!
	virtual void        Call(int iArgs, int iResults) = 0;

	// Similar to Call
	// See: lua_pcall( lua_State*, int, int, int )
	virtual int         PCall(int iArgs, int iResults, int iErrorFunc) = 0;

	// Returns true if the values at iA and iB are equal
	virtual int         Equal(int iA, int iB) = 0;

	// Returns true if the value at iA and iB are equal
	// Does not invoke metamethods
	virtual int         RawEqual(int iA, int iB) = 0;

	// Moves the value at the top of the stack in to iStackPos
	// Any elements above iStackPos are shifted upwards
	virtual void        Insert(int iStackPos) = 0;

	// Removes the value at iStackPos from the stack
	// Any elements above iStackPos are shifted downwards
	virtual void        Remove(int iStackPos) = 0;

	// Allows you to iterate tables similar to pairs(...)
	// See: lua_next( lua_State*, int );
	virtual int         Next(int iStackPos) = 0;

#ifndef GMOD_ALLOW_DEPRECATED
protected:
#endif
	// Deprecated: Use the UserType functions instead of this
	virtual ILuaBase::UserData* NewUserdata(unsigned int iSize) = 0;

public:
	// Throws an error and ceases execution of the function
	// If this function is called, any local C values will not have their destructors called!
	[[noreturn]]
	virtual void        ThrowError(const char* strError) = 0;

	// Checks that the type of the value at iStackPos is iType
	// Throws and error and ceases execution of the function otherwise
	// If this function errors, any local C values will not have their destructors called!
	virtual void        CheckType(int iStackPos, int iType) = 0;

	// Throws a pretty error message about the given argument
	// If this function is called, any local C values will not have their destructors called!
	[[noreturn]]
	virtual void        ArgError(int iArgNum, const char* strMessage) = 0;

	// Pushes table[key] on to the stack
	// table = value at iStackPos
	// key   = value at top of the stack
	// Does not invoke metamethods
	virtual void        RawGet(int iStackPos) = 0;

	// Sets table[key] to the value at the top of the stack
	// table = value at iStackPos
	// key   = value 2nd to the top of the stack
	// Pops the key and the value from the stack
	// Does not invoke metamethods
	virtual void        RawSet(int iStackPos) = 0;

	// Returns the string at iStackPos. iOutLen is set to the length of the string if it is not NULL
	// If the value at iStackPos is a number, it will be converted in to a string
	// Returns NULL upon failure
	virtual const char* GetString(int iStackPos = -1, unsigned int* iOutLen = nullptr) = 0;

	// Returns the number at iStackPos
	// Returns 0 upon failure
	virtual double      GetNumber(int iStackPos = -1) = 0;

	// Returns the boolean at iStackPos
	// Returns false upon failure
	virtual bool        GetBool(int iStackPos = -1) = 0;

	// Returns the C-Function at iStackPos
	// returns NULL upon failure
	virtual CFunc       GetCFunction(int iStackPos = -1) = 0;

#if !defined( GMOD_ALLOW_DEPRECATED ) && !defined( GMOD_ALLOW_LIGHTUSERDATA )
protected:
#endif
	// Deprecated: You should probably be using the UserType functions instead of this
	virtual ILuaBase::UserData* GetUserdata(int iStackPos = -1) = 0;

public:
	// Pushes a nil value on to the stack
	virtual void        PushNil() = 0;

	// Pushes the given string on to the stack
	// If iLen is 0, strlen will be used to determine the string's length
	virtual void        PushString(const char* val, unsigned int iLen = 0) = 0;

	// Pushes the given double on to the stack
	virtual void        PushNumber(double val) = 0;

	// Pushes the given bobolean on to the stack
	virtual void        PushBool(bool val) = 0;

	// Pushes the given C-Function on to the stack
	virtual void        PushCFunction(CFunc val) = 0;

	// Pushes the given C-Function on to the stack with upvalues
	// See: GetUpvalueIndex()
	virtual void        PushCClosure(CFunc val, int iVars) = 0;


#if !defined( GMOD_ALLOW_DEPRECATED ) && !defined( GMOD_ALLOW_LIGHTUSERDATA )
protected:
#endif
	// Deprecated: Don't use light userdata in GMod
	virtual void        PushUserdata(ILuaBase::UserData*) = 0;

public:
	// Allows for values to be stored by reference for later use
	// Make sure you call ReferenceFree when you are done with a reference
	virtual int         ReferenceCreate() = 0;
	virtual void        ReferenceFree(int i) = 0;
	virtual void        ReferencePush(int i) = 0;

	// Push a special value onto the top of the stack (see SPECIAL_* enums)
	virtual void        PushSpecial(int iType) = 0;

	// Returns true if the value at iStackPos is of type iType
	// See: Types.h
	virtual bool        IsType(int iStackPos, int iType) = 0;

	// Returns the type of the value at iStackPos
	// See: Types.h
	virtual int         GetType(int iStackPos) = 0;

	// Returns the name associated with the given type ID
	// See: Types.h
	// Note: GetTypeName does not work with user-created types
	virtual const char* GetTypeName(int iType) = 0;

#ifndef GMOD_ALLOW_DEPRECATED
protected:
#endif
	// Deprecated: Use CreateMetaTable
	virtual void        CreateMetaTableType(const char* strName, int iType) = 0;

public:
	// Like Get* but throws errors and returns if they're not of the expected type
	// If these functions error, any local C values will not have their destructors called!
	virtual const char* CheckString(int iStackPos = -1) = 0;
	virtual double      CheckNumber(int iStackPos = -1) = 0;

	// Returns the length of the object at iStackPos
	// Works for: strings, tables, userdata
	virtual int         ObjLen(int iStackPos = -1) = 0;

	// Returns the angle at iStackPos
	virtual const QAngle& GetAngle(int iStackPos = -1) = 0;

	// Returns the vector at iStackPos
	virtual const Vector& GetVector(int iStackPos = -1) = 0;

	// Pushes the given angle to the top of the stack
	virtual void        PushAngle(const QAngle& val) = 0;

	// Pushes the given vector to the top of the stack
	virtual void        PushVector(const Vector& val) = 0;

	// Sets the lua_State to be used by the ILuaBase implementation
	// You don't need to use this if you use the LUA_FUNCTION macro
	virtual void        SetState(lua_State* L) = 0;

	// Pushes the metatable associated with the given type name
	// Returns the type ID to use for this type
	// If the type doesn't currently exist, it will be created
	virtual int         CreateMetaTable(const char* strName) = 0;

	// Pushes the metatable associated with the given type
	virtual bool        PushMetaTable(int iType) = 0;

	// Creates a new UserData of type iType that references the given data
	virtual void        PushUserType(void* data, int iType) = 0;

	// Sets the data pointer of the UserType at iStackPos
	// You can use this to invalidate a UserType by passing NULL
	virtual void        SetUserType(int iStackPos, void* data) = 0;

	// Returns the data of the UserType at iStackPos if it is of the given type
	template <class T>
	T* GetUserType(int iStackPos, int iType)
	{
		auto* ud = static_cast<UserData*>(GetUserdata(iStackPos));

		if (ud == nullptr || ud->data == nullptr || ud->type != iType)
			return nullptr;

		return static_cast<T*>(ud->data);
	}

	// Creates a new UserData of type iType with an instance of T
	// If your class is complex/has complex members which handle memory,
	// you might need a __gc method to clean these, as Lua won't handle them
	template <typename T>
	T* NewUserType(int iType)
	{
		UserData* ud = static_cast<UserData*>(NewUserdata(sizeof(UserData) + sizeof(T)));
		if (ud == nullptr)
			return nullptr;

		T* data = reinterpret_cast<T*>(reinterpret_cast<unsigned char*>(ud) + sizeof(UserData));
		ud->data = new(data) T;
		ud->type = static_cast<unsigned char>(iType);

		return data;
	}

	// Creates a new UserData with your own data embedded within it
	template <class T>
	void PushUserType_Value(const T& val, int iType)
	{
		using UserData_T = UserData_Value<T>;

		// The UserData allocated by CLuaInterface is only guaranteed to have a data alignment of 8
		static_assert(std::alignment_of<UserData_T>::value <= 8,
			"PushUserType_Value given type with unsupported alignment requirement");

		// Don't give this function objects that can't be trivially destructed
		// You could ignore this limitation if you implement object destruction in `__gc`
		static_assert(std::is_trivially_destructible<UserData_T>::value,
			"PushUserType_Value given type that is not trivially destructible");

		auto* ud = static_cast<UserData_T*>(NewUserdata(sizeof(UserData_T)));
		ud->data = new(&ud->value) T(val);
		ud->type = iType;

		// Set the metatable
		if (PushMetaTable(iType)) SetMetaTable(-2);
	}

	// Gets the internal lua_State
	inline lua_State* GetState() const
	{
		return state;
	}

	// Gets the environment table of the value at the given index
	inline void GetFEnv(int iStackPos)
	{
		lua_getfenv(state, iStackPos);
	}

	// Sets the environment table of the value at the given index
	inline int SetFEnv(int iStackPos)
	{
		return lua_setfenv(state, iStackPos);
	}

	// Pushes a formatted string onto the stack
	inline const char* PushFormattedString(const char* fmt, va_list args)
	{
		return lua_pushvfstring(state, fmt, args);
	}

	// Pushes a formatted string onto the stack
	inline const char* PushFormattedString(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		const char* res = PushFormattedString(fmt, args);
		va_end(args);
		return res;
	}

	// Throws an error (uses the value at the top of the stack)
	[[noreturn]]
	inline void Error()
	{
		lua_error(state);

		// Should never be reached since 'lua_error' never returns.
		std::abort();
	}

	// Throws an error (pushes a formatted string onto the stack and uses it)
	[[noreturn]]
	inline void FormattedError(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		PushFormattedString(fmt, args);
		va_end(args);
		Error();
	}

	// Throws an error related to type differences
	[[noreturn]]
	/*inline void TypeError(int iStackPos, const char* tname)
	{
		luaL_typerror(state, iStackPos, tname);

		// Should never be reached since 'luaL_typerror' never returns.
		std::abort();
	}*/

	// Converts the value at the given index to a generic C pointer (void*)
	inline const void* GetPointer(int iStackPos)
	{
		return lua_topointer(state, iStackPos);
	}

	// Calls a metamethod on the object at iStackPos
	/*inline int CallMeta(int iStackPos, const char* e)
	{
		return luaL_callmeta(state, iStackPos, e);
	}*/

	// Produces the pseudo-index of an upvalue at iPos
	static inline int GetUpvalueIndex(int iPos)
	{
		return static_cast<int>(INDEX_GLOBAL) - iPos;
	}

	// Get information about the interpreter runtime stack
	/*inline int GetStack(int level, lua_Debug* ar)
	{
		return lua_getstack(state, level, ar);
	}

	// Returns information about a specific function or function invocation
	inline int GetInfo(const char* what, lua_Debug* ar)
	{
		return lua_getinfo(state, what, ar);
	}*/

private:
	friend class CLuaInterface;

	lua_State* state;
};

// NOTE: This is a custom class! (Gmod's real class is unknown)
class ILuaThreadedCall {
public:
    virtual ~ILuaThreadedCall() {}
    virtual bool Execute() = 0;
};

#define LUA_MAX_TEMP_OBJECTS 32

class ILuaInterface : public ILuaBase
{
public:
	virtual bool Init( ILuaGameCallback *, bool ) = 0;
	virtual void Shutdown( ) = 0;
	virtual void Cycle( ) = 0;
	virtual GarrysMod::Lua::ILuaObject *Global( ) = 0;
	virtual GarrysMod::Lua::ILuaObject *GetObject( int index ) = 0;
	virtual void PushLuaObject( GarrysMod::Lua::ILuaObject *obj ) = 0;
	virtual void PushLuaFunction( CFunc func ) = 0;
	virtual void LuaError( const char *err, int index ) = 0;
	virtual void TypeError( const char *name, int index ) = 0;
	virtual void CallInternal( int args, int rets ) = 0;
	virtual void CallInternalNoReturns( int args ) = 0;
	virtual bool CallInternalGetBool( int args ) = 0;
	virtual const char *CallInternalGetString( int args ) = 0;
	virtual bool CallInternalGet( int args, GarrysMod::Lua::ILuaObject *obj ) = 0;
	virtual void NewGlobalTable( const char *name ) = 0;
	virtual GarrysMod::Lua::ILuaObject *NewTemporaryObject( ) = 0;
	virtual bool isUserData( int index ) = 0;
	virtual GarrysMod::Lua::ILuaObject *GetMetaTableObject( const char *name, int type ) = 0;
	virtual GarrysMod::Lua::ILuaObject *GetMetaTableObject( int index ) = 0;
	virtual GarrysMod::Lua::ILuaObject *GetReturn( int index ) = 0;
	virtual bool IsServer( ) = 0;
	virtual bool IsClient( ) = 0;
	virtual bool IsMenu( ) = 0;
	virtual void DestroyObject( GarrysMod::Lua::ILuaObject *obj ) = 0;
	virtual GarrysMod::Lua::ILuaObject *CreateObject( ) = 0;
	virtual void SetMember( GarrysMod::Lua::ILuaObject *table, GarrysMod::Lua::ILuaObject *key, GarrysMod::Lua::ILuaObject *value ) = 0;
	virtual GarrysMod::Lua::ILuaObject* GetNewTable( ) = 0;
	virtual void SetMember( GarrysMod::Lua::ILuaObject *table, float key ) = 0;
	virtual void SetMember( GarrysMod::Lua::ILuaObject *table, float key, GarrysMod::Lua::ILuaObject *value ) = 0;
	virtual void SetMember( GarrysMod::Lua::ILuaObject *table, const char *key ) = 0;
	virtual void SetMember( GarrysMod::Lua::ILuaObject *table, const char *key, GarrysMod::Lua::ILuaObject *value ) = 0;
	virtual void SetType( unsigned char ) = 0;
	virtual void PushLong( long num ) = 0;
	virtual int GetFlags( int index ) = 0;
	virtual bool FindOnObjectsMetaTable( int objIndex, int keyIndex ) = 0;
	virtual bool FindObjectOnTable( int tableIndex, int keyIndex ) = 0;
	virtual void SetMemberFast( GarrysMod::Lua::ILuaObject *table, int keyIndex, int valueIndex ) = 0;
	virtual bool RunString( const char *filename, const char *path, const char *stringToRun, bool run, bool showErrors ) = 0;
	virtual bool IsEqual( GarrysMod::Lua::ILuaObject *objA, GarrysMod::Lua::ILuaObject *objB ) = 0;
	virtual void Error( const char *err ) = 0;
	virtual const char *GetStringOrError( int index ) = 0;
	virtual bool RunLuaModule( const char *name ) = 0;
	virtual bool FindAndRunScript( const char *filename, bool run, bool showErrors, const char *stringToRun, bool noReturns ) = 0;
	virtual void SetPathID( const char *pathID ) = 0;
	virtual const char *GetPathID( ) = 0;
	virtual void ErrorNoHalt( const char *fmt, ... ) = 0;
	virtual void Msg( const char *fmt, ... ) = 0;
	virtual void PushPath( const char *path ) = 0;
	virtual void PopPath( ) = 0;
	virtual const char *GetPath( ) = 0;
	virtual int GetColor( int index ) = 0;
	virtual void PushColor( Color color ) = 0;
	virtual int GetStack( int level, lua_Debug *dbg ) = 0;
	virtual int GetInfo( const char *what, lua_Debug *dbg ) = 0;
	virtual const char *GetLocal( lua_Debug *dbg, int n ) = 0;
	virtual const char *GetUpvalue( int funcIndex, int n ) = 0;
	virtual bool RunStringEx( const char *filename, const char *path, const char *stringToRun, bool run, bool printErrors, bool dontPushErrors, bool noReturns ) = 0;
	virtual size_t GetDataString( int index, const char **str ) = 0;
	virtual void ErrorFromLua( const char *fmt, ... ) = 0;
	virtual const char *GetCurrentLocation( ) = 0;
	virtual void MsgColour( const Color &col, const char *fmt, ... ) = 0;
	virtual void GetCurrentFile( std::string &outStr ) = 0;
	virtual void CompileString( Bootil::Buffer &dumper, const std::string &stringToCompile ) = 0;
	virtual bool CallFunctionProtected( int, int, bool ) = 0;
	virtual void Require( const char *name ) = 0;
	virtual const char *GetActualTypeName( int type ) = 0;
	virtual void PreCreateTable( int arrelems, int nonarrelems ) = 0;
	virtual void PushPooledString( int index ) = 0;
	virtual const char *GetPooledString( int index ) = 0;
	virtual void *AddThreadedCall( ILuaThreadedCall * ) = 0;
	virtual void AppendStackTrace( char *, unsigned long ) = 0;
	virtual void *CreateConVar( const char *, const char *, const char *, int ) = 0;
	virtual void *CreateConCommand( const char *, const char *, int, void ( * )( const CCommand & ), int ( * )( const char *, char ( * )[128] ) ) = 0;
	virtual const char* CheckStringOpt( int iStackPos, const char* def ) = 0;
	virtual double CheckNumberOpt( int iStackPos, double def ) = 0;
};

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
	virtual GarrysMod::Lua::ILuaObject *Global();
	virtual GarrysMod::Lua::ILuaObject *GetObject(int index);
	virtual void PushLuaObject(GarrysMod::Lua::ILuaObject *obj);
	virtual void PushLuaFunction(CFunc func);
	virtual void LuaError(const char *err, int index);
	virtual void TypeError(const char *name, int index);
	virtual void CallInternal(int args, int rets);
	virtual void CallInternalNoReturns(int args);
	virtual bool CallInternalGetBool( int args );
	virtual const char *CallInternalGetString( int args );
	virtual bool CallInternalGet( int args, GarrysMod::Lua::ILuaObject *obj );
	virtual void NewGlobalTable( const char *name );
	virtual GarrysMod::Lua::ILuaObject *NewTemporaryObject( );
	virtual bool isUserData( int index );
	virtual GarrysMod::Lua::ILuaObject *GetMetaTableObject( const char *name, int type );
	virtual GarrysMod::Lua::ILuaObject *GetMetaTableObject( int index );
	virtual GarrysMod::Lua::ILuaObject *GetReturn( int index );
	virtual bool IsServer( );
	virtual bool IsClient( );
	virtual bool IsMenu( );
	virtual void DestroyObject( GarrysMod::Lua::ILuaObject *obj );
	virtual GarrysMod::Lua::ILuaObject *CreateObject( );
	virtual void SetMember( GarrysMod::Lua::ILuaObject *table, GarrysMod::Lua::ILuaObject *key, GarrysMod::Lua::ILuaObject *value );
	virtual GarrysMod::Lua::ILuaObject* GetNewTable( );
	virtual void SetMember( GarrysMod::Lua::ILuaObject *table, float key );
	virtual void SetMember( GarrysMod::Lua::ILuaObject *table, float key, GarrysMod::Lua::ILuaObject *value );
	virtual void SetMember( GarrysMod::Lua::ILuaObject *table, const char *key );
	virtual void SetMember( GarrysMod::Lua::ILuaObject *table, const char *key, GarrysMod::Lua::ILuaObject *value );
	virtual void SetType( unsigned char );
	virtual void PushLong( long num );
	virtual int GetFlags( int index );
	virtual bool FindOnObjectsMetaTable( int objIndex, int keyIndex );
	virtual bool FindObjectOnTable( int tableIndex, int keyIndex );
	virtual void SetMemberFast( GarrysMod::Lua::ILuaObject *table, int keyIndex, int valueIndex );
	virtual bool RunString( const char *filename, const char *path, const char *stringToRun, bool run, bool showErrors );
	virtual bool IsEqual( GarrysMod::Lua::ILuaObject *objA, GarrysMod::Lua::ILuaObject *objB );
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
	virtual void *AddThreadedCall( ILuaThreadedCall * );
	virtual void AppendStackTrace( char *, unsigned long );
	virtual void *CreateConVar( const char *, const char *, const char *, int );
	virtual void *CreateConCommand( const char *, const char *, int, void ( * )( const CCommand & ), int ( * )( const char *, char ( * )[128] ) );
	virtual const char* CheckStringOpt( int iStackPos, const char* def );
	virtual double CheckNumberOpt( int iStackPos, double def );
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
	void* _2 = nullptr;
	int _3 = 0;
	int _4 = 0;
	int _5 = 0;
	void* _6 = nullptr;
	void* _7 = nullptr;
	char _8[3];

#ifdef __APPLE__

	size_t _9; // 1 * sizeof(size_t) = 4 (x86) or 8 (x86-64) bytes

#endif

	// x86: offset of 188 bytes
	// x86-64: offset of 368 bytes
	// macOS adds an offset of 4 bytes (total 192) on x86 and 8 bytes (total 376) on x86-64
	GarrysMod::Lua::ILuaObject* m_ProtectedFunctionReturns[4];
	GarrysMod::Lua::ILuaObject* m_TempObjects[LUA_MAX_TEMP_OBJECTS];
	unsigned char m_iRealm = -1; // CLIENT = 0, SERVER = 1, MENU = 2
	ILuaGameCallback* m_pGameCallback = nullptr;
	char m_sPathID[32] = "LuaMenu"; // lsv, lsc or LuaMenu
	int m_iCurrentTempObject = 0;
	GarrysMod::Lua::ILuaObject* m_pGlobal = nullptr;
	GarrysMod::Lua::ILuaObject* m_pStringPool = nullptr;
	// But wait, theres more. In the next fields the metatables objects are saved but idk if it just has a field for each metatable or if it uses a map.
	/*char _9[44];
	GarrysMod::Lua::ILuaObject* m_pVectorMeta;
	GarrysMod::Lua::ILuaObject* m_pAngleMeta;
	char _10[36];
	GarrysMod::Lua::ILuaObject* m_pIMaterialMeta;
	GarrysMod::Lua::ILuaObject* m_pPanelMeta;
	char _11[7];
	GarrysMod::Lua::ILuaObject* m_pITextureMeta;
	char _12[4];
	GarrysMod::Lua::ILuaObject* m_pConVarMeta;
	char _13[20];
	GarrysMod::Lua::ILuaObject* m_pIVideoWriterMeta;
	GarrysMod::Lua::ILuaObject* m_pFileMeta;*/
public:
	void RunThreadedCalls();
	inline void DoStackCheck() {
		//::Msg("Top: %i\n", Top());
	}
private:
	std::list<ILuaThreadedCall*> pThreadedcalls;
	const char* m_pPath = "includes";
};

// Some functions declared inside CLuaInterface_cpp
extern CLuaError* ReadStackIntoError(lua_State* L);
extern int AdvancedLuaErrorReporter(lua_State* L);

extern ILuaInterface* CreateLuaInterface(bool bIsServer);
extern void CloseLuaInterface(ILuaInterface*);
#endif