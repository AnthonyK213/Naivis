#ifndef _Naivis_Util_Mesh_HeaderFile
#define _Naivis_Util_Mesh_HeaderFile

#include <IMeshTools_Parameters.hxx>
#include <MeshVS_Mesh.hxx>
#include <TopoDS_Shape.hxx>

#include <Mesh/XSDRAWSTLVRML_DataSource_1.hxx>

namespace Util_Mesh {

Handle(MeshVS_Mesh) CreateMeshVS(const Handle(Poly_Triangulation) & theMesh);

} /* namespace Util_Mesh */

#endif
