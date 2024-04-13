#include "Ghost_Attribute.hxx"

IMPLEMENT_STANDARD_RTTIEXT(Ghost_Attribute, LODoc_Attribute)

Naivis_GHOSTATTR_IMPLEMENT_GETSET(Ghost_Attribute);

Ghost_Attribute::Ghost_Attribute()
    : LODoc_Attribute(), myType(Ghost_OTNone), myId() {}
