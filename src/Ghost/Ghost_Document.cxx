#include "Ghost_Document.hxx"

#include <AIS_ColoredShape.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>

IMPLEMENT_STANDARD_RTTIEXT(Ghost_Document, Standard_Transient)

Ghost_Document::Ghost_Document(const Handle(AIS_InteractiveContext) &
                               theContext)
    : myContext(theContext) {}

QUuid Ghost_Document::AddShape(const TopoDS_Shape &theShape,
                               const Handle(Ghost_Attribute) & theAttr,
                               Standard_Boolean theToUpdate) {
  if (!myContext || theShape.IsNull())
    return {};

  Handle(Ghost_Attribute) anAttr = !theAttr ? new Ghost_Attribute() : theAttr;
  Handle(AIS_ColoredShape) aShape = new AIS_ColoredShape(theShape);
  QUuid anId = QUuid::createUuid();
  anAttr->setId(anId);
  aShape->SetOwner(anAttr);
  aShape->SetColor(anAttr->Color());
  myContext->Display(aShape, AIS_Shaded, -1, theToUpdate);

  return anId;
}

QUuid Ghost_Document::AddMesh(const Handle(Poly_Triangulation) & theMesh,
                              const Handle(Ghost_Attribute) & theAttr,
                              Standard_Boolean theToUpdate) {
  if (!myContext || theMesh.IsNull())
    return {};

  return {};
}

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

QUuid Ghost_Document::AddVector(const gp_Vec &theV, const gp_Pnt &theAnchor,
                                const Handle(Ghost_Attribute) & theAttr,
                                Standard_Boolean theToUpdate) {
  TopoDS_Shape anArrow = makeArrow(theV, theAnchor, 5, 2, 1);
  return AddShape(anArrow, theAttr, theToUpdate);
}

QUuid Ghost_Document::AddAx3(const gp_Ax3 &theAx3,
                             const Handle(Ghost_Attribute) & theAttr,
                             Standard_Boolean theToUpdate) {
  const gp_Pnt &anAnchor = theAx3.Location();
  const Standard_Real aTL = 5;
  const Standard_Real aTR = 2;
  const Standard_Real aBR = 1;
  const Standard_Real aL = 42;
  const gp_Vec &aVX = theAx3.XDirection();
  const gp_Vec &aVY = theAx3.YDirection();
  const gp_Vec &aVZ = theAx3.Direction();
  TopoDS_Shape aX = makeArrow(aVX * aL, anAnchor, aTL, aTR, aBR);
  TopoDS_Shape aY = makeArrow(aVY * aL, anAnchor, aTL, aTR, aBR);
  TopoDS_Shape aZ = makeArrow(aVZ * aL, anAnchor, aTL, aTR, aBR);

  TopoDS_Compound aCompound{};
  BRep_Builder aBuilder{};
  aBuilder.MakeCompound(aCompound);
  aBuilder.Add(aCompound, aX);
  aBuilder.Add(aCompound, aY);
  aBuilder.Add(aCompound, aZ);

  Handle(Ghost_Attribute) anAttr = !theAttr ? new Ghost_Attribute() : theAttr;
  Handle(AIS_ColoredShape) aShape = new AIS_ColoredShape(aCompound);
  QUuid anId = QUuid::createUuid();
  anAttr->setId(anId);
  aShape->SetOwner(anAttr);
  aShape->SetCustomColor(aX, Quantity_NOC_RED);
  aShape->SetCustomColor(aY, Quantity_NOC_GREEN);
  aShape->SetCustomColor(aZ, Quantity_NOC_BLUE);
  myContext->Display(aShape, AIS_Shaded, -1, theToUpdate);

  return anId;
}
