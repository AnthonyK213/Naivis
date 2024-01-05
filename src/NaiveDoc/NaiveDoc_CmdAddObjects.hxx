#ifndef _Naivis_NaiveDoc_CmdAddObjects_HeaderFile
#define _Naivis_NaiveDoc_CmdAddObjects_HeaderFile

#include <QUndoCommand>

#include "NaiveDoc_Object.hxx"
#include "NaiveDoc_ObjectTable.hxx"

class NaiveDoc_Document;

class NaiveDoc_CmdAddObjects : public QUndoCommand {
public:
  template <typename T>
  NaiveDoc_CmdAddObjects(NaiveDoc_Document *theDoc, T &&theAddList,
                         Standard_Boolean theToUpdate,
                         QUndoCommand *theParent = nullptr)
      : QUndoCommand(theParent), myDoc(theDoc),
        myAddList(std::forward<T>(theAddList)), myOkIndices(),
        myToUpdate(theToUpdate) {
    myOkIndices.reserve(myAddList.size());
  }

  ~NaiveDoc_CmdAddObjects();

  void undo() override;

  void redo() override;

  Standard_Integer NbId() const {
    return static_cast<Standard_Integer>(myOkIndices.size());
  }

  NaiveDoc_IdList Id() const;

private:
  NaiveDoc_Document *myDoc;
  NaiveDoc_ObjectList myAddList;
  QList<Standard_Integer> myOkIndices;
  Standard_Boolean myToUpdate;
};

#endif
