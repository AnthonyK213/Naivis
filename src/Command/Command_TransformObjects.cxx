#include "Command_TransformObjects.hxx"

Command_TransformObjects::Command_TransformObjects(
    Widget_OcctViewer *theOcctViewer, QUndoCommand *parent)
    : QUndoCommand(parent), myOcctViewer(theOcctViewer) {}

Command_TransformObjects::~Command_TransformObjects() {}

void Command_TransformObjects::undo() {}

void Command_TransformObjects::redo() {}
