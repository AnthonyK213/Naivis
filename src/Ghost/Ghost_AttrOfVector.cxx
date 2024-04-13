#include "Ghost_AttrOfVector.hxx"

#include <naivecgl/Math/Util.h>

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
  if (theData > Precision::Confusion() &&
      naivecgl::math::Util::IsValidDouble(theData))
    myData[theIndex] = theData;
}
