#ifndef _Naivis_Ghost_Object_HeaderFile
#define _Naivis_Ghost_Object_HeaderFile

#include <AIS_InteractiveObject.hxx>

#include <QMap>
#include <QUuid>

using Ghost_Id = QUuid;
using Ghost_Object = AIS_InteractiveObject;
using Ghost_ObjectMap = QMap<Ghost_Id, Handle(Ghost_Object)>;

#endif
