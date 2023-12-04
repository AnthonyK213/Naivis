#include "naivis.h"
#include "./ui_naivis.h"

Naivis::Naivis(QWidget *parent) : QMainWindow(parent), ui(new Ui::Naivis) {
  ui->setupUi(this);

  setupActions();
  setupOutputBuffer();

  ui->actionOrthographic->trigger();
}

Naivis::~Naivis() { delete ui; }

// SLOTS {{{

void Naivis::importFile() {
  QString selectedFilter{};

  QString filePath = QFileDialog::getOpenFileName(
      this, "Open", "", tr("STEP (*.stp;*.step);;STL (*.stl)"),
      &selectedFilter);

  if (filePath.isEmpty())
    return;
}

void Naivis::exportFile() {
  QString dirPath = QFileDialog::getExistingDirectory(this, "Export");

  if (dirPath.isEmpty())
    return;
}

void Naivis::quit() { close(); }

void Naivis::transform() {}

void Naivis::undo() {}

void Naivis::redo() {}

void Naivis::selectAll() {}

void Naivis::removeCurrentSelection() {}

// }}}

void Naivis::setupActions() {
  /// File
  connect(ui->actionImport, &QAction::triggered, this, &Naivis::importFile);
  connect(ui->actionExport, &QAction::triggered, this, &Naivis::exportFile);
  connect(ui->actionQuit, &QAction::triggered, this, &Naivis::quit);

  /// Edit
  connect(ui->actionUndo, &QAction::triggered, this, &Naivis::undo);
  connect(ui->actionRedo, &QAction::triggered, this, &Naivis::redo);
  connect(ui->actionTransform, &QAction::triggered, this, &Naivis::transform);
  connect(ui->actionSelectAll, &QAction::triggered, this, &Naivis::selectAll);
  connect(ui->actionDelete, &QAction::triggered, this,
          &Naivis::removeCurrentSelection);

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

  /// Help
}

void Naivis::setupOutputBuffer() {
  ui->outputBuffer->document()->setMaximumBlockCount(9001);
  m_logStream = new LogStream(std::cout, ui->outputBuffer);
}

OcctViewer *Naivis::occtViewer() { return ui->occtViewer; }

void Naivis::setViewProjectionType(
    Graphic3d_Camera::Projection projectionType) {
  occtViewer()->View()->Camera()->SetProjectionType(projectionType);
  occtViewer()->Viewer()->Update();
  occtViewer()->update();
}

// vim: set foldmarker={{{,}}} foldmethod=marker foldlevel=0:
