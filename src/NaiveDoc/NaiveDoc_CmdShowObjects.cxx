#include "NaiveDoc_CmdShowObjects.hxx"
#include "NaiveDoc_Document.hxx"

void NaiveDoc_CmdShowObjects::undo() {
  if (myOkIndices.isEmpty())
    return;

  for (Standard_Integer i : myOkIndices) {
    myDoc->Objects()->hideObjectRaw(myDisplayList[i]);
  }

  if (myToUpdate)
    myDoc->UpdateView();
}

void NaiveDoc_CmdShowObjects::redo() {
  myOkIndices.clear();

  for (Standard_Integer i = 0; i < myDisplayList.size(); ++i) {
    if (myDoc->Objects()->showObjectRaw(myDisplayList[i])) {
      myOkIndices.push_back(i);
    }
  }

  if (!myOkIndices.isEmpty() && myToUpdate)
    myDoc->UpdateView();
}
