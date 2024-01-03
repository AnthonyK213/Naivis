#ifndef _Naivis_NaiveDoc_CmdAddObjects_HeaderFile
#define _Naivis_NaiveDoc_CmdAddObjects_HeaderFile

#include <QUndoCommand>

#include "NaiveDoc_Object.hxx"
#include "NaiveDoc_ObjectTable.hxx"

class NaiveDoc_Document;

class NaiveDoc_CmdAddObjects : public QUndoCommand {
public:
  NaiveDoc_CmdAddObjects(NaiveDoc_Document *theDoc,
                         const NaiveDoc_ObjectList &theAddList,
                         Standard_Boolean theToUpdate,
                         QUndoCommand *theParent = nullptr);

  ~NaiveDoc_CmdAddObjects();

  void undo() override;

  void redo() override;

private:
  NaiveDoc_Document *myDoc;
  NaiveDoc_ObjectList myAddList;
  Standard_Boolean myToUpdate;
};

#endif
