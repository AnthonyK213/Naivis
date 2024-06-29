#ifndef _Naivis_NaiveApp_Extension_HeaderFile
#define _Naivis_NaiveApp_Extension_HeaderFile

#include <QDir>
#include <QObject>

#include <list>

class NaiveApp_ExtensionMgr;

class NaiveApp_Extension : public QObject {
  Q_OBJECT

public:
  NaiveApp_Extension(const QString &theDir, QObject *theParent = nullptr);

  bool IsValid() const { return myType != NaiveApp_ExtensionType_None; }

  bool IsLoaded() const { return myIsLoaded; }

  void Load();
  
  QString Path() const;

  QString Name() const;

  enum NaiveApp_ExtensionType {
    NaiveApp_ExtensionType_None = 0,
    NaiveApp_ExtensionType_Executable,
    NaiveApp_ExtensionType_Library,
  };

private:
  void init();

  NaiveApp_ExtensionMgr *mgr() const;

private:
  ::std::list<NaiveApp_Extension *> myDeps;
  QDir myDir;
  NaiveApp_ExtensionType myType;
  bool myIsLoaded;
};

#endif
