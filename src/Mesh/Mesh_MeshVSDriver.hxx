#ifndef _Naivis_Mesh_MeshVSDriver_HeaderFile
#define _Naivis_Mesh_MeshVSDriver_HeaderFile

#include <Standard.hxx>
#include <Standard_Type.hxx>
#include <TPrsStd_Driver.hxx>

class TDF_Label;
class AIS_InteractiveObject;
class Standard_GUID;

class Mesh_MeshVSDriver;
DEFINE_STANDARD_HANDLE(Mesh_MeshVSDriver, TPrsStd_Driver)

class Mesh_MeshVSDriver : public TPrsStd_Driver {
public:
  virtual Standard_Boolean Update(const TDF_Label &L,
                                  Handle(AIS_InteractiveObject) &
                                      ais) Standard_OVERRIDE;

  static const Standard_GUID &GetID();

  DEFINE_STANDARD_RTTIEXT(Mesh_MeshVSDriver, TPrsStd_Driver)
};

#endif
