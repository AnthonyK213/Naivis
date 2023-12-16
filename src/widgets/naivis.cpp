#include "naivis.h"
#include "./ui_naivis.h"

#include <mesh/util.h>
#include <naivecgl/BndShape/ConvexHull.h>
#include <naivecgl/Tessellation/Sphere.h>

#include <BRepBuilderAPI_MakeVertex.hxx>

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

void Naivis::meshing() {
  auto aSphere = naivecgl::tessellation::tetrasphere({42, -10, 66}, 100);

  if (aSphere.get() == nullptr)
    return;

  auto aMesh = naivis::mesh::naivePoly3DToMesh(*aSphere);
  auto aMeshPrs = naivis::mesh::createMeshVS(aMesh);

  if (aMeshPrs.IsNull())
    return;

  occtViewer()->Context()->Display(aMeshPrs, true);
}

void Naivis::convexHull2D() {
  Naive_List<Naive_Point2d> points(512);

  std::srand(std::time(0));

  for (Naive_Point2d &point : points) {
    point(0) = (Naive_Real)std::rand() / RAND_MAX * 20.0 - 10.0;
    point(1) = (Naive_Real)std::rand() / RAND_MAX * 20.0 - 10.0;
    
    point *= (Naive_Real)std::rand() / RAND_MAX;

    BRepBuilderAPI_MakeVertex vertex({point.x(), point.y(), 0.0});
    Handle(AIS_Shape) shape = new AIS_Shape(vertex);

    occtViewer()->Context()->Display(shape, false);
  }

  Naive_List<Naive_Integer> convexIndices{};
  naivecgl::bndshape::convexHull2D(points, convexIndices);

  for (Naive_Integer i = 0; i < convexIndices.size(); ++i) {
    Naive_Integer j = (i + 1) % convexIndices.size();

    const Naive_Point2d &a = points[convexIndices[i]];
    const Naive_Point2d &b = points[convexIndices[j]];
    BRepBuilderAPI_MakeEdge edge({a.x(), a.y(), 0.0}, {b.x(), b.y(), 0.0});
    Handle(AIS_Shape) shape = new AIS_Shape(edge);

    occtViewer()->Context()->Display(shape, false);
  }
}

// }}}

#define CONNECT_ACTION(A, F) connect(A, &QAction::triggered, this, &Naivis::F);

void Naivis::setupActions() {
  /// File
  CONNECT_ACTION(ui->actionImport, importFile);
  CONNECT_ACTION(ui->actionExport, exportFile);
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

  /// Algo
  CONNECT_ACTION(ui->actionMeshing, meshing);
  CONNECT_ACTION(ui->actionConvexHull2D, convexHull2D);

  /// Help
}

#undef CONNECT_ACTION

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
