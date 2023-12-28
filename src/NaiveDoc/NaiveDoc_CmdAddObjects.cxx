#include "NaiveDoc_CmdAddObjects.hxx"
#include "NaiveDoc_Document.hxx"
#include "V3d_View.hxx"
#include "V3d_Viewer.hxx"

NaiveDoc_CmdAddObjects::NaiveDoc_CmdAddObjects(
    NaiveDoc_Document *theDoc, const NaiveDoc_ObjectList &theAddList)
    : myDoc(theDoc), myAddList(theAddList) {
  myContext = myDoc->Context();
  myObjects = myDoc->Objects();
}

NaiveDoc_CmdAddObjects::~NaiveDoc_CmdAddObjects() {}

void NaiveDoc_CmdAddObjects::undo() {
  for (const auto &anObj : myAddList) {
    myObjects->RemoveObject(anObj);
    myContext->Remove(anObj->Object(), Standard_False);
  }

  myContext->CurrentViewer()->Redraw();
}

void NaiveDoc_CmdAddObjects::redo() {
  for (const auto &anObj : myAddList) {
    myObjects->AddObject(anObj);
    myContext->Display(anObj->Object(), Standard_False);
  }

  myContext->CurrentViewer()->Redraw();
}
