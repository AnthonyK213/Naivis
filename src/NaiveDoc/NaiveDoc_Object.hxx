#ifndef _Naivis_NaiveDoc_Object_HeaderFile
#define _Naivis_NaiveDoc_Object_HeaderFile

#include <AIS_InteractiveObject.hxx>
#include <vector>

#include "NaiveDoc_ObjectInfo.hxx"

using NaiveDoc_Object = AIS_InteractiveObject;
using NaiveDoc_ObjectList = std::vector<Handle(NaiveDoc_Object)>;

QUuid NaiveDoc_Object_GetId(const NaiveDoc_Object &theObject);

void NaiveDoc_Object_SetId(NaiveDoc_Object &theObject);

QString NaiveDoc_Object_GetName(const NaiveDoc_Object &theObject);

void NaiveDoc_Object_SetName(NaiveDoc_Object &theObject,
                             const QString &theName);

#endif
