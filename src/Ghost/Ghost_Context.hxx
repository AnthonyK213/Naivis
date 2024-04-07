#ifndef _Naivis_Ghost_Context_HeaderFile
#define _Naivis_Ghost_Context_HeaderFile

#include <AIS_InteractiveContext.hxx>

#include <NaiveDoc/NaiveDoc_Attribute.hxx>

#include <QUuid>

class Ghost_Context {
public:
  Ghost_Context();

  QUuid AddVector(const gp_Vec &theV, const gp_Pnt &theAnchor,
                  const Handle(NaiveDoc_Attribute) & theAttr,
                  Standard_Boolean theToUpdate = Standard_False);

  QUuid AddAx3(const gp_Ax3 &theAx3, const Handle(NaiveDoc_Attribute) & theAttr,
               Standard_Boolean theToUpdate = Standard_False);

private:
  Handle(AIS_InteractiveContext) myContext;
};

#endif
