#ifndef _Naivis_NaiveApp_ExtensionMgr_HeaderFile
#define _Naivis_NaiveApp_ExtensionMgr_HeaderFile

#include <QObject>
#include <QString>

class NaiveApp_Extension;
class NaiveApp_LuaMgr;

class NaiveApp_ExtensionMgr : public QObject {
  Q_OBJECT

public:
  NaiveApp_ExtensionMgr(const QString &theExtDir, QObject *theParent = nullptr);

  bool Init();

  bool LoadExts();

  NaiveApp_LuaMgr *LuaMgr() const;

  const NaiveApp_Extension *Find(const QString &theName) const;

private:
  QString myExtDir;
  ::std::list<NaiveApp_Extension *> myExts;
};

#endif
