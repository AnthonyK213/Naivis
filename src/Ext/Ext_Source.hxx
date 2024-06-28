#ifndef _Naivis_Ext_Source_HeaderFile
#define _Naivis_Ext_Source_HeaderFile

#include <QObject>

class Ext_Source : QObject {
  Q_OBJECT

public:
  virtual bool Fetch() = 0;

protected:
};

#endif
