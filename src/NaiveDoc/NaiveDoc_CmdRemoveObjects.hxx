#ifndef _Naivis_NaiveDoc_CmdRemoveObjects_HeaderFile
#define _Naivis_NaiveDoc_CmdRemoveObjects_HeaderFile

#include <QUndoCommand>

#include "NaiveDoc_Object.hxx"
#include "NaiveDoc_ObjectTable.hxx"

class NaiveDoc_Document;

class NaiveDoc_CmdRemoveObjects : public QUndoCommand {
public:
  NaiveDoc_CmdRemoveObjects(NaiveDoc_Document *theDoc,
                            const NaiveDoc_ObjectList &theRemoveList,
                            Standard_Boolean theToUpdate,
                            QUndoCommand *theParent = nullptr);

  ~NaiveDoc_CmdRemoveObjects();

  void undo() override;

  void redo() override;

private:
  NaiveDoc_Document *myDoc;
  NaiveDoc_ObjectList myRemoveList;
  Standard_Boolean myToUpdate;
};

#endif
