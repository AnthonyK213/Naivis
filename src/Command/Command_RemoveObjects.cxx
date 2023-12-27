#include "Command_RemoveObjects.hxx"

Command_RemoveObjects::Command_RemoveObjects(Widget_OcctViewer *theOcctViewer,
                                             QUndoCommand *parent)
    : QUndoCommand(parent), myOcctViewer(theOcctViewer) {}

Command_RemoveObjects::~Command_RemoveObjects() {}

void Command_RemoveObjects::undo() {}

void Command_RemoveObjects::redo() {}
