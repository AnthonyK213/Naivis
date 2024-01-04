#ifndef _Naivis_Widget_Naivis_Header
#define _Naivis_Widget_Naivis_Header

#include <QFileDialog>
#include <QFileInfo>
#include <QMainWindow>
#include <QString>

#include <IO/IO_LogStream.hxx>
#include <NaiveDoc/NaiveDoc_Document.hxx>
#include <Widget/Widget_OcctViewer.hxx>

#include <lua.hpp>

QT_BEGIN_NAMESPACE
namespace Ui {
class Naivis;
}
QT_END_NAMESPACE

class Naivis : public QMainWindow {
  Q_OBJECT

public:
  Naivis(QWidget *parent = nullptr);

  ~Naivis();

private slots:
  // File {{{

  void importFile();

  void exportFile();

  void openScript();

  void saveScript();

  void quit();

  // }}}

  // Edit {{{

  void undo();

  void redo();

  void transform();

  void selectAll();

  void removeCurrentSelection();

  // }}}

  // Script {{{

  void runScript();

  // }}}

private:
  void setupActions();

  void setupActionIcons();

  void setupOutputBuffer();

  void setupScriptEditor();

  void setupLuaState();

  Widget_OcctViewer *occtViewer();

  void setViewProjectionType(Graphic3d_Camera::Projection projectionType);

private:
  Ui::Naivis *ui;
  IO_LogStream *myLogStream;
  Handle(NaiveDoc_Document) myDoc;
  lua_State *myL;
};

#endif // NAIVIS_H

// vim: set foldmarker={{{,}}} foldmethod=marker foldlevel=0:
