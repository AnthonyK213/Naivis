#include "naivis.h"
#include "./ui_naivis.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QFont>
#include <QFontDatabase>
#include <QIcon>
#include <QStack>
#include <QStandardPaths>
#include <QString>
#include <QStyle>

#include <Ghost/Ghost_Document.hxx>
#include <NaiveApp/NaiveApp_Application.hxx>
#include <NaiveApp/NaiveApp_ExtensionMgr.hxx>
#include <NaiveApp/NaiveApp_LogStream.hxx>
#include <NaiveApp/NaiveApp_LuaMgr.hxx>
#include <NaiveApp/NaiveApp_Settings.hxx>
#include <Util/Util_AIS.hxx>
#include <Util/Util_Mesh.hxx>
#include <Widget/Widget_OcctViewer.hxx>

#include <luaocct/LOUtil_OCAF.hxx>

#include <Ext/Ext_Bind.hxx>
#include <Ext/Ext_Load.hxx>

#include <lua.hpp>

static QString getExeDir(const QString &relativePath = QString()) {
  QString p = QCoreApplication::applicationDirPath();
  if (relativePath.isNull() || relativePath.isEmpty())
    return p;
  return p + '/' + relativePath;
}

static QString getLuaDir(const QString &relativePath = QString()) {
  QString p = QCoreApplication::applicationDirPath() + "/runtime/lua/";
  if (relativePath.isNull() || relativePath.isEmpty())
    return p;
  return p + '/' + relativePath;
}

static QString getDataDir(const QString &relativePath = QString()) {
  QString p =
      QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);

  if (relativePath.isNull() || relativePath.isEmpty())
    return p;

  return p + '/' + relativePath;
}

Naivis::Naivis(QWidget *parent) : QMainWindow(parent), ui(new Ui::Naivis) {
  ui->setupUi(this);

  QApplication::setWindowIcon(QIcon(":icons/Naivis.ico"));

  setupSettings();
  setupActions();
  setupActionIcons();
  setupOutputBuffer();
  setupScriptEditor();
  setupAssemblyTree();
  setupSelectionPropertiesTable();
  setupLua();
  setupExtension();

  NaiveApp_InitPrsDrivers();

  ui->actionOrthographic->trigger();
}

Naivis::~Naivis() {
  delete myLogStream;
  delete ui;
}

NaiveApp_LuaMgr *Naivis::getLuaMgr() const { return myLuaMgr; }

/* SLOTS {{{ */

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
    myLuaMgr->SetFile(filePath);
  }
}

void Naivis::saveScript() {
  // QString filePath =
  //     QFileDialog::getSaveFileName(this, "Save", "", tr("Lua (*.lua)"));

  // if (filePath.isEmpty())
  //   return;

  // if (!filePath.endsWith(".lua", Qt::CaseInsensitive))
  //   filePath += ".lua";

  // QFile file{filePath};
  // file.open(QFile::ReadWrite | QIODevice::Truncate | QIODevice::Text);
}

void Naivis::quit() { close(); }

void Naivis::transform() {}

void Naivis::undo() {
  document()->Undo();
  /* FIXME: How to excute this when needed? */
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
  std::string anErr;
  if (!myLuaMgr->DoFile(anErr)) {
    std::cout << anErr << '\n';
  }
}

/* }}} */

void Naivis::setupSettings() {
  QString aPath = getDataDir("settings.ini");
  mySettings = new NaiveApp_Settings(aPath, this);
}

#define CONNECT_ACTION(A, F) connect(A, &QAction::triggered, this, &Naivis::F);

void Naivis::setupActions() {
  /* File */
  CONNECT_ACTION(ui->actionImport, importFile);
  CONNECT_ACTION(ui->actionExport, exportFile);
  CONNECT_ACTION(ui->actionOpenScript, openScript);
  CONNECT_ACTION(ui->actionSaveScript, saveScript);
  CONNECT_ACTION(ui->actionQuit, quit);

  /* Edit */
  CONNECT_ACTION(ui->actionUndo, undo);
  CONNECT_ACTION(ui->actionRedo, redo);
  CONNECT_ACTION(ui->actionTransform, transform);
  CONNECT_ACTION(ui->actionSelectAll, selectAll);
  CONNECT_ACTION(ui->actionHide, hideCurrentSelection);
  CONNECT_ACTION(ui->actionShowAll, showAll);
  CONNECT_ACTION(ui->actionDelete, deleteCurrentSelection);

  /* View */
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

  /* Script */
  CONNECT_ACTION(ui->actionRunScript, runScript);

  /* Help */
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
  myLogStream = new NaiveApp_LogStream(std::cout, ui->outputBuffer);
}

void Naivis::setupScriptEditor() {
  QVariant vFont = mySettings->Value("script_editor", "font");
  QString fontName =
      !vFont.isNull() && vFont.isValid() ? vFont.toString() : "monospace";

  QVariant vFontSize = mySettings->Value("script_editor", "font_size");
  int fontSize =
      !vFontSize.isNull() && vFontSize.isValid() ? vFontSize.toInt() : 10;
  fontSize = std::min(std::max(fontSize, 3), 72);

  QFont f = QFontDatabase::font(fontName, "", fontSize);
}

bool Naivis::setupLua() {
  myLuaMgr = new NaiveApp_LuaMgr(this);

  if (!myLuaMgr->Init())
    return false;

  /* Define alias and import third-party lua libraries.
   * [luv](https://github.com/luvit/luv)
   * [wxLua](https://github.com/pkulchenko/wxlua)
   * - Crash on |lua_close|,
   *   issue[#20](https://github.com/pkulchenko/wxlua/issues/20)
   */

  std::string anErr;
  if (!myLuaMgr->DoFile(getLuaDir("__init__.lua").toUtf8().toStdString(),
                        anErr)) {
    std::cout << anErr << '\n';
  }

  LuaBridge__G(myLuaMgr->L())
      .Begin_Namespace(Naivis)

      .Begin_Namespace(NaiveApp)
      .addProperty("Settings", [this]() { return settings(); })
      .addProperty("ExtensionMgr", [this]() { return extensionMgr(); })
      .addFunction("Clear", [this]() { clearOutputBuffer(); })
      .addFunction("CurrentScriptDir",
                   [this]() -> QString {
                     const QString &aFile = getLuaMgr()->File();
                     if (aFile.isNull())
                       return "";
                     QFileInfo anInfo(aFile);
                     return anInfo.dir().absolutePath();
                   })
      .End_Namespace()

      .Begin_Namespace(NaiveDoc)
      .addProperty("ActiveDoc", [this]() { return document(); })
      .End_Namespace()

      .Begin_Namespace(Ghost)
      .addFunction("NewDocument",
                   [this]() -> Handle(Ghost_Document) {
                     return new Ghost_Document(occtViewer()->Context());
                   })
      .End_Namespace()

      .End_Namespace();

  return true;
}

bool Naivis::setupExtension() {
  myExtMgr = new NaiveApp_ExtensionMgr(getDataDir("extensions"), this);
  if (!myExtMgr->Init())
    return false;

  if (!myExtMgr->LoadExts())
    return false;

  return true;
}

Widget_OcctViewer *Naivis::occtViewer() { return ui->occtViewer; }

const Handle(NaiveDoc_Document) & Naivis::document() const {
  return ui->occtViewer->Document();
}

NaiveApp_Settings *Naivis::settings() const { return mySettings; }

NaiveApp_ExtensionMgr *Naivis::extensionMgr() const { return myExtMgr; }

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
    QStringList aProps;

    QVariant aExtraAttributes =
        mySettings->Value("document", "extra_attributes");
    if (!aExtraAttributes.isNull() && aExtraAttributes.isValid()) {
      QVariantHash aExtra = aExtraAttributes.toHash();
      aProps = Util_AIS::GetObjectProperties(anObj, aExtra);
    } else {
      aProps = Util_AIS::GetObjectProperties(anObj);
    }

    auto nbRow = aProps.size() >> 1;
    tbl->setRowCount(nbRow);
    QStringList header;
    header << "Property" << "Value";
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

  for (auto aDocExpl = theDoc->DocumentExplorer(0); aDocExpl->More();
       aDocExpl->Next()) {
    const NaiveDoc_DocumentNode &aNode = aDocExpl->Current();
    Standard_Integer aDepth = aDocExpl->CurrentDepth();
    TCollection_AsciiString aName =
        LOUtil_OCAF::GetXcafNodeName(aDocExpl->Current(), Standard_False);
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

void Naivis::clearOutputBuffer() { ui->outputBuffer->clear(); }

// vim: set foldmarker={{{,}}} foldmethod=marker foldlevel=0:
