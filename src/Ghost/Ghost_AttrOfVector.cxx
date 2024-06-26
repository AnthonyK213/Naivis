#include "Ghost_AttrOfVector.hxx"

IMPLEMENT_STANDARD_RTTIEXT(Ghost_AttrOfVector, Ghost_Attribute)

Naivis_GHOSTATTR_IMPLEMENT_GETSET(Ghost_AttrOfVector);

Ghost_AttrOfVector::Ghost_AttrOfVector() : Ghost_Attribute() {
  setType(Ghost_OTVector);
  SetBodyR(.1);
  SetTipR(1.);
  SetTipL(4.2);
}

void Ghost_AttrOfVector::setData(const Standard_Integer theIndex,
                                 const Standard_Real theData) {
  if (theData > Precision::Confusion() && !::std::isinf(theData) &&
      !::std::isnan(theData))
    myData[theIndex] = theData;
}
