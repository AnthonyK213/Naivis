#ifndef _Naivis_NaiveDoc_CmdDeleteObjects_HeaderFile
#define _Naivis_NaiveDoc_CmdDeleteObjects_HeaderFile

#include "NaiveDoc_CmdDisplayObjects.hxx"

class NaiveDoc_CmdDeleteObjects : public NaiveDoc_CmdDisplayObjects {
public:
  enum Kind {
    Delete = 0x01,
    Purge = 0x03,
  };

public:
  template <typename T>
  NaiveDoc_CmdDeleteObjects(NaiveDoc_Document *theDoc, Kind theKind,
                            T &&theDeleteList, Standard_Boolean theToUpdate,
                            QUndoCommand *theParent = nullptr)
      : NaiveDoc_CmdDisplayObjects(theDoc, std::forward<T>(theDeleteList),
                                   theToUpdate, theParent),
        myKind(theKind) {}

  void undo() override;

  void redo() override;

private:
  Kind myKind;
};

#endif
