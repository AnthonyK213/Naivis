#include "NaiveDoc_Attribute.hxx"

#include <XCAFPrs_AISObject.hxx>

NaiveDoc_Id NaiveDoc_Attribute::GetId(const Handle(NaiveDoc_Object) & theObj) {
  Handle(TPrsStd_AISPresentation) aPrs = GetPrs(theObj);
  if (aPrs.IsNull())
    return {};

  return aPrs->Label();
}

Handle(TPrsStd_AISPresentation)
    NaiveDoc_Attribute::GetPrs(const NaiveDoc_Id &theId) {
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
