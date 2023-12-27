#ifndef _Naivis_Mesh_Util_HeaderFile
#define _Naivis_Mesh_Util_HeaderFile

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

#include "Mesh_MeshBuilder.hxx"
#include "XSDRAWSTLVRML_DataSource_1.hxx"

#include <naivecgl/Common/geometry.h>

namespace Mesh_Util {

/// @brief
Handle(Poly_Triangulation)
    ShapeToMesh(const TopoDS_Shape &theShape,
                const IMeshTools_Parameters &theMeshParams,
                const TopLoc_Location &theLocation);

/// @brief
/// @param thePoly3D
/// @return
Handle(Poly_Triangulation) NaivePoly3DToMesh(const Naive_Poly &thePoly);

/// @brief
/// @param
/// @return
Handle(MeshVS_Mesh) CreateMeshVS(const Handle(Poly_Triangulation) & theMesh);

} // namespace Mesh_Util

#endif
