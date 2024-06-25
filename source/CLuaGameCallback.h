#include "ILuaObject.h"
#include <vector>
class ILuaInterface;

struct CLuaError
{
	~CLuaError();
	struct StackEntry
	{
		~StackEntry();
#ifdef WIN32
		std::string source;
		std::string function;
#else
		const char* source = NULL;
		const char* function = NULL;
#endif
		int line = -1;
	};

#ifdef WIN32
	std::string message;
	std::string side;
#else
	const char* message = NULL;
	const char* side = NULL;
#endif
	std::vector<StackEntry> stack;
};

class ILuaGameCallback
{
public:
	virtual GarrysMod::Lua::ILuaObject *CreateLuaObject() = 0;
	virtual void DestroyLuaObject(GarrysMod::Lua::ILuaObject *pObject) = 0;
	virtual void ErrorPrint(const char *error, bool print) = 0;
	virtual void Msg(const char *msg, bool useless) = 0;
	virtual void MsgColour(const char *msg, const Color &color) = 0;
	virtual void LuaError(const CLuaError *error) = 0;
	virtual void InterfaceCreated(ILuaInterface *iface) = 0;
};

class CLuaGameCallback : public ILuaGameCallback
{
public:
	GarrysMod::Lua::ILuaObject *CreateLuaObject();
	void DestroyLuaObject(GarrysMod::Lua::ILuaObject* pObject);
	void ErrorPrint(const char* error, bool);
	void Msg(const char* msg, bool);
	void MsgColour(const char* msg, const Color& color);
	void LuaError(const CLuaError* error);
	void InterfaceCreated(ILuaInterface* iface);
};