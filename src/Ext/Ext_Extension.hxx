#ifndef _Naivis_Ext_Extension_HeaderFile
#define _Naivis_Ext_Extension_HeaderFile

#include <QObject>

#include <list>

class Ext_Extension : QObject {
  Q_OBJECT

public:
  Ext_Extension(QObject *parent = nullptr);

private:
  ::std::list<Ext_Extension *> myDeps;
};

#endif
