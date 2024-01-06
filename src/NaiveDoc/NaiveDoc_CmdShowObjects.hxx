#ifndef _Naivis_NaiveDoc_CmdShowObjects_HeaderFile
#define _Naivis_NaiveDoc_CmdShowObjects_HeaderFile

#include "NaiveDoc_CmdDisplayObjects.hxx"

class NaiveDoc_CmdShowObjects : public NaiveDoc_CmdDisplayObjects {
public:
  template <typename T>
  NaiveDoc_CmdShowObjects(NaiveDoc_Document *theDoc, T &&theShowList,
                          Standard_Boolean theToUpdate,
                          QUndoCommand *theParent = nullptr)
      : NaiveDoc_CmdDisplayObjects(theDoc, std::forward<T>(theShowList),
                                   theToUpdate, theParent) {}

  void undo() override;

  void redo() override;
};

#endif
