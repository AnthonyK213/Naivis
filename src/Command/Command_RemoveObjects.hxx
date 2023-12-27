#ifndef _Naivis_Command_RemoveObjects_HeaderFile
#define _Naivis_Command_RemoveObjects_HeaderFile

#include <QListView>
#include <QStringListModel>
#include <QUndoCommand>

#include <Widget/Widget_OcctViewer.hxx>

class Command_RemoveObjects final : public QUndoCommand {
public:
  Command_RemoveObjects(Widget_OcctViewer *theOcctViewer,
                        QUndoCommand *parent = nullptr);

  ~Command_RemoveObjects();

  void undo() override;

  void redo() override;

private:
  Widget_OcctViewer *myOcctViewer;
};

#endif
