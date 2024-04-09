#include <GarrysMod/Symbol.hpp>
#include <GarrysMod/ModuleLoader.hpp>
#include <scanning/symbolfinder.hpp>
#include <detouring/hook.hpp>
#include <vector>

SymbolFinder symfinder;
void* FindFunction(void* loader, Symbol symbol)
{
	return symfinder.Resolve(loader, symbol.name.c_str(), symbol.length);
}

template<class T>
void CheckFunction(T func, const char* name)
{
	if (func == nullptr) {
		Msg("Could not locate %s symbol!\n", name);
	}
}

void* GetFunction(SourceSDK::ModuleLoader loader, const char* name, Symbol sym)
{
	void* func = FindFunction(loader.GetModule(), sym);
	CheckFunction(func, name);

	return func;
}

std::vector<Detouring::Hook*> detours = {};
void CreateDetour(Detouring::Hook* hook, const char* name, Detouring::Hook::Target target, void* func)
{
	hook->Create(target, func);
	hook->Enable();

	detours.push_back(hook);
}

struct lua_State;
typedef void (*lua_init_stack_gmod_t)(lua_State* L1, lua_State* L);
const Symbol lua_init_stack_gmodSym = Symbol::FromName("lua_init_stack_gmod");

typedef void (*GMOD_LuaPrint_t)(const char*);
const Symbol GMOD_LuaPrintSym = Symbol::FromName("GMOD_LuaPrint");