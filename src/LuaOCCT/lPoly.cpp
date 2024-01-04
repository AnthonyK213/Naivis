#include "lPoly.h"

#include <Poly_TriangulationParameters.hxx>

void luaocct_init_Poly(lua_State *L) {
  LuaBridge__G(L)
      .Begin_Namespace1(Poly)

      .Begin_Class(Poly_Triangle)
      .addConstructor<void(),
                      void(const Standard_Integer, const Standard_Integer,
                           const Standard_Integer)>()
      .addFunction(
          "Set",
          luabridge::overload<const Standard_Integer, const Standard_Integer,
                              const Standard_Integer>(&Poly_Triangle::Set),
          luabridge::overload<const Standard_Integer, const Standard_Integer>(
              &Poly_Triangle::Set))
      .addFunction(
          "Get",
          +[](const Poly_Triangle &self) -> std::array<Standard_Integer, 3> {
            std::array<Standard_Integer, 3> result{};
            self.Get(result[0], result[1], result[2]);
            return result;
          })
      .Bind_Method(Poly_Triangle, Value)
      .End_Class()

      .Begin_Derive(Poly_Triangulation, Standard_Transient)
      .addConstructorFrom<
          Handle(Poly_Triangulation), void(),
          void(const Standard_Integer, const Standard_Integer,
               const Standard_Boolean, const Standard_Boolean),
          void(const TColgp_Array1OfPnt &, const Poly_Array1OfTriangle &),
          void(const TColgp_Array1OfPnt &, const TColgp_Array1OfPnt2d &,
               const Poly_Array1OfTriangle &)>()
      .Bind_Method(Poly_Triangulation, Copy)
      .addProperty("Deflection_",
                   luabridge::overload<>(&Poly_Triangulation::Deflection),
                   luabridge::overload<const Standard_Real>(
                       &Poly_Triangulation::Deflection))
      .addProperty(
          "Parameters_", luabridge::overload<>(&Poly_Triangulation::Parameters),
          luabridge::overload<const Handle(Poly_TriangulationParameters) &>(
              &Poly_Triangulation::Parameters))
      .Bind_Method(Poly_Triangulation, Clear)
      .Bind_Method(Poly_Triangulation, HasGeometry)
      .Bind_Method(Poly_Triangulation, NbNodes)
      .Bind_Method(Poly_Triangulation, NbTriangles)
      .Bind_Method(Poly_Triangulation, HasUVNodes)
      .Bind_Method(Poly_Triangulation, HasNormals)
      .Bind_Method(Poly_Triangulation, Node)
      .Bind_Method(Poly_Triangulation, SetNode)
      .Bind_Method(Poly_Triangulation, UVNode)
      .Bind_Method(Poly_Triangulation, SetUVNode)
      .Bind_Method(Poly_Triangulation, Triangle)
      .Bind_Method(Poly_Triangulation, SetTriangle)
      .addFunction("Normal", luabridge::overload<Standard_Integer>(
                                 &Poly_Triangulation::Normal))
      .addFunction("SetNormal",
                   luabridge::overload<const Standard_Integer, const gp_Dir &>(
                       &Poly_Triangulation::SetNormal))
      .Bind_Property(Poly_Triangulation, MeshPurpose, SetMeshPurpose)
      .Bind_Property(Poly_Triangulation, CachedMinMax, SetCachedMinMax)
      .Bind_Method(Poly_Triangulation, HasCachedMinMax)
      .Bind_Method(Poly_Triangulation, UpdateCachedMinMax)
      .Bind_Method(Poly_Triangulation, MinMax)
      .Bind_Method(Poly_Triangulation, IsDoublePrecision)
      .Bind_Method(Poly_Triangulation, SetDoublePrecision)
      .Bind_Method(Poly_Triangulation, ResizeNodes)
      .Bind_Method(Poly_Triangulation, ResizeTriangles)
      .Bind_Method(Poly_Triangulation, AddUVNodes)
      .Bind_Method(Poly_Triangulation, RemoveUVNodes)
      .Bind_Method(Poly_Triangulation, AddNormals)
      .Bind_Method(Poly_Triangulation, RemoveNormals)
      .Bind_Method(Poly_Triangulation, ComputeNormals)
      .Bind_Method(Poly_Triangulation, MapNodeArray)
      .Bind_Method(Poly_Triangulation, MapTriangleArray)
      .Bind_Method(Poly_Triangulation, MapUVNodeArray)
      .Bind_Method(Poly_Triangulation, MapNormalArray)
      .Bind_Method(Poly_Triangulation, InternalTriangles)
      .Bind_Method(Poly_Triangulation, InternalNodes)
      .Bind_Method(Poly_Triangulation, InternalUVNodes)
      .Bind_Method(Poly_Triangulation, InternalNormals)
      .End_Derive()

      .End_Namespace1();
}
