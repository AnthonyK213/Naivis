#ifndef NAIVIS_CMD_CMDREMOVEOBJECTS_H
#define NAIVIS_CMD_CMDREMOVEOBJECTS_H

#include "common.h"

class CmdRemoveObjects final : public QUndoCommand {
public:
  CmdRemoveObjects(OcctViewer *theOcctViewer, QUndoCommand *parent = nullptr);

  ~CmdRemoveObjects();

  void undo() override;

  void redo() override;

private:
  OcctViewer *m_occtViewer;
};

#endif
