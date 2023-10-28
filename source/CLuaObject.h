#pragma once

#ifndef CFunc
typedef int ( *CFunc )( lua_State* L );
#endif

class BaseEntity;
class IPhysicsObject;

class ILuaObject
{
public:
	virtual void Set(ILuaObject *obj) = 0;
	virtual void SetFromStack(int i) = 0;
	virtual void UnReference() = 0;

	virtual int GetType() = 0;
	virtual const char *GetString() = 0;
	virtual float GetFloat() = 0;
	virtual int GetInt() = 0;
	virtual void *GetUserData() = 0;

	virtual void SetMember(const char *name) = 0;
	virtual void SetMember(const char *name, ILuaObject *obj) = 0;
	virtual void SetMember(const char *name, float val) = 0;
	virtual void SetMember(const char *name, bool val) = 0;
	virtual void SetMember(const char *name, const char *val) = 0;
	virtual void SetMember(const char *name, CFunc f) = 0;

	virtual bool GetMemberBool(const char *name, bool b = true) = 0;
	virtual int GetMemberInt(const char *name, int i = 0) = 0;
	virtual float GetMemberFloat(const char *name, float f = 0.0f) = 0;
	virtual const char *GetMemberStr(const char *name, const char *s = "") = 0;
	virtual void *GetMemberUserData(const char *name, void *u = 0) = 0;
	virtual void *GetMemberUserData(float name, void *u = 0) = 0;
	virtual ILuaObject *GetMember(const char *name) = 0;
	virtual ILuaObject *GetMember(ILuaObject *key) = 0;

	virtual void SetMetaTable(ILuaObject *obj) = 0;
	virtual void SetUserData(void *obj) = 0;

	virtual void Push() = 0;

	virtual bool isNil() = 0;
	virtual bool isTable() = 0;
	virtual bool isString() = 0;
	virtual bool isNumber() = 0;
	virtual bool isFunction() = 0;
	virtual bool isUserData() = 0;

	virtual ILuaObject *GetMember(float fKey) = 0;

	virtual void *Remove_Me_1(const char *name, void * = 0) = 0;

	virtual void SetMember(float fKey) = 0;
	virtual void SetMember(float fKey, ILuaObject *obj) = 0;
	virtual void SetMember(float fKey, float val) = 0;
	virtual void SetMember(float fKey, bool val) = 0;
	virtual void SetMember(float fKey, const char *val) = 0;
	virtual void SetMember(float fKey, CFunc f) = 0;

	virtual const char *GetMemberStr(float name, const char *s = "") = 0;

	virtual void SetMember(ILuaObject *k, ILuaObject *v) = 0;
	virtual bool GetBool() = 0;

	virtual bool PushMemberFast(int iStackPos) = 0;
	virtual void SetMemberFast(int iKey, int iValue) = 0;

	virtual void SetFloat(float val) = 0;
	virtual void SetString(const char *val) = 0;

	virtual double GetDouble() = 0;

	virtual void SetMember_FixKey(const char *, float) = 0;
	virtual void SetMember_FixKey(const char *, const char *) = 0;
	virtual void SetMember_FixKey(const char *, ILuaObject *) = 0;
	virtual void SetMember_FixKey(const char *, double) = 0;
	virtual void SetMember_FixKey(const char *, int) = 0;

	virtual bool isBool() = 0;

	virtual void SetMemberDouble(const char *, double) = 0;

	virtual void SetMemberNil(const char *) = 0;
	virtual void SetMemberNil(float) = 0;

	virtual bool RemoveMe() = 0;

	virtual void Init() = 0;

	virtual void SetFromGlobal(const char *) = 0;

	virtual int GetStringLen(unsigned int *) = 0;

	virtual unsigned int GetMemberUInt(const char *, unsigned int) = 0;

	virtual void SetMember(const char *, unsigned long long) = 0;
	virtual void SetReference(int) = 0;

	virtual void RemoveMember(const char *) = 0;
	virtual void RemoveMember(float) = 0;

	virtual bool MemberIsNil(const char *) = 0;

	virtual void SetMemberDouble(float, double) = 0;
	virtual double GetMemberDouble(const char *, double) = 0;

	virtual BaseEntity *GetMemberEntity(const char *, BaseEntity *) = 0;
	virtual void SetMemberEntity(float, BaseEntity *) = 0;
	virtual void SetMemberEntity(const char *, BaseEntity *) = 0;
	virtual bool isEntity() = 0;
	virtual BaseEntity *GetEntity() = 0;
	virtual void SetEntity(BaseEntity *) = 0;

	virtual void SetMemberVector(const char *, Vector *) = 0;
	virtual void SetMemberVector(const char *, Vector &) = 0;
	virtual void SetMemberVector(float, Vector *) = 0;
	virtual Vector *GetMemberVector(const char *, const Vector *) = 0;
	virtual Vector *GetMemberVector(int) = 0;
	virtual Vector *GetVector() = 0;
	virtual bool isVector() = 0;

	virtual void SetMemberAngle(const char *, QAngle *) = 0;
	virtual QAngle *GetMemberAngle(const char *, QAngle *) = 0;
	virtual QAngle *GetAngle() = 0;
	virtual bool isAngle() = 0;

	virtual void SetMemberPhysObject(const char *, IPhysicsObject *) = 0;
};

class CLuaObject : public ILuaObject
{
public:
	virtual void Set(ILuaObject *obj) {};
	virtual void SetFromStack(int i) {};
	virtual void UnReference() {};

	virtual int GetType() { return 0; };
	virtual const char *GetString() { return ""; };
	virtual float GetFloat() { return 0; };
	virtual int GetInt() { return 0; };
	virtual void *GetUserData() { return nullptr; };

	virtual void SetMember(const char *name) {};
	virtual void SetMember(const char *name, ILuaObject *obj) {};
	virtual void SetMember(const char *name, float val) {};
	virtual void SetMember(const char *name, bool val) {};
	virtual void SetMember(const char *name, const char *val) {};
	virtual void SetMember(const char *name, CFunc f) {};

	virtual bool GetMemberBool(const char *name, bool b = true) { return false; };
	virtual int GetMemberInt(const char *name, int i = 0) { return 0; };
	virtual float GetMemberFloat(const char *name, float f = 0.0f) { return 0; };
	virtual const char *GetMemberStr(const char *name, const char *s = "") { return ""; };
	virtual void *GetMemberUserData(const char *name, void *u = 0) { return nullptr; };
	virtual void *GetMemberUserData(float name, void *u = 0) { return nullptr; };
	virtual ILuaObject *GetMember(const char *name) { return nullptr; };
	virtual ILuaObject *GetMember(ILuaObject *key) { return nullptr; };

	virtual void SetMetaTable(ILuaObject *obj) {};
	virtual void SetUserData(void *obj) {};

	virtual void Push() {};

	virtual bool isNil() { return false; };
	virtual bool isTable() { return false; };
	virtual bool isString() { return false; };
	virtual bool isNumber() { return false; };
	virtual bool isFunction() { return false; };
	virtual bool isUserData() { return false; };

	virtual ILuaObject *GetMember(float fKey) { return nullptr; };

	virtual void *Remove_Me_1(const char *name, void * = 0) { return nullptr; };

	virtual void SetMember(float fKey) {};
	virtual void SetMember(float fKey, ILuaObject *obj) {};
	virtual void SetMember(float fKey, float val) {};
	virtual void SetMember(float fKey, bool val) {};
	virtual void SetMember(float fKey, const char *val) {};
	virtual void SetMember(float fKey, CFunc f) {};

	virtual const char *GetMemberStr(float name, const char *s = "") { return ""; };

	virtual void SetMember(ILuaObject *k, ILuaObject *v) {};
	virtual bool GetBool() { return false; };

	virtual bool PushMemberFast(int iStackPos) { return false; };
	virtual void SetMemberFast(int iKey, int iValue) {};

	virtual void SetFloat(float val) {};
	virtual void SetString(const char *val) {};

	virtual double GetDouble() { return 0; };

	virtual void SetMember_FixKey(const char *, float) {};
	virtual void SetMember_FixKey(const char *, const char *) {};
	virtual void SetMember_FixKey(const char *, ILuaObject *) {};
	virtual void SetMember_FixKey(const char *, double) {};
	virtual void SetMember_FixKey(const char *, int) {};

	virtual bool isBool() { return false; };

	virtual void SetMemberDouble(const char *, double) {};

	virtual void SetMemberNil(const char *) {};
	virtual void SetMemberNil(float) {};

	virtual bool RemoveMe() { return false; };

	virtual void Init() {};

	virtual void SetFromGlobal(const char *) {};

	virtual int GetStringLen(unsigned int *) { return 0; };

	virtual unsigned int GetMemberUInt(const char *, unsigned int) { return 0; };

	virtual void SetMember(const char *, unsigned long long) {};
	virtual void SetReference(int) {};

	virtual void RemoveMember(const char *) {};
	virtual void RemoveMember(float) {};

	virtual bool MemberIsNil(const char *) { return false; };

	virtual void SetMemberDouble(float, double) {};
	virtual double GetMemberDouble(const char *, double) { return 0; };

	virtual BaseEntity *GetMemberEntity(const char *, BaseEntity *) { return nullptr; };
	virtual void SetMemberEntity(float, BaseEntity *) {};
	virtual void SetMemberEntity(const char *, BaseEntity *) {};
	virtual bool isEntity() { return false; };
	virtual BaseEntity *GetEntity() { return nullptr; };
	virtual void SetEntity(BaseEntity *) {};

	virtual void SetMemberVector(const char *, Vector *) {};
	virtual void SetMemberVector(const char *, Vector &) {};
	virtual void SetMemberVector(float, Vector *) {};
	virtual Vector *GetMemberVector(const char *, const Vector *) { return nullptr; };
	virtual Vector *GetMemberVector(int) { return nullptr; };
	virtual Vector *GetVector() { return nullptr; };
	virtual bool isVector() { return false; };

	virtual void SetMemberAngle(const char *, QAngle *) {};
	virtual QAngle *GetMemberAngle(const char *, QAngle *) { return nullptr; };
	virtual QAngle *GetAngle() { return nullptr; };
	virtual bool isAngle() { return false; };

	virtual void SetMemberPhysObject(const char *, IPhysicsObject *) {};
};