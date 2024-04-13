#ifndef _Naivis_Ghost_Document_HeaderFile
#define _Naivis_Ghost_Document_HeaderFile

#include <Poly_Triangulation.hxx>

#include "Ghost_AttrOfVector.hxx"
#include "Ghost_Attribute.hxx"
#include "Ghost_Object.hxx"

class Ghost_Document : public Standard_Transient {
public:
  explicit Ghost_Document(const Handle(AIS_InteractiveContext) & theContext);

  Ghost_Id AddShape(const TopoDS_Shape &theShape,
                    const Handle(Ghost_Attribute) & theAttr,
                    Standard_Boolean theToUpdate = Standard_False);

  Ghost_Id AddMesh(const Handle(Poly_Triangulation) & theMesh,
                   const Handle(Ghost_Attribute) & theAttr,
                   Standard_Boolean theToUpdate = Standard_False);

  Ghost_Id AddVector(const gp_Vec &theV, const gp_Pnt &theAnchor,
                     const Handle(Ghost_AttrOfVector) & theAttr,
                     Standard_Boolean theToUpdate = Standard_False);

  Ghost_Id AddAx3(const gp_Ax3 &theAx3,
                  const Handle(Ghost_AttrOfVector) & theAttr,
                  Standard_Boolean theToUpdate = Standard_False);

  Standard_Boolean DeleteObject(const Ghost_Id &theId,
                                Standard_Boolean theToUpdate = Standard_False);

  Standard_Integer DeleteObjects(const QList<Ghost_Id> &theIds,
                                 Standard_Boolean theToUpdate = Standard_False);

  Standard_Integer
  TransformObject(const Ghost_Id &theId,
                  Standard_Boolean theToUpdate = Standard_False);

  Handle(Ghost_Object) Find(const Ghost_Id &theId) const;

  void Clear(Standard_Boolean theToUpdate);

  DEFINE_STANDARD_RTTIEXT(Ghost_Document, Standard_Transient)

private:
  Standard_Boolean deleteObjectRaw(const Ghost_Id &theId);

private:
  Handle(AIS_InteractiveContext) myContext;
  Ghost_ObjectMap myObjects;
};

DEFINE_STANDARD_HANDLE(Ghost_Document, Standard_Transient)

#endif
