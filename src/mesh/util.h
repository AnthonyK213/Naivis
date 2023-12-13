#ifndef _Naivis_Mesh_Util_Header
#define _Naivis_Mesh_Util_Header

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

#include "XSDRAWSTLVRML_DataSource_1.hxx"

#include <naivecgl/Common/geometry.h>

namespace naivis {
namespace mesh {

class MeshBuilder {
public:
  MeshBuilder();

  ~MeshBuilder();

public:
  bool add(Handle(Poly_Triangulation) theMesh,
           const TopLoc_Location &theLocation);

  void clear();

  Handle(Poly_Triangulation) mesh() const;

  void transform(const gp_Trsf &theT);

private:
  std::vector<gp_Pnt> myNodes{};

  std::vector<Poly_Triangle> myTriangles{};
};

/// @brief
Handle(Poly_Triangulation)
    shapeToMesh(const TopoDS_Shape &theShape,
                const IMeshTools_Parameters &theMeshParams,
                const TopLoc_Location &theLocation);

/// @brief
/// @param thePoly3D
/// @return
Handle(Poly_Triangulation) naivePoly3DToMesh(const Naive_Poly &thePoly);

/// @brief
/// @param
/// @return
Handle(MeshVS_Mesh) createMeshVS(const Handle(Poly_Triangulation) & theMesh);

} // namespace mesh
} // namespace naivis

#endif
