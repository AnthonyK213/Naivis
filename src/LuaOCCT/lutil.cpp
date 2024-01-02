#include "lutil.h"

#include "util_curve.h"

void luaocct_init_util(lua_State *L) {
  LuaBridge__G(L)
      .Begin_Namespace2(util, Curve)
      .addFunction(
          "GetLength",
          luabridge::overload<const Handle(Geom_Curve) &>(
              &luaocct::util::Curve::GetLength),
          luabridge::overload<const Handle(Geom_Curve) &, const Standard_Real,
                              const Standard_Real>(
              &luaocct::util::Curve::GetLength))
      .End_Namespace2();

  LuaBridge__G(L).Begin_Namespace2(util, Shape).End_Namespace2();
}
