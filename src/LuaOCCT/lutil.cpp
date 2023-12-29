#include "lutil.h"

namespace luaocct {
namespace util {
namespace Curve {

static Standard_Real GetLength(const Handle(Geom_Curve) & theCurve,
                               const Standard_Real t0, const Standard_Real t1) {
  GeomAdaptor_Curve aCrv(theCurve);
  return CPnts_AbscissaPoint::Length(aCrv, t0, t1);
}

static Standard_Real GetLength(const Handle(Geom_Curve) & theCurve) {
  return GetLength(theCurve, theCurve->FirstParameter(),
                   theCurve->LastParameter());
}

} // namespace Curve
} // namespace util
} // namespace luaocct

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
