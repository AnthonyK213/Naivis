#include "NaiveDoc_Attribute.hxx"

#include <XCAFPrs_AISObject.hxx>

IMPLEMENT_STANDARD_RTTIEXT(NaiveDoc_Attribute, Standard_Transient)

NaiveDoc_Attribute::NaiveDoc_Attribute() : myId() {}

NaiveDoc_Attribute::~NaiveDoc_Attribute() {
#ifndef NDEBUG
  std::cout << "NaiveDoc_Attribute dtor: " << myId << '\n';
#endif
}

static Handle(NaiveDoc_Attribute)
    findAttr(const Handle(NaiveDoc_Object) & theObj,
             const Standard_Boolean theToInit = Standard_False) {
  if (theObj.IsNull())
    return nullptr;

  Handle(NaiveDoc_Attribute) anAttr;

  if (theObj->HasOwner()) {
    anAttr = Handle(NaiveDoc_Attribute)::DownCast(theObj->GetOwner());
    if (!anAttr.IsNull())
      return anAttr;
  }

  if (theToInit) {
    anAttr = new NaiveDoc_Attribute;
    theObj->SetOwner(anAttr);
  }

  return anAttr;
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

Standard_Boolean NaiveDoc_Attribute::SetId(const Handle(NaiveDoc_Object) &
                                               theObj,
                                           const NaiveDoc_Id &theId) {
  Handle(NaiveDoc_Attribute) anAttr = findAttr(theObj, Standard_True);
  if (anAttr.IsNull())
    Standard_False;

  anAttr->setId(theId);
  return Standard_True;
}

Handle(TPrsStd_AISPresentation)
    NaiveDoc_Attribute::GetPrs(const NaiveDoc_Id &theId) {
  if (theId.IsNull())
    return nullptr;

  Handle(TPrsStd_AISPresentation) aPrs;
  if (!theId.FindAttribute(TPrsStd_AISPresentation::GetID(), aPrs))
    return nullptr;

  return aPrs;
}

Handle(TPrsStd_AISPresentation)
    NaiveDoc_Attribute::GetPrs(const Handle(NaiveDoc_Object) & theObj) {
  return GetPrs(GetId(theObj));
}
