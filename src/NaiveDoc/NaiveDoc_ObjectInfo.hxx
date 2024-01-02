#ifndef _Naivis_NaiveDoc_ObjectInfo_HeaderFile
#define _Naivis_NaiveDoc_ObjectInfo_HeaderFile

#include <Standard.hxx>
#include <Standard_Handle.hxx>
#include <Standard_Transient.hxx>
#include <Standard_Type.hxx>

#include <QString>
#include <QUuid>

class NaiveDoc_ObjectInfo;
DEFINE_STANDARD_HANDLE(NaiveDoc_ObjectInfo, Standard_Transient)

class NaiveDoc_ObjectInfo : public Standard_Transient {
public:
  NaiveDoc_ObjectInfo();

  ~NaiveDoc_ObjectInfo();

public:
  const QUuid &Id() const { return myId; }

  const QString &Name() const { return myName; }

  void SetId() { myId = QUuid::createUuid(); }

  void SetName(const QString &theName) { myName = theName; }

  DEFINE_STANDARD_RTTIEXT(NaiveDoc_ObjectInfo, Standard_Transient)

private:
  QUuid myId;
  QString myName;
};

#endif
