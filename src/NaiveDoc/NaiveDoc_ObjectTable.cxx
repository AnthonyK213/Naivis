#include "NaiveDoc_ObjectTable.hxx"
#include "NaiveDoc_CmdAddObjects.hxx"
#include "NaiveDoc_CmdDeleteObjects.hxx"

#include <Mesh/Mesh_Util.hxx>

IMPLEMENT_STANDARD_RTTIEXT(NaiveDoc_ObjectTable, Standard_Transient)

NaiveDoc_ObjectTable::NaiveDoc_ObjectTable(NaiveDoc_Document *theDoc)
    : myDoc(theDoc), myContext(nullptr) {
  myUndoStack = new QUndoStack();
  myUndoStack->setUndoLimit(100);
}

NaiveDoc_ObjectTable::~NaiveDoc_ObjectTable() { delete myUndoStack; }

NaiveDoc_Id NaiveDoc_ObjectTable::AddShape(const TopoDS_Shape &theShape,
                                           Standard_Boolean theToUpdate) {
  Handle(NaiveDoc_Object) anObj = new AIS_Shape(theShape);
  NaiveDoc_Object_SetId(*anObj);
  anObj->SetDisplayMode(AIS_Shaded);

  return addObject(anObj, theToUpdate);
}

NaiveDoc_Id NaiveDoc_ObjectTable::AddMesh(const Handle(Poly_Triangulation) &
                                              theMesh,
                                          Standard_Boolean theToUpdate) {
  Handle(NaiveDoc_Object) anObj = Mesh_Util::CreateMeshVS(theMesh);
  NaiveDoc_Object_SetId(*anObj);
  anObj->SetDisplayMode(MeshVS_DMF_Shading);

  return addObject(anObj, theToUpdate);
}

void NaiveDoc_ObjectTable::Clear(Standard_Boolean theToUpdate) {
  NaiveDoc_ObjectList theList = myObjects.values();
  DeleteObjects(theList, theToUpdate);
}

Handle(NaiveDoc_Object)
    NaiveDoc_ObjectTable::FindObject(const NaiveDoc_Id &theId) const {
  if (myObjects.contains(theId)) {
    return myObjects.value(theId);
  }

  return nullptr;
}

Standard_Boolean
NaiveDoc_ObjectTable::DeleteObject(const Handle(NaiveDoc_Object) & theObject,
                                   Standard_Boolean theToUpdate) {
  return DeleteObjects({theObject}, theToUpdate) == 1;
}

Standard_Boolean
NaiveDoc_ObjectTable::DeleteObject(const NaiveDoc_Id &theId,
                                   Standard_Boolean theToUpdate) {
  Handle(NaiveDoc_Object) anObj = FindObject(theId);
  return DeleteObject(anObj, theToUpdate);
}

Standard_Integer
NaiveDoc_ObjectTable::DeleteObjects(const NaiveDoc_ObjectList &theObjects,
                                    Standard_Boolean theToUpdate) {
  NaiveDoc_CmdDeleteObjects *cmd = new NaiveDoc_CmdDeleteObjects(
      myDoc, NaiveDoc_CmdDeleteObjects::Delete, theObjects, theToUpdate);
  myUndoStack->push(cmd);

  return cmd->Size();
}

Standard_Integer
NaiveDoc_ObjectTable::DeleteObjects(NaiveDoc_ObjectList &&theObjects,
                                    Standard_Boolean theToUpdate) {
  NaiveDoc_CmdDeleteObjects *cmd =
      new NaiveDoc_CmdDeleteObjects(myDoc, NaiveDoc_CmdDeleteObjects::Delete,
                                    std::move(theObjects), theToUpdate);
  myUndoStack->push(cmd);

  return cmd->Size();
}

Standard_Boolean
NaiveDoc_ObjectTable::HideObject(const Handle(NaiveDoc_Object) & theObject,
                                 Standard_Boolean theToUpdate) {
  NaiveDoc_ObjectList anObjectList{theObject};
  return HideObjects(anObjectList, theToUpdate) == 1;
}

Standard_Boolean
NaiveDoc_ObjectTable::HideObject(const NaiveDoc_Id &theId,
                                 Standard_Boolean theToUpdate) {
  Handle(NaiveDoc_Object) anObj = FindObject(theId);
  return HideObject(anObj, theToUpdate);
}

Standard_Integer
NaiveDoc_ObjectTable::HideObjects(const NaiveDoc_ObjectList &theObjects,
                                  Standard_Boolean theToUpdate) {
  NaiveDoc_CmdDeleteObjects *cmd = new NaiveDoc_CmdDeleteObjects(
      myDoc, NaiveDoc_CmdDeleteObjects::Hide, theObjects, theToUpdate);
  myUndoStack->push(cmd);

  return cmd->Size();
}

Standard_Boolean
NaiveDoc_ObjectTable::PurgeObject(const Handle(NaiveDoc_Object) & theObject,
                                  Standard_Boolean theToUpdate) {
  NaiveDoc_ObjectList anObjectList{theObject};
  return PurgeObjects(anObjectList, theToUpdate) == 1;
}

Standard_Boolean
NaiveDoc_ObjectTable::PurgeObject(const NaiveDoc_Id &theId,
                                  Standard_Boolean theToUpdate) {
  Handle(NaiveDoc_Object) anObj = FindObject(theId);
  return PurgeObject(anObj, theToUpdate);
}

Standard_Integer
NaiveDoc_ObjectTable::PurgeObjects(const NaiveDoc_ObjectList &theObjects,
                                   Standard_Boolean theToUpdate) {
  NaiveDoc_CmdDeleteObjects *cmd = new NaiveDoc_CmdDeleteObjects(
      myDoc, NaiveDoc_CmdDeleteObjects::Purge, theObjects, theToUpdate);
  myUndoStack->push(cmd);

  return cmd->Size();
}

NaiveDoc_ObjectList NaiveDoc_ObjectTable::SelectedObjects() const {
  NaiveDoc_ObjectList aResult{};
  aResult.reserve(myContext->NbSelected());

  for (myContext->InitSelected(); myContext->MoreSelected();
       myContext->NextSelected()) {
    aResult.push_back(myContext->SelectedInteractive());
  }

  return aResult;
}

NaiveDoc_Id NaiveDoc_ObjectTable::addObject(const Handle(NaiveDoc_Object) &
                                                theObject,
                                            Standard_Boolean theToUpdate) {
  NaiveDoc_IdList anIds = addObjects({theObject}, theToUpdate);

  if (anIds.isEmpty())
    return NaiveDoc_Id();

  return anIds.first();
}

NaiveDoc_IdList
NaiveDoc_ObjectTable::addObjects(const NaiveDoc_ObjectList &theObjects,
                                 Standard_Boolean theToUpdate) {
  NaiveDoc_CmdAddObjects *cmd =
      new NaiveDoc_CmdAddObjects(myDoc, theObjects, theToUpdate);
  myUndoStack->push(cmd);

  return cmd->Id();
}

NaiveDoc_IdList
NaiveDoc_ObjectTable::addObjects(NaiveDoc_ObjectList &&theObjects,
                                 Standard_Boolean theToUpdate) {
  NaiveDoc_CmdAddObjects *cmd =
      new NaiveDoc_CmdAddObjects(myDoc, std::move(theObjects), theToUpdate);
  myUndoStack->push(cmd);

  return cmd->Id();
}

Standard_Boolean
NaiveDoc_ObjectTable::addObjectRaw(const Handle(NaiveDoc_Object) & theObject,
                                   Standard_Boolean theToUpdate) {
  if (theObject.IsNull())
    return Standard_False;

  NaiveDoc_Id anId = NaiveDoc_Object_GetId(*theObject);

  if (myObjects.contains(anId)) {
    if (!NaiveDoc_Object_IsDeleted(*theObject)) {
      return Standard_False;
    }

    NaiveDoc_Object_SetDeleted(*theObject, Standard_False);
  } else {
    myObjects.insert(anId, theObject);
  }

  myContext->Display(theObject, theToUpdate);

  return Standard_True;
}

Standard_Boolean
NaiveDoc_ObjectTable::deleteObjectRaw(const Handle(NaiveDoc_Object) & theObject,
                                      Standard_Boolean theToUpdate) {
  if (theObject.IsNull())
    return Standard_False;

  if (NaiveDoc_Object_IsDeleted(*theObject))
    return Standard_False;

  NaiveDoc_Id anId = NaiveDoc_Object_GetId(*theObject);

  if (!myObjects.contains(anId))
    return Standard_False;

  NaiveDoc_Object_SetDeleted(*theObject, Standard_True);
  myContext->Remove(theObject, theToUpdate);

  return Standard_True;
}

Standard_Boolean
NaiveDoc_ObjectTable::hideObjectRaw(const Handle(NaiveDoc_Object) & theObject,
                                    Standard_Boolean theToUpdate) {
  if (theObject.IsNull())
    return Standard_False;

  if (NaiveDoc_Object_IsHidden(*theObject))
    return Standard_False;

  NaiveDoc_Id anId = NaiveDoc_Object_GetId(*theObject);

  if (!myObjects.contains(anId))
    return Standard_False;

  NaiveDoc_Object_SetHidden(*theObject, Standard_True);
  myContext->Erase(theObject, theToUpdate);

  return Standard_True;
}

Standard_Boolean
NaiveDoc_ObjectTable::purgeObjectRaw(const Handle(NaiveDoc_Object) & theObject,
                                     Standard_Boolean theToUpdate) {
  if (theObject.IsNull())
    return Standard_False;

  NaiveDoc_Id anId = NaiveDoc_Object_GetId(*theObject);

  if (!myObjects.contains(anId))
    return Standard_False;

  myObjects.remove(NaiveDoc_Object_GetId(*theObject));
  myContext->Remove(theObject, theToUpdate);

  return Standard_True;
}
