#ifndef NAIVIS_CMD_CMDTRANSFORMOBJECTS_H
#define NAIVIS_CMD_CMDTRANSFORMOBJECTS_H

#include "common.h"

class CmdTransformObjects final : public QUndoCommand {
public:
  CmdTransformObjects(OcctViewer *theOcctViewer,
                      QUndoCommand *parent = nullptr);

  ~CmdTransformObjects();

  void undo() override;

  void redo() override;

private:
  OcctViewer *m_occtViewer;
};

#endif
