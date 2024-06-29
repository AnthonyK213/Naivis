#include "NaiveApp_ExtensionMgr.hxx"
#include "NaiveApp_Extension.hxx"
#include "NaiveApp_LuaMgr.hxx"
#include <Widget/naivis.h>

#include <QDir>
#include <QDirIterator>

NaiveApp_ExtensionMgr::NaiveApp_ExtensionMgr(const QString &theExtDir,
                                             QObject *theParent)
    : QObject(theParent), myExtDir(theExtDir) {
  if (!QDir().exists(myExtDir)) {
    QDir().mkdir(myExtDir);
  }
}

bool NaiveApp_ExtensionMgr::Init() {
  QDirIterator anIter(myExtDir, QDir::Dirs | QDir::NoDotAndDotDot);

  while (anIter.hasNext()) {
    NaiveApp_Extension *anExt = new NaiveApp_Extension(anIter.next(), this);
    myExts.push_back(anExt);
  }

  return true;
}

bool NaiveApp_ExtensionMgr::LoadExts() {
  for (NaiveApp_Extension *ext : myExts) {
    ext->Load();
  }

  return true;
}

NaiveApp_LuaMgr *NaiveApp_ExtensionMgr::LuaMgr() const {
  Naivis *naivis = dynamic_cast<Naivis *>(parent());
  if (!naivis)
    return nullptr;
  return naivis->getLuaMgr();
}

const NaiveApp_Extension *
NaiveApp_ExtensionMgr::Find(const QString &theName) const {
  auto it = std::find_if(myExts.cbegin(), myExts.cend(),
                         [&](const NaiveApp_Extension *const &anExt) {
                           return anExt->Name() == theName;
                         });
  if (it != myExts.cend())
    return *it;
  return nullptr;
}
