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
#include <QUuid>

#include "NaiveDoc_Object.hxx"

class NaiveDoc_Document;
class NaiveDoc_CmdAddObjects;
class NaiveDoc_CmdRemoveObjects;

class NaiveDoc_ObjectTable;
DEFINE_STANDARD_HANDLE(NaiveDoc_ObjectTable, Standard_Transient)

class NaiveDoc_ObjectTable : public Standard_Transient {
  friend class NaiveDoc_Document;
  friend class NaiveDoc_CmdAddObjects;
  friend class NaiveDoc_CmdRemoveObjects;

public:
  explicit NaiveDoc_ObjectTable(NaiveDoc_Document *theDoc = nullptr);

  ~NaiveDoc_ObjectTable();

public:
  Handle(NaiveDoc_Object)
      AddShape(const TopoDS_Shape &theShape, Standard_Boolean theToUpdate);

  Handle(NaiveDoc_Object) AddMesh(const Handle(Poly_Triangulation) & theMesh,
                                  Standard_Boolean theToUpdate);

  NaiveDoc_ObjectList SelectedObjects() const;

  DEFINE_STANDARD_RTTIEXT(NaiveDoc_ObjectTable, Standard_Transient)

private:
  void addObjectRaw(const Handle(NaiveDoc_Object) & theObject,
                    Standard_Boolean theToUpdate = Standard_False);

  void addObject(const Handle(NaiveDoc_Object) & theObject,
                 Standard_Boolean theToUpdate = Standard_False);

  void addObjects(const NaiveDoc_ObjectList &theObjects,
                  Standard_Boolean theToUpdate = Standard_False);

  void removeObjectRaw(const Handle(NaiveDoc_Object) & theObject,
                       Standard_Boolean theToUpdate = Standard_False);

private:
  QHash<QUuid, Handle(NaiveDoc_Object)> myObjects;
  Handle(AIS_InteractiveContext) myContext;
  NaiveDoc_Document *myDoc;
  QUndoStack *myUndoStack;
};

#endif
