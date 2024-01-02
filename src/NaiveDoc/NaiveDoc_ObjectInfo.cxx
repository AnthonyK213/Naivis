#include "NaiveDoc_ObjectInfo.hxx"

IMPLEMENT_STANDARD_RTTIEXT(NaiveDoc_ObjectInfo, Standard_Transient)

NaiveDoc_ObjectInfo::NaiveDoc_ObjectInfo()
    : myId(), myName(), myOwner(nullptr) {}

NaiveDoc_ObjectInfo::~NaiveDoc_ObjectInfo() {}
