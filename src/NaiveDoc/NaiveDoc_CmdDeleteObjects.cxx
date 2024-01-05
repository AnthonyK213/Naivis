#include "NaiveDoc_CmdDeleteObjects.hxx"
#include "NaiveDoc_Document.hxx"

NaiveDoc_CmdDeleteObjects::~NaiveDoc_CmdDeleteObjects() {}

void NaiveDoc_CmdDeleteObjects::undo() {
  if (myOkIndices.isEmpty())
    return;

  for (Standard_Integer i : myOkIndices) {
    myDoc->Objects()->addObjectRaw(myDeleteList[i]);
  }

  if (myToUpdate)
    myDoc->UpdateView();
}

void NaiveDoc_CmdDeleteObjects::redo() {
  myOkIndices.clear();

  for (Standard_Integer i = 0; i < myDeleteList.size(); ++i) {
    if (myDoc->Objects()->deleteObjectRaw(myDeleteList[i])) {
      myOkIndices.push_back(i);
    }
  }

  if (!myOkIndices.isEmpty() && myToUpdate)
    myDoc->UpdateView();
}
