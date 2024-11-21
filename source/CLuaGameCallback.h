#include "public/ILuaObject.h"
#include "public/ILuaGameCallback.h"

class CLuaGameCallback : public ILuaGameCallback
{
public:
	ILuaObject *CreateLuaObject();
	void DestroyLuaObject(ILuaObject* pObject);
	void ErrorPrint(const char* error, bool);
	void Msg(const char* msg, bool);
	void MsgColour(const char* msg, const Color& color);
	void LuaError(const CLuaError* error);
	void InterfaceCreated(ILuaInterface* iface);
};