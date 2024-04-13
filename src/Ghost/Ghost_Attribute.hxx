#ifndef _Naivis_Ghost_Attribute_HeaderFile
#define _Naivis_Ghost_Attribute_HeaderFile

#include "Ghost_Object.hxx"
#include "Ghost_ObjectType.hxx"

#include <luaocct/LODoc_Attribute.hxx>

#define Naivis_GHOSTATTR_DEFINE_GETSET(T)                                      \
public:                                                                        \
  static Handle(T) Get(const Handle(Ghost_Object) & theObj);                   \
  static Handle(T)                                                             \
      Set(const Handle(Ghost_Object) & theObj, const Handle(T) & theAttr);

#define Naivis_GHOSTATTR_IMPLEMENT_GETSET(T)                                   \
  Handle(T) T::Get(const Handle(Ghost_Object) & theObj) {                      \
    if (!theObj || !theObj->HasOwner())                                        \
      return nullptr;                                                          \
    return Handle(T)::DownCast(theObj->GetOwner());                            \
  }                                                                            \
  Handle(T)                                                                    \
      T::Set(const Handle(Ghost_Object) & theObj, const Handle(T) & theAttr) { \
    if (!theObj || theObj->HasOwner())                                         \
      return nullptr;                                                          \
    Handle(T) anAttr = !theAttr ? new T : new T(*theAttr);                     \
    theObj->SetOwner(anAttr);                                                  \
    return anAttr;                                                             \
  }

class Ghost_Attribute : public LODoc_Attribute {
  friend class Ghost_Document;

public:
  Ghost_Attribute();

  inline Ghost_ObjectType Type() const { return myType; }

  inline const Ghost_Id &Id() const { return myId; }

  DEFINE_STANDARD_RTTIEXT(Ghost_Attribute, LODoc_Attribute)

  Naivis_GHOSTATTR_DEFINE_GETSET(Ghost_Attribute);

protected:
  void setType(const Ghost_ObjectType theType) { myType = theType; }

  void setId(const Ghost_Id &theId) { myId = theId; }

protected:
  Ghost_ObjectType myType;
  Ghost_Id myId;
};

DEFINE_STANDARD_HANDLE(Ghost_Attribute, LODoc_Attribute)

#endif
