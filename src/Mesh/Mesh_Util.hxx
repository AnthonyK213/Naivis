#ifndef _Naivis_Mesh_Util_HeaderFile
#define _Naivis_Mesh_Util_HeaderFile

#include <IMeshTools_Parameters.hxx>
#include <MeshVS_Mesh.hxx>
#include <TopoDS_Shape.hxx>

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
