/*
** Bit manipulation library.
** Copyright (C) 2005-2025 Mike Pall. See Copyright Notice in luajit.h
*/

#define lib_vector_c
#define LUA_LIB

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "lj_obj.h"
#include "lj_err.h"
#include "lj_buf.h"
#include "lj_strscan.h"
#include "lj_strfmt.h"
#if LJ_HASFFI
#include "lj_ctype.h"
#include "lj_cdata.h"
#include "lj_cconv.h"
#include "lj_carith.h"
#endif
#include "lj_ff.h"
#include "lj_lib.h"
#include "lj_vec.h"

/* ------------------------------------------------------------------------ */

#define LJLIB_MODULE_vector

LJLIB_CF(vector_new)   LJLIB_REC(.)
{
  float x = (float)luaL_optnumber(L, 1, 0.0);
  float y = (float)luaL_optnumber(L, 2, 0.0);
  float z = (float)luaL_optnumber(L, 3, 0.0);

  GCvec3 *v = lj_alloc_vec3(L, x, y, z);
  setvec3V(L, L->top++, v);

  return 1;
}

LJLIB_CF(vector___tostring)
{
  GCvec3 *v = luaL_checkvec3(L, 1);
  luaL_argcheck(L, v != NULL, 1, "vector expected");

  SBuf *sb = lj_buf_tmp_(L);
  lj_strfmt_putfnum(sb, STRFMT_G14, v->x);
  lj_buf_putchar(sb, ' ');
  lj_strfmt_putfnum(sb, STRFMT_G14, v->y);
  lj_buf_putchar(sb, ' ');
  lj_strfmt_putfnum(sb, STRFMT_G14, v->z);

  setstrV(L, L->top++, lj_buf_str(L, sb));
  return 1;
}

/* ------------------------------------------------------------------------ */

#include "lj_libdef.h"

LUALIB_API int luaopen_vector(lua_State *L)
{
  luaL_newmetatable(L, "VectorMT");
    lua_pushcfunction(L, lj_cf_vector___tostring);
    lua_setfield(L, -2, "__tostring");
  setgcref(basemt_it(G(L), LJ_TVEC3), obj2gco(tabV(L->top - 1)));

  LJ_LIB_REG(L, LUA_VECLIBNAME, vector);
  return 1;
}