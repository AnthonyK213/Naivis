#include "NaiveDoc_CmdRemoveObjects.hxx"
#include "NaiveDoc_Document.hxx"
#include "V3d_View.hxx"
#include "V3d_Viewer.hxx"

NaiveDoc_CmdRemoveObjects::NaiveDoc_CmdRemoveObjects(
    NaiveDoc_Document *theDoc, const NaiveDoc_ObjectList &theRemoveList,
    Standard_Boolean theToUpdate, QUndoCommand *theParent)
    : QUndoCommand(theParent), myDoc(theDoc), myRemoveList(theRemoveList),
      myToUpdate(theToUpdate) {
  myContext = myDoc->Context();
  myObjects = myDoc->Objects();
}

NaiveDoc_CmdRemoveObjects::~NaiveDoc_CmdRemoveObjects() {}

void NaiveDoc_CmdRemoveObjects::undo() {
  for (const auto &anObj : myRemoveList) {
    myObjects->AddObject(anObj);
    myContext->Display(anObj, Standard_False);
  }

  if (myToUpdate)
    myContext->CurrentViewer()->Redraw();
}

void NaiveDoc_CmdRemoveObjects::redo() {
  for (const auto &anObj : myRemoveList) {
    myObjects->RemoveObject(anObj);
    myContext->Remove(anObj, Standard_False);
  }

  if (myToUpdate)
    myContext->CurrentViewer()->Redraw();
}
