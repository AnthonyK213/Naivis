#include "NaiveDoc_CmdRemoveObjects.hxx"
#include "NaiveDoc_Document.hxx"

NaiveDoc_CmdRemoveObjects::NaiveDoc_CmdRemoveObjects(
    NaiveDoc_Document *theDoc, const NaiveDoc_ObjectList &theRemoveList,
    Standard_Boolean theToUpdate, QUndoCommand *theParent)
    : QUndoCommand(theParent), myDoc(theDoc), myRemoveList(theRemoveList),
      myToUpdate(theToUpdate) {}

NaiveDoc_CmdRemoveObjects::~NaiveDoc_CmdRemoveObjects() {}

void NaiveDoc_CmdRemoveObjects::undo() {
  for (const auto &anObj : myRemoveList) {
    myDoc->Objects()->addObjectRaw(anObj);
  }

  if (myToUpdate)
    myDoc->UpdateView();
}

void NaiveDoc_CmdRemoveObjects::redo() {
  for (const auto &anObj : myRemoveList) {
    myDoc->Objects()->removeObjectRaw(anObj);
  }

  if (myToUpdate)
    myDoc->UpdateView();
}
