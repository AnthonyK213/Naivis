#ifndef _Naivis_Mesh_MeshBuilder_HeaderFile
#define _Naivis_Mesh_MeshBuilder_HeaderFile

#include <Poly_Triangle.hxx>
#include <Poly_Triangulation.hxx>
#include <TopLoc_Location.hxx>
#include <gp_Pnt.hxx>

#include <vector>

class Mesh_MeshBuilder {
public:
  Mesh_MeshBuilder();

  ~Mesh_MeshBuilder();

public:
  Standard_Boolean Add(Handle(Poly_Triangulation) theMesh,
                       const TopLoc_Location &theLocation);

  void Clear();

  Handle(Poly_Triangulation) Mesh() const;

  void Transform(const gp_Trsf &theT);

private:
  std::vector<gp_Pnt> myNodes{};
  std::vector<Poly_Triangle> myTriangles{};
};

#endif
