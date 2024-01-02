#include "NaiveDoc_CmdAddObjects.hxx"
#include "NaiveDoc_Document.hxx"
#include "V3d_View.hxx"
#include "V3d_Viewer.hxx"

NaiveDoc_CmdAddObjects::NaiveDoc_CmdAddObjects(
    NaiveDoc_Document *theDoc, const NaiveDoc_ObjectList &theAddList,
    Standard_Boolean theToUpdate, QUndoCommand *theParent)
    : QUndoCommand(theParent), myDoc(theDoc), myAddList(theAddList),
      myToUpdate(theToUpdate) {
  myContext = myDoc->Context();
  myObjects = myDoc->Objects();
}

NaiveDoc_CmdAddObjects::~NaiveDoc_CmdAddObjects() {}

void NaiveDoc_CmdAddObjects::undo() {
  for (const auto &anObj : myAddList) {
    myObjects->RemoveObject(anObj);
    myContext->Remove(anObj, Standard_False);
  }

  if (myToUpdate)
    myContext->CurrentViewer()->Redraw();
}

void NaiveDoc_CmdAddObjects::redo() {
  for (const auto &anObj : myAddList) {
    myObjects->AddObject(anObj);
    myContext->Display(anObj, Standard_False);
  }

  if (myToUpdate)
    myContext->CurrentViewer()->Redraw();
}
