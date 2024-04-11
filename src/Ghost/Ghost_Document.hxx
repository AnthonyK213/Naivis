#ifndef _Naivis_Ghost_Document_HeaderFile
#define _Naivis_Ghost_Document_HeaderFile

#include <AIS_InteractiveContext.hxx>
#include <Poly_Triangulation.hxx>

#include "Ghost_Attribute.hxx"

#include <QMap>
#include <QUuid>

class Ghost_Document : public Standard_Transient {
public:
  explicit Ghost_Document(const Handle(AIS_InteractiveContext) & theContext);

  QUuid AddShape(const TopoDS_Shape &theShape,
                 const Handle(Ghost_Attribute) & theAttr,
                 Standard_Boolean theToUpdate = Standard_False);

  QUuid AddMesh(const Handle(Poly_Triangulation) & theMesh,
                const Handle(Ghost_Attribute) & theAttr,
                Standard_Boolean theToUpdate = Standard_False);

  QUuid AddVector(const gp_Vec &theV, const gp_Pnt &theAnchor,
                  const Handle(Ghost_Attribute) & theAttr,
                  Standard_Boolean theToUpdate = Standard_False);

  QUuid AddAx3(const gp_Ax3 &theAx3, const Handle(Ghost_Attribute) & theAttr,
               Standard_Boolean theToUpdate = Standard_False);

  Standard_Boolean DeleteObject(const QUuid &theId,
                                Standard_Boolean theToUpdate = Standard_False);

  Standard_Integer DeleteObjects(const QList<QUuid> &theIds,
                                 Standard_Boolean theToUpdate = Standard_False);

  Standard_Integer
  TransformObject(const QUuid &theId,
                  Standard_Boolean theToUpdate = Standard_False);

  void Clear(Standard_Boolean theToUpdate);

  DEFINE_STANDARD_RTTIEXT(Ghost_Document, Standard_Transient)

private:
  Standard_Boolean deleteObjectRaw(const QUuid &theId);

private:
  Handle(AIS_InteractiveContext) myContext;
  QMap<QUuid, Handle(AIS_InteractiveObject)> myObjects;
};

DEFINE_STANDARD_HANDLE(Ghost_Document, Standard_Transient)

#endif
