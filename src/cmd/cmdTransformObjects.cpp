#include "cmdTransformObjects.h"

// CmdTransformObjects
CmdTransformObjects::CmdTransformObjects(OcctViewer *theOcctViewer,
                                         QUndoCommand *parent)
    : QUndoCommand(parent), m_occtViewer(theOcctViewer) {}

CmdTransformObjects::~CmdTransformObjects() {}

void CmdTransformObjects::undo() {}

void CmdTransformObjects::redo() {}
