#ifndef _Naivis_Ghost_AttrOfVector_HeaderFile
#define _Naivis_Ghost_AttrOfVector_HeaderFile

#include "Ghost_Attribute.hxx"

class Ghost_AttrOfVector : public Ghost_Attribute {
  friend class Ghost_Document;

public:
  Ghost_AttrOfVector();

  Standard_Real BodyR() const { return myData[0]; }

  void SetBodyR(const Standard_Real theBodyR) { setData(0, theBodyR); }

  Standard_Real TipR() const { return myData[1]; }

  void SetTipR(const Standard_Real theTipR) { setData(1, theTipR); }

  Standard_Real TipL() const { return myData[2]; }

  void SetTipL(const Standard_Real theTipL) { setData(2, theTipL); }

  DEFINE_STANDARD_RTTIEXT(Ghost_AttrOfVector, Ghost_Attribute)

  Naivis_GHOSTATTR_DEFINE_GETSET(Ghost_AttrOfVector);

private:
  void setData(const Standard_Integer theIndex, const Standard_Real theData);

private:
  Standard_Real myData[3];
};

DEFINE_STANDARD_HANDLE(Ghost_AttrOfVector, Ghost_Attribute)

#endif
