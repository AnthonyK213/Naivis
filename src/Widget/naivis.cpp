#include "naivis.h"
#include "./ui_naivis.h"

#include <QFont>
#include <QFontDatabase>
#include <QIcon>
#include <QStack>
#include <QStyle>

#include <Ext/Ext_Load.hxx>
#include <NaiveApp/NaiveApp_Application.hxx>
#include <Util/Util_AIS.hxx>
#include <Util/Util_Mesh.hxx>

#include <luaocct/luaocct.h>
#include <luaocct/LOUtil_OCAF.hxx>

class Naivis::LuaManager {
public:
  LuaManager() : myL(luaL_newstate()) {
    luaL_openlibs(myL);
    luaopen_luaocct(myL);
    Ext_Load(myL);

    auto rtp = QCoreApplication::applicationDirPath().toUtf8().toStdString();
    pathAppend(rtp);
    cpathAppend(rtp);
  }

  ~LuaManager() { lua_close(myL); }

  operator lua_State *() { return myL; }

  lua_State *L() const { return myL; }

  const QString &file() const { return myFile; }

  bool setFile(const QString &theFile) {
    myFile = theFile;

    /// WORKAROUND: Add the file directory to the runtime path.
    QFileInfo info(myFile);
    auto rtp = info.dir().absolutePath().toUtf8().toStdString();
    pathAppend(rtp);
    cpathAppend(rtp);

    return true;
  }

  bool doString(const std::string &theCode, std::string &theErr) {
    if (luaL_dostring(myL, theCode.c_str()) != 0) {
      theErr = lua_tostring(myL, -1);
      lua_pop(myL, -1);
      return false;
    }

    return true;
  }

  bool doFile(std::string &theErr) const {
    if (luaL_dofile(myL, myFile.toUtf8().toStdString().c_str()) != 0) {
      theErr = lua_tostring(myL, -1);
      lua_pop(myL, -1);
      return false;
    }

    return true;
  }

private:
  bool pathAppend(const std::string &thePath, bool theToComplete = true) const {
    lua_getglobal(myL, "package");
    lua_getfield(myL, -1, "path");
    std::string path = lua_tostring(myL, -1);
    if (!path.empty())
      path.append(";");
    path.append(thePath);
    if (theToComplete)
      path.append("/?.lua");
    lua_pop(myL, 1);
    lua_pushstring(myL, path.c_str());
    lua_setfield(myL, -2, "path");
    lua_pop(myL, 1);

    return true;
  }

  bool cpathAppend(const std::string &thePath,
                   bool theToComplete = true) const {
    lua_getglobal(myL, "package");
    lua_getfield(myL, -1, "cpath");
    std::string path = lua_tostring(myL, -1);
    if (!path.empty())
      path.append(";");
    path.append(thePath);
    if (theToComplete) {
#ifdef _WIN32
      path.append("/?.dll");
#else
      path.append("/?.so");
#endif
    }
    lua_pop(myL, 1);
    lua_pushstring(myL, path.c_str());
    lua_setfield(myL, -2, "cpath");
    lua_pop(myL, 1);

    return true;
  }

private:
  lua_State *myL;
  QString myFile;
};

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

  NaiveApp_InitPrsDrivers();

  ui->actionOrthographic->trigger();
}

Naivis::~Naivis() {
  delete myLuaMgr;
  delete mySettings;
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
    myLuaMgr->setFile(filePath);
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
  std::string anErr;
  if (!myLuaMgr->doFile(anErr)) {
    std::cout << anErr << '\n';
  }
}

// }}}

void Naivis::setupSettings() {
  QString aPath =
      QCoreApplication::applicationDirPath() + "/NaivisSettings.ini";
  mySettings = new NaiveApp_Settings(aPath);
}

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
  QVariant vFont = mySettings->Value("script_editor", "font");
  QString fontName =
      !vFont.isNull() && vFont.isValid() ? vFont.toString() : "monospace";

  QVariant vFontSize = mySettings->Value("script_editor", "font_size");
  int fontSize =
      !vFontSize.isNull() && vFontSize.isValid() ? vFontSize.toInt() : 10;
  fontSize = std::min(std::max(fontSize, 3), 72);

  QFont f = QFontDatabase::font(fontName, "", fontSize);
}

void Naivis::setupLua() {
  myLuaMgr = new LuaManager;

  LuaBridge__G(myLuaMgr->L())
      .Begin_Namespace(Naivis)

      .Begin_Namespace(NaiveApp)
      .addProperty("Settings", [this]() { return settings(); })
      .addFunction("Clear", [this]() { clearOutputBuffer(); })
      .End_Namespace()

      .Begin_Namespace(NaiveDoc)
      .addProperty("ActiveDoc", [this]() { return document(); })
      .End_Namespace()

      .End_Namespace();

  /* Import third-party lua libraries.
   * [luv](https://github.com/luvit/luv)
   * [wxLua](https://github.com/pkulchenko/wxlua)
   * - Crash on |lua_close|,
   *   issue[#20](https://github.com/pkulchenko/wxlua/issues/20)
   */

  std::string anErr;
  std::string aCode = R"===(
  _G.uv=require("luv")
  --_G.wx=require("wx")
  )===";
  if (!myLuaMgr->doString(aCode, anErr)) {
    std::cout << anErr << '\n';
  }
}

Widget_OcctViewer *Naivis::occtViewer() { return ui->occtViewer; }

const Handle(NaiveDoc_Document) & Naivis::document() const {
  return ui->occtViewer->Document();
}

NaiveApp_Settings *Naivis::settings() const { return mySettings; }

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
