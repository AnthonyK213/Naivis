#include "NaiveDoc_ObjectTable.hxx"
#include "NaiveDoc_CmdAddObjects.hxx"

#include <Mesh/Mesh_Util.hxx>

IMPLEMENT_STANDARD_RTTIEXT(NaiveDoc_ObjectTable, Standard_Transient)

NaiveDoc_ObjectTable::NaiveDoc_ObjectTable(NaiveDoc_Document *theDoc)
    : myDoc(theDoc), myContext(nullptr) {
  myUndoStack = new QUndoStack();
  myUndoStack->setUndoLimit(100);
}

NaiveDoc_ObjectTable::~NaiveDoc_ObjectTable() { delete myUndoStack; }

Handle(NaiveDoc_Object) NaiveDoc_ObjectTable::AddShape(
    const TopoDS_Shape &theShape, Standard_Boolean theToUpdate) {
  Handle(NaiveDoc_Object) anObj = new AIS_Shape(theShape);
  NaiveDoc_Object_SetId(*anObj);
  anObj->SetDisplayMode(AIS_Shaded);
  addObject(anObj, theToUpdate);

  return anObj;
}

Handle(NaiveDoc_Object) NaiveDoc_ObjectTable::AddMesh(
    const Handle(Poly_Triangulation) & theMesh, Standard_Boolean theToUpdate) {
  Handle(NaiveDoc_Object) anObj = Mesh_Util::CreateMeshVS(theMesh);
  NaiveDoc_Object_SetId(*anObj);
  anObj->SetDisplayMode(MeshVS_DMF_Shading);
  addObject(anObj, theToUpdate);

  return anObj;
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

void NaiveDoc_ObjectTable::addObjectRaw(const Handle(NaiveDoc_Object) &
                                            theObject,
                                        Standard_Boolean theToUpdate) {
  myObjects.insert(NaiveDoc_Object_GetId(*theObject), theObject);
  myContext->Display(theObject, theToUpdate);
}

void NaiveDoc_ObjectTable::addObject(const Handle(NaiveDoc_Object) & theObject,
                                     Standard_Boolean theToUpdate) {
  NaiveDoc_ObjectList anObjectList{theObject};
  addObjects(anObjectList, theToUpdate);
}

void NaiveDoc_ObjectTable::addObjects(const NaiveDoc_ObjectList &theObjects,
                                      Standard_Boolean theToUpdate) {
  NaiveDoc_CmdAddObjects *cmd =
      new NaiveDoc_CmdAddObjects(myDoc, theObjects, theToUpdate);
  myUndoStack->push(cmd);
}

void NaiveDoc_ObjectTable::removeObjectRaw(const Handle(NaiveDoc_Object) &
                                               theObject,
                                           Standard_Boolean theToUpdate) {
  myObjects.remove(NaiveDoc_Object_GetId(*theObject));
  myContext->Remove(theObject, theToUpdate);
}
