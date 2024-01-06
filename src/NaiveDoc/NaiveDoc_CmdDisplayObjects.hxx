#ifndef _Naivis_NaiveDoc_CmdDisplayObjects_HeaderFile
#define _Naivis_NaiveDoc_CmdDisplayObjects_HeaderFile

#include <QUndoCommand>

#include "NaiveDoc_Object.hxx"
#include "NaiveDoc_ObjectTable.hxx"

class NaiveDoc_Document;

class NaiveDoc_CmdDisplayObjects : public QUndoCommand {
public:
  template <typename T>
  NaiveDoc_CmdDisplayObjects(NaiveDoc_Document *theDoc, T &&theDisplayList,
                             Standard_Boolean theToUpdate,
                             QUndoCommand *theParent = nullptr)
      : QUndoCommand(theParent), myDoc(theDoc),
        myDisplayList(std::forward<T>(theDisplayList)), myOkIndices(),
        myToUpdate(theToUpdate) {
    myOkIndices.reserve(myDisplayList.size());
  }

  virtual ~NaiveDoc_CmdDisplayObjects() {}

  virtual void undo() override {}

  virtual void redo() override {}

  Standard_Integer Size() const {
    return static_cast<Standard_Integer>(myOkIndices.size());
  }

  NaiveDoc_IdList Id() const {
    NaiveDoc_IdList result{};

    if (myOkIndices.isEmpty())
      return result;

    result.reserve(myOkIndices.size());

    for (Standard_Integer i : myOkIndices) {
      result.push_back(NaiveDoc_Object_GetId(*myDisplayList[i]));
    }

    return result;
  }

protected:
  NaiveDoc_Document *myDoc;
  NaiveDoc_ObjectList myDisplayList;
  QList<Standard_Integer> myOkIndices;
  Standard_Boolean myToUpdate;
};

#endif
