#include "Ext_Load.hxx"

extern void Ext_Ghost(lua_State *L);
extern void Ext_NaiveApp(lua_State *L);
extern void Ext_NaiveDoc(lua_State *L);
extern void Ext_Override(lua_State *L);
extern void Ext_Qt(lua_State *L);

void Ext_Load(lua_State *L) {
  Ext_Qt(L);
  Ext_NaiveApp(L);
  Ext_NaiveDoc(L);
  Ext_Ghost(L);
  Ext_Override(L);
}
