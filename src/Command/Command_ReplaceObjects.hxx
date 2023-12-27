#ifndef _Naivis_Command_ReplaceObjects_HeaderFile
#define _Naivis_Command_ReplaceObjects_HeaderFile

#include <QListView>
#include <QStringListModel>
#include <QUndoCommand>

#include <Widget/Widget_OcctViewer.hxx>

class Command_ReplaceObjects final : public QUndoCommand {
public:
  Command_ReplaceObjects(Widget_OcctViewer *theOcctViewer,
                         QUndoCommand *parent = nullptr);

  ~Command_ReplaceObjects();

  void undo() override;

  void redo() override;

private:
  Widget_OcctViewer *myOcctViewer;
};

#endif
