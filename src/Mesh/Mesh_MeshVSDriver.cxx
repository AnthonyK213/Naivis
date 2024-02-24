#include "Mesh_MeshVSDriver.hxx"

#include <AIS_InteractiveObject.hxx>
#include <MeshVS_Mesh.hxx>
#include <Poly_Triangulation.hxx>
#include <Standard_GUID.hxx>
#include <TDF_Label.hxx>
#include <TDataXtd_Triangulation.hxx>

#include <Mesh/XSDRAWSTLVRML_DataSource_1.hxx>
#include <Util/Util_Mesh.hxx>

IMPLEMENT_STANDARD_RTTIEXT(Mesh_MeshVSDriver, TPrsStd_Driver)

Standard_Boolean Mesh_MeshVSDriver::Update(const TDF_Label &L,
                                               Handle(AIS_InteractiveObject) &
                                                   ais) {
  Handle(TDataXtd_Triangulation) aTri;
  if (!L.FindAttribute(TDataXtd_Triangulation::GetID(), aTri))
    return Standard_False;
  
  ais = Util_Mesh::CreateMeshVS(aTri->Get());

  return Standard_True;
}

const Standard_GUID &Mesh_MeshVSDriver::GetID() {
  static Standard_GUID ID("18c481f6-0e13-46bb-8565-eb188b25c570");
  return ID;
}
