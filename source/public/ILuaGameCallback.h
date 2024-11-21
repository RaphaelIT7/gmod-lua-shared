#ifndef GMOD_ILUAGAMECALLBACK_H
#define GMOD_ILUAGAMECALLBACK_H
#include <vector>
#include <string>

class ILuaObject;
class ILuaInterface;

struct CLuaError
{
	~CLuaError()
	{
		stack.clear();

#ifndef WIN32
		if (message)
		{
			delete[] message;
			message = NULL;
		}

		if (side)
		{
			delete[] side;
			side = NULL;
		}
#endif
	}

	struct StackEntry
	{
		~StackEntry()
		{
#ifndef WIN32
			if (source)
			{
				delete[] source;
				source = NULL;
			}

			if (function)
			{
				delete[] function;
				function = NULL;
			}
#endif
		}

#ifdef _WIN32
		std::string source;
		std::string function;
#else
		const char* source = NULL;
		const char* function = NULL;
#endif
		int line = -1;
	};

#ifdef _WIN32
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
	virtual ILuaObject *CreateLuaObject() = 0;
	virtual void DestroyLuaObject(ILuaObject *pObject) = 0;
	virtual void ErrorPrint(const char *error, bool print) = 0;
	virtual void Msg(const char *msg, bool useless) = 0;
	virtual void MsgColour(const char *msg, const Color &color) = 0;
	virtual void LuaError(const CLuaError *error) = 0;
	virtual void InterfaceCreated(ILuaInterface *iface) = 0;
};
#endif