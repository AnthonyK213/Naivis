#include "Ext_Load.hxx"

void Ext_Load(lua_State *L) {
  Ext_Qt(L);
  Ext_NaiveDoc(L);
}
