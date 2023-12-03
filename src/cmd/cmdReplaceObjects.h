#ifndef NAIVIS_CMD_CMDREPLACEOBJECTS_H
#define NAIVIS_CMD_CMDREPLACEOBJECTS_H

#include "common.h"

class CmdReplaceObjects final : public QUndoCommand {
public:
  CmdReplaceObjects(OcctViewer *theOcctViewer, QUndoCommand *parent = nullptr);

  ~CmdReplaceObjects();

  void undo() override;

  void redo() override;

private:
  OcctViewer *m_occtViewer;
};

#endif
