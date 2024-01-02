#include "NaiveDoc_Object.hxx"

static Handle(NaiveDoc_ObjectInfo)
    getObjectInfo(const NaiveDoc_Object &theObject) {
  if (!theObject.HasOwner()) {
    return nullptr;
  }

  return Handle(NaiveDoc_ObjectInfo)::DownCast(theObject.GetOwner());
}

QUuid NaiveDoc_Object_GetId(const NaiveDoc_Object &theObject) {
  Handle(NaiveDoc_ObjectInfo) anInfo = getObjectInfo(theObject);

  if (anInfo.IsNull()) {
    return QUuid();
  }

  return anInfo->Id();
}

QString NaiveDoc_Object_GetName(const NaiveDoc_Object &theObject) {
  Handle(NaiveDoc_ObjectInfo) anInfo = getObjectInfo(theObject);

  if (anInfo.IsNull()) {
    return QString();
  }

  return anInfo->Name();
}

void NaiveDoc_Object_SetId(NaiveDoc_Object &theObject) {
  Handle(NaiveDoc_ObjectInfo) anInfo = getObjectInfo(theObject);

  if (anInfo.IsNull()) {
    anInfo = new NaiveDoc_ObjectInfo();
    theObject.SetOwner(anInfo);
  }

  anInfo->SetId();
}

void NaiveDoc_Object_SetName(NaiveDoc_Object &theObject,
                             const QString &theName) {
  Handle(NaiveDoc_ObjectInfo) anInfo = getObjectInfo(theObject);

  if (anInfo.IsNull()) {
    anInfo = new NaiveDoc_ObjectInfo();
    theObject.SetOwner(anInfo);
  }

  anInfo->SetName(theName);
}
