#ifndef _Naivis_Ghost_Attribute_HeaderFile
#define _Naivis_Ghost_Attribute_HeaderFile

#include <luaocct/LODoc_Attribute.hxx>

#include <QUuid>

class Ghost_Attribute : public LODoc_Attribute {
  friend class Ghost_Document;

public:
  Ghost_Attribute();

  inline const QUuid &Id() const { return myId; }

  DEFINE_STANDARD_RTTIEXT(Ghost_Attribute, LODoc_Attribute)

private:
  void setId(const QUuid &theId) { myId = theId; }

private:
  QUuid myId;
};

DEFINE_STANDARD_HANDLE(Ghost_Attribute, LODoc_Attribute)

#endif
