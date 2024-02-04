#include "Ext_Util.hxx"

#include <Util/Util_OCAF.hxx>

void Ext_Util(lua_State *L) {
  LuaBridge__G(L)
      .Begin_Namespace(Naivis)

      .Begin_Namespace(Util_OCAF)
      .Bind_Method(Util_OCAF, GetXcafNodeName)
      .End_Namespace()

      .End_Namespace();
}
