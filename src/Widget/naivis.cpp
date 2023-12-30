#include "naivis.h"
#include "./ui_naivis.h"

#include <QIcon>
#include <QStyle>

#include <Mesh/Mesh_Util.hxx>
#include <naivecgl/BndShape/ConvexHull.h>
#include <naivecgl/Tessellation/Sphere.h>

#include <Ext/Ext_NaiveDoc.hxx>
#include <LuaOCCT/luaocct.h>

Naivis::Naivis(QWidget *parent) : QMainWindow(parent), ui(new Ui::Naivis) {
  ui->setupUi(this);

  setupActions();
  setupActionIcons();
  setupOutputBuffer();
  setupScriptEditor();

  myDoc = new NaiveDoc_Document();
  myDoc->SetContext(ui->occtViewer->Context());

  ui->occtViewer->Context()
      ->HighlightStyle(Prs3d_TypeOfHighlight_Selected)
      ->SetColor(Quantity_NOC_YELLOW);

  setupLuaState();

  ui->actionOrthographic->trigger();
}

Naivis::~Naivis() {
  lua_close(myL);
  delete ui;
}

// SLOTS {{{

void Naivis::importFile() {
  QString selectedFilter{};

  QString filePath = QFileDialog::getOpenFileName(
      this, "Import", "", tr("STEP (*.stp *.step);;STL (*.stl)"),
      &selectedFilter);

  if (filePath.isEmpty())
    return;

  myDoc->ImportStep(filePath.toUtf8().toStdString().c_str());
}

void Naivis::exportFile() {
  QString dirPath = QFileDialog::getExistingDirectory(this, "Export");

  if (dirPath.isEmpty())
    return;
}

void Naivis::openScript() {
  QString filePath =
      QFileDialog::getOpenFileName(this, "Open", "", tr("Lua (*.lua)"));

  if (filePath.isEmpty())
    return;

  QFile file{filePath};
  file.open(QFile::ReadOnly | QFile::Text);
  ui->scriptEditor->setPlainText(file.readAll());
}

void Naivis::saveScript() {
  QString filePath =
      QFileDialog::getSaveFileName(this, "Save", "", tr("Lua (*.lua)"));

  if (filePath.isEmpty())
    return;

  if (!filePath.endsWith(".lua", Qt::CaseInsensitive))
    filePath += ".lua";

  QFile file{filePath};
  file.open(QFile::ReadWrite | QIODevice::Truncate | QIODevice::Text);
  file.write(ui->scriptEditor->toPlainText().toUtf8().toStdString().c_str());
}

void Naivis::quit() { close(); }

void Naivis::transform() {}

void Naivis::undo() {
  myDoc->Undo();
  update();
}

void Naivis::redo() {
  myDoc->Redo();
  update();
}

void Naivis::selectAll() {}

void Naivis::removeCurrentSelection() {}

void Naivis::runScript() {
  QString script = ui->scriptEditor->toPlainText();

  if (luaL_dostring(myL, script.toUtf8().toStdString().c_str()) != 0) {
    std::cout << lua_tostring(myL, -1) << '\n';
    lua_pop(myL, -1);
  }
}

// }}}

#define CONNECT_ACTION(A, F) connect(A, &QAction::triggered, this, &Naivis::F);

void Naivis::setupActions() {
  /// File
  CONNECT_ACTION(ui->actionImport, importFile);
  CONNECT_ACTION(ui->actionExport, exportFile);
  CONNECT_ACTION(ui->actionOpenScript, openScript);
  CONNECT_ACTION(ui->actionSaveScript, saveScript);
  CONNECT_ACTION(ui->actionQuit, quit);

  /// Edit
  CONNECT_ACTION(ui->actionUndo, undo);
  CONNECT_ACTION(ui->actionRedo, redo);
  CONNECT_ACTION(ui->actionTransform, transform);
  CONNECT_ACTION(ui->actionSelectAll, selectAll);
  CONNECT_ACTION(ui->actionDelete, removeCurrentSelection);

  /// View
  connect(ui->actionOrthographic, &QAction::triggered, [this] {
    ui->actionOrthographic->setChecked(true);
    ui->actionPerspective->setChecked(false);
    setViewProjectionType(
        Graphic3d_Camera::Projection::Projection_Orthographic);
  });
  connect(ui->actionPerspective, &QAction::triggered, [this] {
    ui->actionOrthographic->setChecked(false);
    ui->actionPerspective->setChecked(true);
    setViewProjectionType(Graphic3d_Camera::Projection::Projection_Perspective);
  });

  /// Script
  CONNECT_ACTION(ui->actionRunScript, runScript);

  /// Help
}

#undef CONNECT_ACTION

#define STANDARD_ICON(icon) QApplication::style()->standardIcon(QStyle::icon)

void Naivis::setupActionIcons() {
  ui->actionImport->setIcon(STANDARD_ICON(SP_FileIcon));
  ui->actionExport->setIcon(STANDARD_ICON(SP_DialogSaveButton));
  ui->actionUndo->setIcon(STANDARD_ICON(SP_ArrowBack));
  ui->actionRedo->setIcon(STANDARD_ICON(SP_ArrowRight));
  ui->actionRunScript->setIcon(STANDARD_ICON(SP_MediaPlay));
  ui->actionAbout->setIcon(STANDARD_ICON(SP_MessageBoxInformation));
}

#undef STANDARD_ICON

void Naivis::setupOutputBuffer() {
  ui->outputBuffer->document()->setMaximumBlockCount(9001);
  myLogStream = new IO_LogStream(std::cout, ui->outputBuffer);
}

void Naivis::setupScriptEditor() {
  ui->scriptEditor->setFont(QFont("Monospace"));
}

void Naivis::setupLuaState() {
  myL = luaL_newstate();

  luaL_openlibs(myL);
  luaopen_luaocct(myL);
  Ext_NaiveDoc(myL);

  LuaBridge__G(myL)
      .Begin_Namespace(Naivis)
      .addVariable("ActiveDoc", myDoc)
      .End_Namespace();
}

Widget_OcctViewer *Naivis::occtViewer() { return ui->occtViewer; }

void Naivis::setViewProjectionType(
    Graphic3d_Camera::Projection projectionType) {
  occtViewer()->View()->Camera()->SetProjectionType(projectionType);
  occtViewer()->View()->FitAll();
  occtViewer()->Viewer()->Redraw();
}

// vim: set foldmarker={{{,}}} foldmethod=marker foldlevel=0:
