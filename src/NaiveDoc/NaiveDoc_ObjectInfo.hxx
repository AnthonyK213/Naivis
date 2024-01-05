#ifndef _Naivis_NaiveDoc_ObjectInfo_HeaderFile
#define _Naivis_NaiveDoc_ObjectInfo_HeaderFile

#include <Standard.hxx>
#include <Standard_Handle.hxx>
#include <Standard_Transient.hxx>
#include <Standard_Type.hxx>

#include <QList>
#include <QString>
#include <QUuid>

using NaiveDoc_Id = QUuid;
using NaiveDoc_IdList = QList<NaiveDoc_Id>;

class NaiveDoc_ObjectInfo;
DEFINE_STANDARD_HANDLE(NaiveDoc_ObjectInfo, Standard_Transient)

class NaiveDoc_ObjectInfo : public Standard_Transient {
public:
  NaiveDoc_ObjectInfo();

  ~NaiveDoc_ObjectInfo();

public:
  const NaiveDoc_Id &Id() const { return myId; }

  void SetId() { myId = NaiveDoc_Id::createUuid(); }

  const QString &Name() const { return myName; }

  void SetName(const QString &theName) { myName = theName; }

  Standard_Boolean HasOwner() const { return !myOwner.IsNull(); }

  const Handle(Standard_Transient) & GetOwner() const { return myOwner; }

  void SetOwner(const Handle(Standard_Transient) & theOwner) {
    myOwner = theOwner;
  }

  Standard_Boolean IsDeleted() const { return myIsDeleted; }

  void SetDeleted(const Standard_Boolean theDeleted) {
    myIsDeleted = theDeleted;
  }

  Standard_Boolean IsHidden() const { return myIsHidden; }

  void SetHidden(const Standard_Boolean theHidden) { myIsHidden = theHidden; }

  DEFINE_STANDARD_RTTIEXT(NaiveDoc_ObjectInfo, Standard_Transient)

private:
  NaiveDoc_Id myId;
  QString myName;
  Handle(Standard_Transient) myOwner;
  Standard_Boolean myIsDeleted;
  Standard_Boolean myIsHidden;
};

#endif
