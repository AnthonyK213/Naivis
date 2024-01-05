#ifndef _Naivis_NaiveDoc_CmdDeleteObjects_HeaderFile
#define _Naivis_NaiveDoc_CmdDeleteObjects_HeaderFile

#include <QUndoCommand>

#include "NaiveDoc_Object.hxx"
#include "NaiveDoc_ObjectTable.hxx"

class NaiveDoc_Document;

class NaiveDoc_CmdDeleteObjects : public QUndoCommand {
public:
  enum Kind {
    Hide = 0x00,
    Delete = 0x01,
    Purge = 0x03,
  };

public:
  template <typename T>
  NaiveDoc_CmdDeleteObjects(NaiveDoc_Document *theDoc, Kind theKind,
                            T &&theDeleteList, Standard_Boolean theToUpdate,
                            QUndoCommand *theParent = nullptr)
      : QUndoCommand(theParent), myDoc(theDoc), myKind(theKind),
        myDeleteList(std::forward<T>(theDeleteList)), myOkIndices(),
        myToUpdate(theToUpdate) {
    myOkIndices.reserve(myDeleteList.size());
  }

  ~NaiveDoc_CmdDeleteObjects();

  void undo() override;

  void redo() override;

  Standard_Integer Size() const {
    return static_cast<Standard_Integer>(myOkIndices.size());
  }

private:
  NaiveDoc_Document *myDoc;
  Kind myKind;
  NaiveDoc_ObjectList myDeleteList;
  QList<Standard_Integer> myOkIndices;
  Standard_Boolean myToUpdate;
};

#endif
