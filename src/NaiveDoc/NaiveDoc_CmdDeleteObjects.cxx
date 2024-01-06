#include "NaiveDoc_CmdDeleteObjects.hxx"
#include "NaiveDoc_Document.hxx"

void NaiveDoc_CmdDeleteObjects::undo() {
  if (myOkIndices.isEmpty())
    return;

  for (Standard_Integer i : myOkIndices) {
    myDoc->Objects()->addObjectRaw(myDisplayList[i]);
  }

  if (myToUpdate)
    myDoc->UpdateView();
}

void NaiveDoc_CmdDeleteObjects::redo() {
  myOkIndices.clear();

  switch (myKind) {
  case NaiveDoc_CmdDeleteObjects::Delete: {
    for (Standard_Integer i = 0; i < myDisplayList.size(); ++i) {
      if (myDoc->Objects()->deleteObjectRaw(myDisplayList[i])) {
        myOkIndices.push_back(i);
      }
    }
    break;
  }

  case NaiveDoc_CmdDeleteObjects::Purge: {
    for (Standard_Integer i = 0; i < myDisplayList.size(); ++i) {
      if (myDoc->Objects()->purgeObjectRaw(myDisplayList[i])) {
        myOkIndices.push_back(i);
      }
    }
    break;
  }

  default:
    return;
  }

  if (!myOkIndices.isEmpty() && myToUpdate)
    myDoc->UpdateView();
}
