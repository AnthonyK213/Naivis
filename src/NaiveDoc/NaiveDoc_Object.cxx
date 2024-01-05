#include "NaiveDoc_Object.hxx"

static Handle(NaiveDoc_ObjectInfo)
    getObjectInfoConst(const NaiveDoc_Object &theObject) {
  if (!theObject.HasOwner()) {
    return nullptr;
  }

  return Handle(NaiveDoc_ObjectInfo)::DownCast(theObject.GetOwner());
}

static Handle(NaiveDoc_ObjectInfo) getObjectInfo(NaiveDoc_Object &theObject) {
  Handle(NaiveDoc_ObjectInfo) anInfo;

  if (!theObject.HasOwner()) {
    anInfo = new NaiveDoc_ObjectInfo;
    theObject.SetOwner(anInfo);
    return anInfo;
  }

  anInfo = Handle(NaiveDoc_ObjectInfo)::DownCast(theObject.GetOwner());

  if (!anInfo.IsNull()) {
    return anInfo;
  }

  anInfo = new NaiveDoc_ObjectInfo;
  anInfo->SetOwner(theObject.GetOwner());
  theObject.SetOwner(anInfo);

  return anInfo;
}

NaiveDoc_Id NaiveDoc_Object_GetId(const NaiveDoc_Object &theObject) {
  Handle(NaiveDoc_ObjectInfo) anInfo = getObjectInfoConst(theObject);

  if (anInfo.IsNull()) {
    return NaiveDoc_Id();
  }

  return anInfo->Id();
}

void NaiveDoc_Object_SetId(NaiveDoc_Object &theObject) {
  getObjectInfo(theObject)->SetId();
}

QString NaiveDoc_Object_GetName(const NaiveDoc_Object &theObject) {
  Handle(NaiveDoc_ObjectInfo) anInfo = getObjectInfoConst(theObject);

  if (anInfo.IsNull()) {
    return QString();
  }

  return anInfo->Name();
}

void NaiveDoc_Object_SetName(NaiveDoc_Object &theObject,
                             const QString &theName) {
  getObjectInfo(theObject)->SetName(theName);
}

Standard_Boolean NaiveDoc_Object_IsDeleted(const NaiveDoc_Object &theObject) {
  Handle(NaiveDoc_ObjectInfo) anInfo = getObjectInfoConst(theObject);

  if (anInfo.IsNull()) {
    return Standard_False;
  }

  return anInfo->IsDeleted();
}

void NaiveDoc_Object_SetDeleted(NaiveDoc_Object &theObject,
                                const Standard_Boolean theDeleted) {
  getObjectInfo(theObject)->SetDeleted(theDeleted);
}

Standard_Boolean NaiveDoc_Object_IsHidden(const NaiveDoc_Object &theObject) {
  Handle(NaiveDoc_ObjectInfo) anInfo = getObjectInfoConst(theObject);

  if (anInfo.IsNull()) {
    return Standard_False;
  }

  return anInfo->IsHidden();
}

void NaiveDoc_Object_SetHidden(NaiveDoc_Object &theObject,
                               const Standard_Boolean theHidden) {
  getObjectInfo(theObject)->SetHidden(theHidden);
}
