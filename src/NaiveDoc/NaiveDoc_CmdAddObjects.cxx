#include "NaiveDoc_CmdAddObjects.hxx"
#include "NaiveDoc_Document.hxx"
#include "V3d_View.hxx"
#include "V3d_Viewer.hxx"

NaiveDoc_CmdAddObjects::~NaiveDoc_CmdAddObjects() {}

void NaiveDoc_CmdAddObjects::undo() {
  if (myOkIndices.isEmpty())
    return;

  for (Standard_Integer i : myOkIndices) {
    myDoc->Objects()->deleteObjectRaw(myAddList[i]);
  }

  if (myToUpdate)
    myDoc->UpdateView();
}

void NaiveDoc_CmdAddObjects::redo() {
  myOkIndices.clear();

  for (Standard_Integer i = 0; i < myAddList.size(); ++i) {
    if (myDoc->Objects()->addObjectRaw(myAddList[i])) {
      myOkIndices.push_back(i);
    }
  }

  if (!myOkIndices.isEmpty() && myToUpdate)
    myDoc->UpdateView();
}

NaiveDoc_IdList NaiveDoc_CmdAddObjects::Id() const {
  NaiveDoc_IdList result{};

  if (myOkIndices.isEmpty())
    return result;

  result.reserve(myOkIndices.size());

  for (Standard_Integer i : myOkIndices) {
    result.push_back(NaiveDoc_Object_GetId(*myAddList[i]));
  }

  return result;
}
