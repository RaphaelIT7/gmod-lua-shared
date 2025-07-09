#define lj_vec_c
#define LUA_CORE

#include "lj_vec.h"
#include "lj_gc.h"

GCvec3 *lj_alloc_vec3(lua_State *L, float x, float y, float z)
{
  GCvec3 *v = (GCvec3 *)lj_mem_newobj(L, GCvec3);
  v->gct = ~LJ_TVEC3;

  v->x = x;
  v->y = y;
  v->z = z;

  GCtab *mt = tabref(basemt_it(G(L), LJ_TVEC3));
  setgcref(v->metatable, obj2gco(mt));

  return v;
}

/* Free a C data object. */
void LJ_FASTCALL lj_vec3_free(global_State *g, GCvec3 *vec3)
{
  lj_mem_free(g, vec3, sizeof(GCvec3));
}