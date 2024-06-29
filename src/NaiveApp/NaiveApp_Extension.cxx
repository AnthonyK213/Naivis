#include "NaiveApp_Extension.hxx"
#include "NaiveApp_ExtensionMgr.hxx"
#include "NaiveApp_LuaMgr.hxx"

#include <QFileInfo>

#include <iostream>

NaiveApp_Extension::NaiveApp_Extension(const QString &theDir,
                                       QObject *theParent)
    : QObject(theParent), myDir(theDir), myType(NaiveApp_ExtensionType_None),
      myIsLoaded(false) {
  init();
}

void NaiveApp_Extension::Load() {
  if (IsLoaded())
    return;

  if (!IsValid())
    return;

  NaiveApp_ExtensionMgr *M = mgr();
  if (!M)
    return;

  NaiveApp_LuaMgr *L = M->LuaMgr();
  std::string aDir = myDir.absoluteFilePath("lua").toUtf8().toStdString();
  L->PathAppend(aDir);
  L->CPathAppend(aDir);

  if (myType == NaiveApp_ExtensionType_Executable) {
    std::string E;
    if (!L->DoFile(aDir + "/init.lua", E)) {
      std::cout << "Failed to load extension at "
                << myDir.absolutePath().toUtf8().toStdString() << '\n';
      return;
    }
  }

  myIsLoaded = true;
}

QString NaiveApp_Extension::Path() const { return myDir.absolutePath(); }

QString NaiveApp_Extension::Name() const { return myDir.dirName(); }

void NaiveApp_Extension::init() {
  QFileInfo aLuaDirInfo(myDir.absoluteFilePath("lua"));
  if (!aLuaDirInfo.exists() || !aLuaDirInfo.isDir())
    return;

  QFileInfo aInitFileInfo(myDir.absoluteFilePath("lua/init.lua"));
  if (aInitFileInfo.exists() && aInitFileInfo.isFile()) {
    myType = NaiveApp_ExtensionType_Executable;
  } else {
    myType = NaiveApp_ExtensionType_Library;
  }
}

NaiveApp_ExtensionMgr *NaiveApp_Extension::mgr() const {
  NaiveApp_ExtensionMgr *M = dynamic_cast<NaiveApp_ExtensionMgr *>(parent());
  return M;
}
