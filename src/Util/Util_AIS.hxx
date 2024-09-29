#ifndef _Naivis_Util_AIS_HeaderFile
#define _Naivis_Util_AIS_HeaderFile

#include <AIS_InteractiveContext.hxx>
#include <AIS_InteractiveObject.hxx>

#include <vector>

#include <QHash>
#include <QList>

namespace Util_AIS {

QStringList GetObjectProperties(const Handle(AIS_InteractiveObject) & theObj,
                                const QVariantHash &theExtra = {});

std::vector<Handle(AIS_InteractiveObject)>
GetSelections(const Handle(AIS_InteractiveContext) & theCtx);

} /* namespace Util_AIS */

#endif
