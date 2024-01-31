#ifndef _Naivis_NaiveDoc_ObjectTable_HeaderFile
#define _Naivis_NaiveDoc_ObjectTable_HeaderFile

#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <Poly_Triangulation.hxx>
#include <Standard.hxx>
#include <Standard_Transient.hxx>
#include <Standard_Type.hxx>
#include <TopoDS_Shape.hxx>

#include <QHash>
#include <QUndoStack>

#include "NaiveDoc_Object.hxx"

class NaiveDoc_Document;
class NaiveDoc_CmdAddObjects;
class NaiveDoc_CmdDeleteObjects;
class NaiveDoc_CmdShowObjects;
class NaiveDoc_CmdHideObjects;

class NaiveDoc_ObjectTable : public Standard_Transient {
  friend class NaiveDoc_Document;
  friend class NaiveDoc_CmdAddObjects;
  friend class NaiveDoc_CmdDeleteObjects;
  friend class NaiveDoc_CmdShowObjects;
  friend class NaiveDoc_CmdHideObjects;

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

  Handle(NaiveDoc_Object) FindId(const NaiveDoc_Id &theId) const;

  Standard_Boolean DeleteObject(const Handle(NaiveDoc_Object) & theObject,
                                Standard_Boolean theToUpdate);

  Standard_Boolean DeleteObject(const NaiveDoc_Id &theId,
                                Standard_Boolean theToUpdate);

  Standard_Integer DeleteObjects(const NaiveDoc_ObjectList &theObjects,
                                 Standard_Boolean theToUpdate);

  Standard_Integer DeleteObjects(NaiveDoc_ObjectList &&theObjects,
                                 Standard_Boolean theToUpdate);

  Standard_Boolean ShowObject(const Handle(NaiveDoc_Object) & theObject,
                              Standard_Boolean theToUpdate);

  Standard_Boolean ShowObject(const NaiveDoc_Id &theId,
                              Standard_Boolean theToUpdate);

  Standard_Integer ShowObjects(const NaiveDoc_ObjectList &theObjects,
                               Standard_Boolean theToUpdate);

  Standard_Integer ShowObjects(NaiveDoc_ObjectList &&theObjects,
                               Standard_Boolean theToUpdate);

  Standard_Integer ShowAll(Standard_Boolean theToUpdate);

  Standard_Boolean HideObject(const Handle(NaiveDoc_Object) & theObject,
                              Standard_Boolean theToUpdate);

  Standard_Boolean HideObject(const NaiveDoc_Id &theId,
                              Standard_Boolean theToUpdate);

  Standard_Integer HideObjects(const NaiveDoc_ObjectList &theObjects,
                               Standard_Boolean theToUpdate);

  Standard_Integer HideObjects(NaiveDoc_ObjectList &&theObjects,
                               Standard_Boolean theToUpdate);

  Standard_Boolean PurgeObject(const Handle(NaiveDoc_Object) & theObject,
                               Standard_Boolean theToUpdate);

  Standard_Boolean PurgeObject(const NaiveDoc_Id &theId,
                               Standard_Boolean theToUpdate);

  Standard_Integer PurgeObjects(const NaiveDoc_ObjectList &theObjects,
                                Standard_Boolean theToUpdate);

  Standard_Integer PurgeObjects(NaiveDoc_ObjectList &&theObjects,
                                Standard_Boolean theToUpdate);

  Standard_Boolean SelectObject(const Handle(NaiveDoc_Object) & theObject,
                                Standard_Boolean theSelect,
                                Standard_Boolean theToUpdate);

  Standard_Boolean SelectObject(const NaiveDoc_Id &theId,
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
  NaiveDoc_Id addObject(const Handle(NaiveDoc_Object) & theObject,
                        Standard_Boolean theToUpdate);

  NaiveDoc_IdList addObjects(const NaiveDoc_ObjectList &theObjects,
                             Standard_Boolean theToUpdate);

  NaiveDoc_IdList addObjects(NaiveDoc_ObjectList &&theObjects,
                             Standard_Boolean theToUpdate);

private:
  Standard_Boolean addObjectRaw(const Handle(NaiveDoc_Object) & theObject,
                                Standard_Boolean theToUpdate = Standard_False);

  Standard_Boolean
  deleteObjectRaw(const Handle(NaiveDoc_Object) & theObject,
                  Standard_Boolean theToUpdate = Standard_False);

  Standard_Boolean showObjectRaw(const Handle(NaiveDoc_Object) & theObject,
                                 Standard_Boolean theToUpdate = Standard_False);

  Standard_Boolean hideObjectRaw(const Handle(NaiveDoc_Object) & theObject,
                                 Standard_Boolean theToUpdate = Standard_False);

  Standard_Boolean
  purgeObjectRaw(const Handle(NaiveDoc_Object) & theObject,
                 Standard_Boolean theToUpdate = Standard_False);

  void purgeAllRaw(Standard_Boolean theToUpdate = Standard_False);

private:
  QHash<NaiveDoc_Id, Handle(NaiveDoc_Object)> myObjects;
  NaiveDoc_Document *myDoc;
  QUndoStack *myUndoStack;
};

DEFINE_STANDARD_HANDLE(NaiveDoc_ObjectTable, Standard_Transient)

#endif
