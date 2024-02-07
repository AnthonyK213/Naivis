#ifndef _Naivis_NaiveDoc_Attribute_HeaderFile
#define _Naivis_NaiveDoc_Attribute_HeaderFile

#include <TPrsStd_AISPresentation.hxx>
#include <TopLoc_Location.hxx>

#include "NaiveDoc_Object.hxx"

class NaiveDoc_Attribute {
public:
  static NaiveDoc_Id GetId(const Handle(NaiveDoc_Object) & theObj);

  static TCollection_ExtendedString
  GetName(const NaiveDoc_Id &theId,
          Standard_Boolean theIsInstanceName = Standard_False);

  static TCollection_ExtendedString
  GetName(const Handle(NaiveDoc_Object) & theObj,
          Standard_Boolean theIsInstanceName = Standard_False);

  static Handle(TPrsStd_AISPresentation) GetPrs(const NaiveDoc_Id &theId);

  static Handle(TPrsStd_AISPresentation)
      GetPrs(const Handle(NaiveDoc_Object) & theObj);

  static TopoDS_Shape GetShape(const NaiveDoc_Id &theId);

  static TopoDS_Shape GetShape(const Handle(NaiveDoc_Object) & theObj,
                               TopLoc_Location &theLoc);

  static Standard_Boolean GetInteger(const NaiveDoc_Id &theObj,
                                     const Standard_GUID &theGuid,
                                     Standard_Integer &theValue);

  static Standard_Boolean GetInteger(const Handle(NaiveDoc_Object) & theObj,
                                     const Standard_GUID &theGuid,
                                     Standard_Integer &theValue);

  static Standard_Boolean SetInteger(const NaiveDoc_Id &theId,
                                     const Standard_GUID &theGuid,
                                     Standard_Integer theValue);

  static Standard_Boolean SetInteger(const Handle(NaiveDoc_Object) & theObj,
                                     const Standard_GUID &theGuid,
                                     Standard_Integer theValue);
};

#endif
