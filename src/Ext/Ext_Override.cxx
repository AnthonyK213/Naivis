#include "Ext_Bind.hxx"

#include <iostream>

static int printToStdout(lua_State *L) {
  int n = lua_gettop(L);
  int i;
  lua_getglobal(L, "tostring");

  for (i = 1; i <= n; i++) {
    const char *s;
    lua_pushvalue(L, -1); /* function to be called */
    lua_pushvalue(L, i);  /* value to print */
    lua_call(L, 1, 1);
    s = lua_tostring(L, -1); /* get result */

    if (s == NULL)
      return luaL_error(
          L, LUA_QL("tostring") " must return a string to " LUA_QL("print"));

    if (i > 1)
      std::cout << '\t';

    std::cout << s;
    lua_pop(L, 1); /* pop result */
  }

  std::cout << std::endl;

  return 0;
}

static const struct luaL_Reg overrideLib[] = {
    {"print", printToStdout},
    {NULL, NULL},
};

void Ext_Override(lua_State *L) {
  lua_getglobal(L, "_G");
  luaL_setfuncs(L, overrideLib, 0);
  lua_pop(L, 1);
}
