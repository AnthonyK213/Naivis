#ifndef _Naivis_Command_TransformObjects_HeaderFile
#define _Naivis_Command_TransformObjects_HeaderFile

#include <QListView>
#include <QStringListModel>
#include <QUndoCommand>

#include <Widget/Widget_OcctViewer.hxx>

class Command_TransformObjects final : public QUndoCommand {
public:
  Command_TransformObjects(Widget_OcctViewer *theOcctViewer,
                           QUndoCommand *parent = nullptr);

  ~Command_TransformObjects();

  void undo() override;

  void redo() override;

private:
  Widget_OcctViewer *myOcctViewer;
};

#endif
