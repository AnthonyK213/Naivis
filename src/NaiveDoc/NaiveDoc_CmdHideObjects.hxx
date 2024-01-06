#ifndef _Naivis_NaiveDoc_CmdHideObjects_HeaderFile
#define _Naivis_NaiveDoc_CmdHideObjects_HeaderFile

#include "NaiveDoc_CmdDisplayObjects.hxx"

class NaiveDoc_CmdHideObjects : public NaiveDoc_CmdDisplayObjects {
public:
  template <typename T>
  NaiveDoc_CmdHideObjects(NaiveDoc_Document *theDoc, T &&theHideList,
                          Standard_Boolean theToUpdate,
                          QUndoCommand *theParent = nullptr)
      : NaiveDoc_CmdDisplayObjects(theDoc, std::forward<T>(theHideList),
                                   theToUpdate, theParent) {}

  void undo() override;

  void redo() override;
};

#endif
