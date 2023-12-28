#ifndef _Naivis_NaiveDoc_Object_HeaderFile
#define _Naivis_NaiveDoc_Object_HeaderFile

#include <AIS_InteractiveObject.hxx>
#include <Standard.hxx>
#include <Standard_Handle.hxx>
#include <Standard_Type.hxx>

#include <QList>
#include <QString>
#include <QUuid>

class NaiveDoc_Object;
DEFINE_STANDARD_HANDLE(NaiveDoc_Object, Standard_Transient)

class NaiveDoc_Object : public Standard_Transient {
public:
  NaiveDoc_Object(const Handle(AIS_InteractiveObject) & theObject);

  ~NaiveDoc_Object();

public:
  const Handle(AIS_InteractiveObject) & Object() const;

  const QUuid &Id() const;

  void SetId(const QUuid &theId);

  void SetId();

  const QString &Name() const;

  void SetName(const QString &theName);

  DEFINE_STANDARD_RTTIEXT(NaiveDoc_Object, Standard_Transient)

protected:
private:
  Handle(AIS_InteractiveObject) myObject;
  QString myName;
  QUuid myUuid;
};

using NaiveDoc_ObjectList = QList<Handle(NaiveDoc_Object)>;

#endif
