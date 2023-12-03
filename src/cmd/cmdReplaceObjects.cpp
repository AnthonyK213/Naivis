#include "cmdReplaceObjects.h"

CmdReplaceObjects::CmdReplaceObjects(OcctViewer *theOcctViewer,
                                     QUndoCommand *parent)
    : QUndoCommand(parent), m_occtViewer(theOcctViewer) {}

CmdReplaceObjects::~CmdReplaceObjects() {}

void CmdReplaceObjects::undo() {}

void CmdReplaceObjects::redo() {}
