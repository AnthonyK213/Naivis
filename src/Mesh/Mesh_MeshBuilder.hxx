#ifndef _Naivis_Mesh_MeshBuilder_HeaderFile
#define _Naivis_Mesh_MeshBuilder_HeaderFile

#include <BRepMesh_IncrementalMesh.hxx>
#include <BRep_TFace.hxx>
#include <BRep_Tool.hxx>
#include <IMeshTools_Parameters.hxx>
#include <MeshVS_Mesh.hxx>
#include <MeshVS_MeshPrsBuilder.hxx>
#include <Poly_Triangle.hxx>
#include <Poly_Triangulation.hxx>
#include <TColStd_HPackedMapOfInteger.hxx>
#include <TopExp_Explorer.hxx>
#include <TopLoc_Location.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <gp_Pnt.hxx>
#include <gp_Trsf.hxx>

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
