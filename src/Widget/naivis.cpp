#include "naivis.h"
#include "./ui_naivis.h"

#include <QIcon>
#include <QStack>
#include <QStyle>

#include <Ext/Ext_Load.hxx>
#include <NaiveApp/NaiveApp_Application.hxx>
#include <Util/Util_AIS.hxx>
#include <Util/Util_Mesh.hxx>
#include <Util/Util_OCAF.hxx>

#include <luaocct/luaocct.h>

Naivis::Naivis(QWidget *parent) : QMainWindow(parent), ui(new Ui::Naivis) {
  ui->setupUi(this);

  QApplication::setWindowIcon(QIcon(":icons/Naivis.ico"));

  setupActions();
  setupActionIcons();
  setupOutputBuffer();
  setupScriptEditor();
  setupAssemblyTree();
  setupSelectionPropertiesTable();
  setupLuaState();

  NaiveApp_InitPrsDrivers();

  ui->actionOrthographic->trigger();
}

Naivis::~Naivis() {
  lua_close(myL);
  delete myLogStream;
  delete ui;
}

// SLOTS {{{

void Naivis::importFile() {
  QString selectedFilter{};
  QString filePath = QFileDialog::getOpenFileName(
      this, "Import", "", tr("STEP (*.stp *.step);;STL (*.stl);;PLY (*.ply)"),
      &selectedFilter);

  if (filePath.isEmpty())
    return;

  if (selectedFilter.startsWith("STEP")) {
    document()->ImportStep(filePath.toUtf8().toStdString().c_str());
  } else if (selectedFilter.startsWith("STL")) {
  } else if (selectedFilter.startsWith("PLY")) {
  } else {
  }

  updateAssemblyTree(document());
  occtViewer()->View()->FitAll();
}

void Naivis::exportFile() {
  QString selectedFilter{};
  QString filePath = QFileDialog::getSaveFileName(
      this, "Export", "", tr("STEP (*.stp *.step);;STL (*.stl);;PLY (*.ply)"),
      &selectedFilter);

  if (filePath.isEmpty())
    return;

  if (selectedFilter.startsWith("STEP")) {
    if (!filePath.endsWith(".stp", Qt::CaseInsensitive))
      filePath += ".stp";

    document()->ExportStep(filePath.toUtf8().toStdString().c_str());
  } else if (selectedFilter.startsWith("STL")) {
  } else if (selectedFilter.startsWith("PLY")) {
  } else {
  }
}

void Naivis::openScript() {
  QString filePath =
      QFileDialog::getOpenFileName(this, "Open", "", tr("Lua (*.lua)"));

  if (filePath.isEmpty())
    return;

  QFile file{filePath};
  if (file.open(QFile::ReadOnly | QFile::Text)) {
    myLuaFile = filePath;
    ui->scriptEditor->setPlainText(file.readAll());

    /// WORKAROUND: Add the file directory to the runtime path.
    QFileInfo info(filePath);
    lua_getglobal(myL, "package");
    lua_getfield(myL, -1, "path");
    std::string path = lua_tostring(myL, -1);
    path.append(";");
    path.append(info.dir().absolutePath().toUtf8().toStdString().c_str());
    path.append("/?.lua");
    lua_pop(myL, 1);
    lua_pushstring(myL, path.c_str());
    lua_setfield(myL, -2, "path");
    lua_pop(myL, 1);
  }
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
  document()->Undo();
  /// FIXME: How to excute this when needed?
  updateAssemblyTree(document());
  update();
}

void Naivis::redo() {
  document()->Redo();
  updateAssemblyTree(document());
  update();
}

void Naivis::selectAll() {
  int count = document()->Objects()->SelectAll(true);
  occtViewer()->OnSelectionChanged(occtViewer()->Context(),
                                   occtViewer()->View());
  update();
}

void Naivis::hideCurrentSelection() {
  NaiveDoc_ObjectList selection = document()->Objects()->SelectedObjects();
  int count = document()->Objects()->HideObjects(std::move(selection), true);
  std::cout << "Hid " << count << " object(s).\n";
  update();
}

void Naivis::showAll() {
  int count = document()->Objects()->ShowAll(true);
  std::cout << "Showed " << count << " hidden object(s).\n";
  update();
}

void Naivis::deleteCurrentSelection() {
  NaiveDoc_ObjectList selection = document()->Objects()->SelectedObjects();
  int count = document()->Objects()->DeleteObjects(selection, true);
  std::cout << "Deleted " << count << " object(s).\n";
  occtViewer()->OnSelectionChanged(occtViewer()->Context(),
                                   occtViewer()->View());
  updateAssemblyTree(document());
  update();
}

void Naivis::runScript() {
  QString script = ui->scriptEditor->toPlainText();

  // if (luaL_dostring(myL, script.toUtf8().toStdString().c_str()) != 0) {
  //   std::cout << lua_tostring(myL, -1) << '\n';
  //   lua_pop(myL, -1);
  // }

  if (luaL_dofile(myL, myLuaFile.toUtf8().toStdString().c_str()) != 0) {
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
  CONNECT_ACTION(ui->actionHide, hideCurrentSelection);
  CONNECT_ACTION(ui->actionShowAll, showAll);
  CONNECT_ACTION(ui->actionDelete, deleteCurrentSelection);

  /// View
  connect(ui->actionOrthographic, &QAction::triggered, [this] {
    ui->actionOrthographic->setChecked(true);
    ui->actionPerspective->setChecked(false);
    setViewProjectionType(
        Graphic3d_Camera::Projection::Projection_Orthographic);
    update();
  });
  connect(ui->actionPerspective, &QAction::triggered, [this] {
    ui->actionOrthographic->setChecked(false);
    ui->actionPerspective->setChecked(true);
    setViewProjectionType(Graphic3d_Camera::Projection::Projection_Perspective);
    update();
  });

  /// Script
  CONNECT_ACTION(ui->actionRunScript, runScript);

  /// Help

  connect(occtViewer(), &Widget_OcctViewer::selectionChanged, this,
          &Naivis::updateSelectionPropertiesTable);
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
  Ext_Load(myL);

  lua_getglobal(myL, "package");
  lua_getfield(myL, -1, "path");
  std::string path = lua_tostring(myL, -1);
  path.append(";");
  path.append(QCoreApplication::applicationDirPath().toUtf8().toStdString());
  path.append("/?.lua");
  lua_pop(myL, 1);
  lua_pushstring(myL, path.c_str());
  lua_setfield(myL, -2, "path");
  lua_pop(myL, 1);

  LuaBridge__G(myL)
      .Begin_Namespace(Naivis)
      .addVariable("ActiveDoc", document())
      .End_Namespace();
}

Widget_OcctViewer *Naivis::occtViewer() { return ui->occtViewer; }

const Handle(NaiveDoc_Document) & Naivis::document() const {
  return ui->occtViewer->Document();
}

void Naivis::setViewProjectionType(
    Graphic3d_Camera::Projection projectionType) {
  occtViewer()->View()->Camera()->SetProjectionType(projectionType);
  occtViewer()->View()->FitAll();
  occtViewer()->Viewer()->Redraw();
}

void Naivis::updateSelectionPropertiesTable(
    const NaiveDoc_ObjectList &theSelections) {
  auto *tbl = ui->tableProperties;
  auto nbSelections = theSelections.size();

  tbl->clear();

  if (theSelections.empty()) {
    tbl->setRowCount(0);
    tbl->setColumnCount(0);
  } else if (nbSelections == 1) {
    Handle(NaiveDoc_Object) anObj = theSelections[0];
    tbl->setColumnCount(2);
    QStringList aProps = Util_AIS::GetObjectProperties(anObj);
    auto nbRow = aProps.size() >> 1;
    tbl->setRowCount(nbRow);
    QStringList header;
    header << "Property"
           << "Value";
    tbl->setHorizontalHeaderLabels(header);

    for (int i = 0; i < nbRow; ++i) {
      tbl->setItem(i, 0, new QTableWidgetItem(aProps[i << 1]));
      tbl->setItem(i, 1, new QTableWidgetItem(aProps[(i << 1) + 1]));
    }
  } else {
    tbl->setRowCount(1);
    tbl->setColumnCount(1);
    QStringList header;
    header << "Selection";
    tbl->setHorizontalHeaderLabels(header);
    tbl->setItem(
        0, 0, new QTableWidgetItem(QString("%1 object(s)").arg(nbSelections)));
  }

  tbl->update();
}

void Naivis::setupSelectionPropertiesTable() {
  auto *tbl = ui->tableProperties;
  tbl->setEditTriggers(QAbstractItemView::NoEditTriggers);
  tbl->setSelectionBehavior(QAbstractItemView::SelectRows);
  tbl->horizontalHeader()->setVisible(true);
  tbl->verticalHeader()->setVisible(false);
  tbl->horizontalHeader()->setStretchLastSection(true);
}

void Naivis::setupAssemblyTree() {
  connect(document().get(), &NaiveDoc_Document::OnAddObject, this,
          &Naivis::updateAssemblyTree);
}

void Naivis::updateAssemblyTree(const Handle(NaiveDoc_Document) & theDoc) {
  QTreeWidget *aTree = ui->treeAssemblies;

  if (theDoc.IsNull() || !aTree)
    return;

  aTree->clear();

  QTreeWidgetItem *aRoot = new QTreeWidgetItem();
  aRoot->setText(0, "Assemblies");
  QStack<QTreeWidgetItem *> aStack{};
  aStack.push(aRoot);

  for (XCAFPrs_DocumentExplorer aDocExpl = theDoc->GetXcafExplorer();
       aDocExpl.More(); aDocExpl.Next()) {
    const XCAFPrs_DocumentNode &aNode = aDocExpl.Current();
    Standard_Integer aDepth = aDocExpl.CurrentDepth();
    TCollection_AsciiString aName =
        Util_OCAF::GetXcafNodeName(aDocExpl.Current(), Standard_False);
    auto nbPopPlusOne = aStack.size() - aDepth;

    for (int i = 1; i < nbPopPlusOne; ++i) {
      aStack.pop();
    }

    if (aDepth == aStack.size() - 1) {
      QTreeWidgetItem *anItem = new QTreeWidgetItem();
      anItem->setText(0, aName.ToCString());
      QTreeWidgetItem *aParent = aStack.top();
      aParent->addChild(anItem);
      aStack.push(anItem);
    }
  }

  aTree->addTopLevelItem(aRoot);
  aTree->expandAll();
}

// vim: set foldmarker={{{,}}} foldmethod=marker foldlevel=0:
