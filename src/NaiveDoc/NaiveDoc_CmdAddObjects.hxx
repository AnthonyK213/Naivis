#ifndef _Naivis_NaiveDoc_CmdAddObjects_HeaderFile
#define _Naivis_NaiveDoc_CmdAddObjects_HeaderFile

#include "NaiveDoc_CmdDisplayObjects.hxx"

class NaiveDoc_CmdAddObjects : public NaiveDoc_CmdDisplayObjects {
public:
  template <typename T>
  NaiveDoc_CmdAddObjects(NaiveDoc_Document *theDoc, T &&theAddList,
                         Standard_Boolean theToUpdate,
                         QUndoCommand *theParent = nullptr)
      : NaiveDoc_CmdDisplayObjects(theDoc, std::forward<T>(theAddList),
                                   theToUpdate, theParent) {}

  void undo() override;

  void redo() override;
};

#endif
