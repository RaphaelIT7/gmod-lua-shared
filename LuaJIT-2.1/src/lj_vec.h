#ifndef _LJ_VEC_H
#define _LJ_VEC_H

#include "lj_obj.h"

LJ_FUNCA GCvec3 *lj_alloc_vec3(lua_State *L, float x, float y, float z);
LJ_FUNC void LJ_FASTCALL lj_vec3_free(global_State *g, GCvec3 *cd);

LUALIB_API GCvec3 *luaL_checkvec3(lua_State *L, int idx);

#endif