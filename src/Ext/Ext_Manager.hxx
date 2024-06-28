#ifndef _Naivis_Ext_Manager_HeaderFile
#define _Naivis_Ext_Manager_HeaderFile

#include <QObject>
#include <QString>

class Ext_Extension;

class Ext_Manager : QObject {
  Q_OBJECT

public:
  Ext_Manager(QObject *parent = nullptr);

  bool LoadExts();

private:
  QString myPkgDir;
};

#endif
