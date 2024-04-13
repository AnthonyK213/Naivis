#include "Ghost_Document.hxx"

#include <AIS_ColoredShape.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>

#include <Util/Util_Mesh.hxx>

IMPLEMENT_STANDARD_RTTIEXT(Ghost_Document, Standard_Transient)

Ghost_Document::Ghost_Document(const Handle(AIS_InteractiveContext) &
                               theContext)
    : myContext(theContext) {}

Ghost_Id Ghost_Document::AddShape(const TopoDS_Shape &theShape,
                                  const Handle(Ghost_Attribute) & theAttr,
                                  Standard_Boolean theToUpdate) {
  if (!myContext || theShape.IsNull())
    return {};

  Handle(AIS_ColoredShape) aShape = new AIS_ColoredShape(theShape);
  Handle(Ghost_Attribute) anAttr = Ghost_Attribute::Set(aShape, theAttr);
  if (!anAttr)
    return {};

  Ghost_Id anId = Ghost_Id::createUuid();
  anAttr->setType(Ghost_OTShape);
  anAttr->setId(anId);
  aShape->SetColor(anAttr->Color());
  myContext->Display(aShape, AIS_Shaded, -1, theToUpdate);
  myObjects.insert(anId, aShape);

  return anId;
}

Ghost_Id Ghost_Document::AddMesh(const Handle(Poly_Triangulation) & theMesh,
                                 const Handle(Ghost_Attribute) & theAttr,
                                 Standard_Boolean theToUpdate) {
  if (!myContext || theMesh.IsNull())
    return {};

  Handle(MeshVS_Mesh) aMesh = Util_Mesh::CreateMeshVS(theMesh);
  Handle(Ghost_Attribute) anAttr = Ghost_Attribute::Set(aMesh, theAttr);
  if (!anAttr)
    return {};

  Ghost_Id anId = Ghost_Id::createUuid();
  anAttr->setType(Ghost_OTMesh);
  anAttr->setId(anId);
  myContext->Display(aMesh, MeshVS_DMF_Shading, -1, theToUpdate);
  myObjects.insert(anId, aMesh);

  return anId;
}

static TopoDS_Shape makeArrow(const gp_Vec &theV, const gp_Pnt &theAnchor,
                              const Handle(Ghost_AttrOfVector) & theAttr) {
  if (!theAttr)
    return TopoDS_Shape();

  Standard_Real aVLen = theV.Magnitude();
  if (aVLen < Precision::Confusion())
    return TopoDS_Shape();

  Standard_Real aTipLen = theAttr->TipL();
  if (aVLen - aTipLen < Precision::Confusion())
    aTipLen = 0.38 * aVLen;

  Standard_Real aBodyLen = aVLen - aTipLen;
  if (aTipLen <= Precision::Confusion() || aBodyLen <= Precision::Confusion())
    return TopoDS_Shape();

  gp_Ax2 aBase1(theAnchor, theV);
  TopoDS_Shape aBody =
      BRepPrimAPI_MakeCylinder(aBase1, theAttr->BodyR(), aBodyLen);
  gp_Ax2 aBase2(theAnchor.Translated(theV), -theV);
  TopoDS_Shape aTip = BRepPrimAPI_MakeCone(aBase2, 0, theAttr->TipR(), aTipLen);

  TopoDS_Compound aCompound{};
  BRep_Builder aBuilder{};
  aBuilder.MakeCompound(aCompound);
  aBuilder.Add(aCompound, aBody);
  aBuilder.Add(aCompound, aTip);

  return aCompound;
}

Ghost_Id Ghost_Document::AddVector(const gp_Vec &theV, const gp_Pnt &theAnchor,
                                   const Handle(Ghost_AttrOfVector) & theAttr,
                                   Standard_Boolean theToUpdate) {
  TopoDS_Shape anArrow = makeArrow(theV, theAnchor, theAttr);
  Ghost_Id anId = AddShape(anArrow, theAttr, theToUpdate);
  Handle(Ghost_Object) anObj = Find(anId);
  Handle(Ghost_AttrOfVector) anAttr = Ghost_AttrOfVector::Get(anObj);
  if (!anAttr)
    return {};
  anAttr->setType(Ghost_OTVector);
  return anId;
}

Ghost_Id Ghost_Document::AddAx3(const gp_Ax3 &theAx3,
                                const Handle(Ghost_AttrOfVector) & theAttr,
                                Standard_Boolean theToUpdate) {
  Handle(Ghost_AttrOfVector) anAttr =
      !theAttr ? new Ghost_AttrOfVector : theAttr;
  const gp_Pnt &anAnchor = theAx3.Location();
  const Standard_Real aTL = anAttr->TipL();
  const Standard_Real aTR = anAttr->TipR();
  const Standard_Real aBR = anAttr->BodyR();
  const Standard_Real aL = 42;
  const gp_Vec &aVX = theAx3.XDirection();
  const gp_Vec &aVY = theAx3.YDirection();
  const gp_Vec &aVZ = theAx3.Direction();
  TopoDS_Shape aX = makeArrow(aVX * aL, anAnchor, anAttr);
  TopoDS_Shape aY = makeArrow(aVY * aL, anAnchor, anAttr);
  TopoDS_Shape aZ = makeArrow(aVZ * aL, anAnchor, anAttr);

  TopoDS_Compound aCompound{};
  BRep_Builder aBuilder{};
  aBuilder.MakeCompound(aCompound);
  aBuilder.Add(aCompound, aX);
  aBuilder.Add(aCompound, aY);
  aBuilder.Add(aCompound, aZ);

  Handle(AIS_ColoredShape) aShape = new AIS_ColoredShape(aCompound);
  Handle(Ghost_Attribute) anAttr1 = Ghost_Attribute::Set(aShape, anAttr);
  Ghost_Id anId = Ghost_Id::createUuid();
  anAttr1->setType(Ghost_OTAx3);
  anAttr1->setId(anId);
  aShape->SetCustomColor(aX, Quantity_NOC_RED);
  aShape->SetCustomColor(aY, Quantity_NOC_GREEN);
  aShape->SetCustomColor(aZ, Quantity_NOC_BLUE);
  myContext->Display(aShape, AIS_Shaded, -1, theToUpdate);
  myObjects.insert(anId, aShape);

  return anId;
}

Standard_Boolean Ghost_Document::DeleteObject(const Ghost_Id &theId,
                                              Standard_Boolean theToUpdate) {
  if (!deleteObjectRaw(theId))
    return Standard_False;

  if (theToUpdate)
    myContext->UpdateCurrentViewer();

  return Standard_True;
}

Standard_Integer Ghost_Document::DeleteObjects(const QList<Ghost_Id> &theIds,
                                               Standard_Boolean theToUpdate) {
  Standard_Integer nbDelete = 0;

  for (const Ghost_Id &anId : theIds) {
    if (deleteObjectRaw(anId))
      nbDelete++;
  }

  return nbDelete;
}

Standard_Integer Ghost_Document::TransformObject(const Ghost_Id &theId,
                                                 Standard_Boolean theToUpdate) {
  return Standard_True;
}

Handle(Ghost_Object) Ghost_Document::Find(const Ghost_Id &theId) const {
  auto it = myObjects.find(theId);
  if (it != myObjects.end())
    return it.value();
  return nullptr;
}

void Ghost_Document::Clear(const Standard_Boolean theToUpdate) {
  for (const auto &anObj : myObjects) {
    myContext->Remove(anObj, Standard_False);
  }

  myObjects.clear();
}

Standard_Boolean Ghost_Document::deleteObjectRaw(const Ghost_Id &theId) {
  auto it = myObjects.find(theId);
  if (it == myObjects.end())
    return Standard_False;

  Handle(Ghost_Object) anObj = it.value();
  myContext->Remove(anObj, Standard_False);
  myObjects.erase(it);

  return Standard_True;
}
