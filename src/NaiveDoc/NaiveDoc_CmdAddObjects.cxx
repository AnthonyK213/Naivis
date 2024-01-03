#include "NaiveDoc_CmdAddObjects.hxx"
#include "NaiveDoc_Document.hxx"
#include "V3d_View.hxx"
#include "V3d_Viewer.hxx"

NaiveDoc_CmdAddObjects::NaiveDoc_CmdAddObjects(
    NaiveDoc_Document *theDoc, const NaiveDoc_ObjectList &theAddList,
    Standard_Boolean theToUpdate, QUndoCommand *theParent)
    : QUndoCommand(theParent), myDoc(theDoc), myAddList(theAddList),
      myToUpdate(theToUpdate) {}

NaiveDoc_CmdAddObjects::~NaiveDoc_CmdAddObjects() {}

void NaiveDoc_CmdAddObjects::undo() {
  for (const auto &anObj : myAddList) {
    myDoc->Objects()->removeObjectRaw(anObj);
  }

  if (myToUpdate)
    myDoc->UpdateView();
}

void NaiveDoc_CmdAddObjects::redo() {
  for (const auto &anObj : myAddList) {
    myDoc->Objects()->addObjectRaw(anObj);
  }

  if (myToUpdate)
    myDoc->UpdateView();
}
