#ifndef _Naivis_NaiveDoc_ObjectTable_HeaderFile
#define _Naivis_NaiveDoc_ObjectTable_HeaderFile

#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <Poly_Triangulation.hxx>
#include <Standard.hxx>
#include <Standard_Transient.hxx>
#include <Standard_Type.hxx>
#include <TopoDS_Shape.hxx>

#include "NaiveDoc_Object.hxx"

class NaiveDoc_Document;

class NaiveDoc_ObjectTable : public Standard_Transient {
  friend class NaiveDoc_Document;

public:
  explicit NaiveDoc_ObjectTable(NaiveDoc_Document *theDoc = nullptr);

  ~NaiveDoc_ObjectTable();

public:
  const Handle(AIS_InteractiveContext) & Context() const;

  NaiveDoc_Id AddShape(const TopoDS_Shape &theShape,
                       Standard_Boolean theToUpdate);

  NaiveDoc_Id AddMesh(const Handle(Poly_Triangulation) & theMesh,
                      Standard_Boolean theToUpdate);

  void Clear(Standard_Boolean theToUpdate);

  Handle(NaiveDoc_Object) Find(const NaiveDoc_Id &theId) const;

  Standard_Boolean DeleteObject(const Handle(NaiveDoc_Object) & theObject,
                                Standard_Boolean theToUpdate);

  Standard_Integer DeleteObjects(const NaiveDoc_ObjectList &theObjects,
                                 Standard_Boolean theToUpdate);

  Standard_Boolean ShowObject(const Handle(NaiveDoc_Object) & theObject,
                              Standard_Boolean theToUpdate);

  Standard_Integer ShowObjects(const NaiveDoc_ObjectList &theObjects,
                               Standard_Boolean theToUpdate);

  Standard_Integer ShowAll(Standard_Boolean theToUpdate);

  Standard_Boolean HideObject(const NaiveDoc_Id &theId,
                              Standard_Boolean theToUpdate);

  Standard_Boolean HideObject(const Handle(NaiveDoc_Object) & theObject,
                              Standard_Boolean theToUpdate);

  Standard_Integer HideObjects(const NaiveDoc_IdList &theIds,
                               Standard_Boolean theToUpdate);

  Standard_Integer HideObjects(const NaiveDoc_ObjectList &theObjects,
                               Standard_Boolean theToUpdate);

  Standard_Boolean PurgeObject(const Handle(NaiveDoc_Object) & theObject,
                               Standard_Boolean theToUpdate);

  Standard_Integer PurgeObjects(const NaiveDoc_ObjectList &theObjects,
                                Standard_Boolean theToUpdate);

  Standard_Boolean SelectObject(const NaiveDoc_Id &theId,
                                Standard_Boolean theSelect,
                                Standard_Boolean theToUpdate);

  Standard_Boolean SelectObject(const Handle(NaiveDoc_Object) & theObject,
                                Standard_Boolean theSelect,
                                Standard_Boolean theToUpdate);

  Standard_Integer SelectObjects(const NaiveDoc_IdList &theIds,
                                 Standard_Boolean theSelect,
                                 Standard_Boolean theToUpdate);

  Standard_Integer SelectObjects(const NaiveDoc_ObjectList &theObjects,
                                 Standard_Boolean theSelect,
                                 Standard_Boolean theToUpdate);

  Standard_Integer SelectAll(Standard_Boolean theToUpdate);

  Standard_Integer UnselectAll(Standard_Boolean theToUpdate);

  NaiveDoc_ObjectList SelectedObjects() const;

  DEFINE_STANDARD_RTTIEXT(NaiveDoc_ObjectTable, Standard_Transient)

private:
  NaiveDoc_Document *myDoc;
};

DEFINE_STANDARD_HANDLE(NaiveDoc_ObjectTable, Standard_Transient)

#endif
