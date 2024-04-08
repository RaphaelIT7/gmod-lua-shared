#include "gmod.h"
#include "lj_obj.h"

class ILuaBase
{
public:
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	SetState(lua_State* L) = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
	virtual void	Nothing() = 0;
};

void lua_init_stack_gmod(lua_State* L1, lua_State* L)
{
    if (L && L != L1)
	{
		L1->luabase = L->luabase;
		if (L->luabase)
			((ILuaBase*)L->luabase)->SetState(L);
	}
}