#include "NaiveDoc_Attribute.hxx"

#include <XCAFPrs_AISObject.hxx>

IMPLEMENT_STANDARD_RTTIEXT(NaiveDoc_Attribute, Standard_Transient)

NaiveDoc_Attribute::NaiveDoc_Attribute() {}

NaiveDoc_Attribute::~NaiveDoc_Attribute() {
#ifndef NDEBUG
  std::cout << "NaiveDoc_Attribute dtor: " << '\n';
#endif
}

NaiveDoc_Id NaiveDoc_Attribute::GetId(const Handle(NaiveDoc_Object) & theObj) {
  Handle(NaiveDoc_Attribute) anAttr = findAttr(theObj);
  if (!anAttr.IsNull())
    return anAttr->getId();

  if (theObj->IsKind(STANDARD_TYPE(XCAFPrs_AISObject))) {
    auto anObj = Handle(XCAFPrs_AISObject)::DownCast(theObj);
    return anObj->GetLabel();
  }

  return {};
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
