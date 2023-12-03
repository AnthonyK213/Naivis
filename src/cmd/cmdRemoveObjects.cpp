#include "cmdRemoveObjects.h"

CmdRemoveObjects::CmdRemoveObjects(OcctViewer *theOcctViewer,
                                   QUndoCommand *parent)
    : QUndoCommand(parent), m_occtViewer(theOcctViewer) {}

CmdRemoveObjects::~CmdRemoveObjects() {}

void CmdRemoveObjects::undo() {}

void CmdRemoveObjects::redo() {}
