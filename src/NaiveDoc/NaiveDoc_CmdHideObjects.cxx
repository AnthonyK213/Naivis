#include "NaiveDoc_CmdHideObjects.hxx"
#include "NaiveDoc_Document.hxx"

void NaiveDoc_CmdHideObjects::undo() {
  if (myOkIndices.isEmpty())
    return;

  for (Standard_Integer i : myOkIndices) {
    myDoc->Objects()->showObjectRaw(myDisplayList[i]);
  }

  if (myToUpdate)
    myDoc->UpdateView();
}

void NaiveDoc_CmdHideObjects::redo() {
  myOkIndices.clear();

  for (Standard_Integer i = 0; i < myDisplayList.size(); ++i) {
    if (myDoc->Objects()->hideObjectRaw(myDisplayList[i])) {
      myOkIndices.push_back(i);
    }
  }

  if (!myOkIndices.isEmpty() && myToUpdate)
    myDoc->UpdateView();
}
