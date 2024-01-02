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

QUuid NaiveDoc_Object_GetId(const NaiveDoc_Object &theObject) {
  Handle(NaiveDoc_ObjectInfo) anInfo = getObjectInfoConst(theObject);

  if (anInfo.IsNull()) {
    return QUuid();
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
