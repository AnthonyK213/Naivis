#ifndef _Naivis_NaiveDoc_CmdRemoveObjects_HeaderFile
#define _Naivis_NaiveDoc_CmdRemoveObjects_HeaderFile

#include <QUndoCommand>

class NaiveDoc_CmdRemoveObjects : public QUndoCommand {
public:
  NaiveDoc_CmdRemoveObjects();

  ~NaiveDoc_CmdRemoveObjects();

  void undo() override;

  void redo() override;

private:
};

#endif
