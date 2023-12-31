﻿#include "NaiveDoc_CmdAddObjects.hxx"
#include "NaiveDoc_Document.hxx"

void NaiveDoc_CmdAddObjects::undo() {
  if (myOkIndices.isEmpty())
    return;

  for (Standard_Integer i : myOkIndices) {
    myDoc->Objects()->deleteObjectRaw(myDisplayList[i]);
  }

  if (myToUpdate)
    myDoc->UpdateView();
}

void NaiveDoc_CmdAddObjects::redo() {
  myOkIndices.clear();

  for (Standard_Integer i = 0; i < myDisplayList.size(); ++i) {
    if (myDoc->Objects()->addObjectRaw(myDisplayList[i])) {
      myOkIndices.push_back(i);
    }
  }

  if (!myOkIndices.isEmpty() && myToUpdate)
    myDoc->UpdateView();
}
