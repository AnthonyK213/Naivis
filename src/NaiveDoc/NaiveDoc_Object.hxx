#ifndef _Naivis_NaiveDoc_Object_HeaderFile
#define _Naivis_NaiveDoc_Object_HeaderFile

#include <AIS_InteractiveObject.hxx>
#include <TDF_Label.hxx>

using NaiveDoc_Object = AIS_InteractiveObject;
using NaiveDoc_ObjectList = std::vector<Handle(NaiveDoc_Object)>;
using NaiveDoc_Id = TDF_Label;
using NaiveDoc_IdList = std::vector<TDF_Label>;

#endif
