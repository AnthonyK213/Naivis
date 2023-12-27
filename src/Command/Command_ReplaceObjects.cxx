#include "Command_ReplaceObjects.hxx"

Command_ReplaceObjects::Command_ReplaceObjects(Widget_OcctViewer *theOcctViewer,
                                               QUndoCommand *parent)
    : QUndoCommand(parent), myOcctViewer(theOcctViewer) {}

Command_ReplaceObjects::~Command_ReplaceObjects() {}

void Command_ReplaceObjects::undo() {}

void Command_ReplaceObjects::redo() {}
