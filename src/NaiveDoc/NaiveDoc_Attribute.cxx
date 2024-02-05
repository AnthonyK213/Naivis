#include "NaiveDoc_Attribute.hxx"

#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFPrs_AISObject.hxx>

#include <Util/Util_OCAF.hxx>

NaiveDoc_Id NaiveDoc_Attribute::GetId(const Handle(NaiveDoc_Object) & theObj) {
  Handle(TPrsStd_AISPresentation) aPrs = GetPrs(theObj);
  if (aPrs.IsNull())
    return {};

  return aPrs->Label();
}

TCollection_AsciiString
NaiveDoc_Attribute::GetName(const Handle(NaiveDoc_Object) & theObj,
                            Standard_Boolean theIsInstanceName) {
  NaiveDoc_Id anId = GetId(theObj);
  if (anId.IsNull())
    return {};

  Handle(TDataStd_Name) aNodeName;
  TDF_Label aRef = anId;

  if (theIsInstanceName)
    XCAFDoc_ShapeTool::GetReferredShape(anId, aRef);

  if (anId.FindAttribute(TDataStd_Name::GetID(), aNodeName)) {
    return aNodeName->Get();
  }

  return {};
}

Handle(TPrsStd_AISPresentation)
    NaiveDoc_Attribute::GetPrs(const NaiveDoc_Id &theId) {
  if (theId.IsNull())
    return nullptr;

  Handle(TPrsStd_AISPresentation) aPrs;
  theId.FindAttribute(TPrsStd_AISPresentation::GetID(), aPrs);
  return aPrs;
}

Handle(TPrsStd_AISPresentation)
    NaiveDoc_Attribute::GetPrs(const Handle(NaiveDoc_Object) & theObj) {
  if (theObj.IsNull() || !theObj->HasOwner())
    return nullptr;

  return Handle(TPrsStd_AISPresentation)::DownCast(theObj->GetOwner());
}

TopoDS_Shape NaiveDoc_Attribute::GetShape(const Handle(NaiveDoc_Object) &
                                              theObj,
                                          TopLoc_Location &theLoc) {
  Handle(AIS_Shape) anObj = Handle(AIS_Shape)::DownCast(theObj);
  if (anObj.IsNull())
    return TopoDS_Shape();

  theLoc = anObj->Transformation();
  return anObj->Shape();
}
