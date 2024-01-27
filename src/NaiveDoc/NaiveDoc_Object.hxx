#ifndef _Naivis_NaiveDoc_Object_HeaderFile
#define _Naivis_NaiveDoc_Object_HeaderFile

#include <AIS_InteractiveObject.hxx>
#include <QList>

#include "NaiveDoc_ObjectInfo.hxx"

using NaiveDoc_Object = AIS_InteractiveObject;
using NaiveDoc_ObjectList = QList<Handle(NaiveDoc_Object)>;

NaiveDoc_Id NaiveDoc_Object_GetId(const NaiveDoc_Object &theObject);

void NaiveDoc_Object_SetId(NaiveDoc_Object &theObject);

QString NaiveDoc_Object_GetName(const NaiveDoc_Object &theObject);

void NaiveDoc_Object_SetName(NaiveDoc_Object &theObject,
                             const QString &theName);

Standard_Boolean NaiveDoc_Object_IsDeleted(const NaiveDoc_Object &theObject);

void NaiveDoc_Object_SetDeleted(NaiveDoc_Object &theObject,
                                const Standard_Boolean theDeleted);

Standard_Boolean NaiveDoc_Object_IsHidden(const NaiveDoc_Object &theHidden);

void NaiveDoc_Object_SetHidden(NaiveDoc_Object &theObject,
                               const Standard_Boolean theHidden);

#endif
