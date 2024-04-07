#include "Ghost_Context.hxx"

#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>

Ghost_Context::Ghost_Context() {}

static TopoDS_Shape makeArrow(const gp_Vec &theV, const gp_Pnt &theAnchor,
                              const Standard_Real theTipL,
                              const Standard_Real theTipR,
                              const Standard_Real theBodyR) {
  if (theTipL <= Precision::Confusion() || theTipR <= Precision::Confusion() ||
      theBodyR <= Precision::Confusion() || theTipR < theBodyR)
    return TopoDS_Shape();

  Standard_Real aVLen = theV.Magnitude();
  if (aVLen < Precision::Confusion())
    return TopoDS_Shape();

  Standard_Real aTipLen = theTipL;
  if (aVLen - aTipLen < Precision::Confusion())
    aTipLen = 0.38 * aVLen;

  Standard_Real aBodyLen = aVLen - aTipLen;
  if (aTipLen <= Precision::Confusion() || aBodyLen <= Precision::Confusion())
    return TopoDS_Shape();

  gp_Ax2 aBase1(theAnchor, theV);
  TopoDS_Shape aBody = BRepPrimAPI_MakeCylinder(aBase1, theBodyR, aBodyLen);
  gp_Ax2 aBase2(theAnchor.Translated(theV), -theV);
  TopoDS_Shape aTip = BRepPrimAPI_MakeCone(aBase2, 0, theTipR, aTipLen);

  TopoDS_Compound aCompound{};
  BRep_Builder aBuilder{};
  aBuilder.MakeCompound(aCompound);
  aBuilder.Add(aCompound, aBody);
  aBuilder.Add(aCompound, aTip);

  return aCompound;
}

QUuid Ghost_Context::AddVector(const gp_Vec &theV, const gp_Pnt &theAnchor,
                               const Handle(NaiveDoc_Attribute) & theAttr,
                               Standard_Boolean theToUpdate) {
  TopoDS_Shape anArrow = makeArrow(theV, theAnchor, 5, 2, 1);
  if (anArrow.IsNull())
    return {};

  return {};
}

QUuid Ghost_Context::AddAx3(const gp_Ax3 &theAx3,
                            const Handle(NaiveDoc_Attribute) & theAttr,
                            Standard_Boolean theToUpdate) {
  return {};
}
