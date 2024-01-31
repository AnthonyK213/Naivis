#ifndef _Naivis_Util_AIS_HeaderFile
#define _Naivis_Util_AIS_HeaderFile

#include <QList>

#include <AIS_InteractiveContext.hxx>
#include <AIS_InteractiveObject.hxx>

namespace Util_AIS {

QStringList GetObjectProperties(const Handle(AIS_InteractiveObject) & theObj);

QList<Handle(AIS_InteractiveObject)>
GetSelections(const Handle(AIS_InteractiveContext) & theCtx);

} // namespace Util_AIS

#endif