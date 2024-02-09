#ifndef _Naivis_Util_Mesh_HeaderFile
#define _Naivis_Util_Mesh_HeaderFile

#include <IMeshTools_Parameters.hxx>
#include <MeshVS_Mesh.hxx>
#include <TopoDS_Shape.hxx>

#include <Mesh/XSDRAWSTLVRML_DataSource_1.hxx>

#include <naivecgl/Common/geometry.h>

namespace Util_Mesh {

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
/// @param thePoly3D
/// @return
Naive_H_Poly MeshToNaivePoly3D(const Handle(Poly_Triangulation) & theMesh);

/// @brief
/// @param
/// @return
Handle(MeshVS_Mesh) CreateMeshVS(const Handle(Poly_Triangulation) & theMesh);

} // namespace Util_Mesh

#endif
