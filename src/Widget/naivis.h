﻿#ifndef _Naivis_Widget_Naivis_Header
#define _Naivis_Widget_Naivis_Header

#include <QFileDialog>
#include <QFileInfo>
#include <QMainWindow>
#include <QString>

#include <NaiveDoc/NaiveDoc_Document.hxx>

QT_BEGIN_NAMESPACE
namespace Ui {
class Naivis;
}
QT_END_NAMESPACE

class IO_LogStream;
class NaiveApp_Settings;
class Widget_OcctViewer;

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

  void hideCurrentSelection();

  void showAll();

  void deleteCurrentSelection();

  // }}}

  // Script {{{

  void runScript();

  // }}}

  void updateSelectionPropertiesTable(const NaiveDoc_ObjectList &theSelections);

  void updateAssemblyTree(const Handle(NaiveDoc_Document) & theDoc);

  void clearOutputBuffer();

private:
  void setupSettings();

  void setupActions();

  void setupActionIcons();

  void setupOutputBuffer();

  void setupScriptEditor();

  bool setupLua();

  Widget_OcctViewer *occtViewer();

  const Handle(NaiveDoc_Document) & document() const;

  NaiveApp_Settings *settings() const;

  void setViewProjectionType(Graphic3d_Camera::Projection projectionType);

  void setupSelectionPropertiesTable();

  void setupAssemblyTree();

private:
  class LuaManager;

private:
  Ui::Naivis *ui;
  IO_LogStream *myLogStream;
  NaiveApp_Settings *mySettings;
  LuaManager *myLuaMgr;
};

#endif // NAIVIS_H

// vim: set foldmarker={{{,}}} foldmethod=marker foldlevel=0:
