#ifndef _Naivis_NaiveDoc_Attribute_HeaderFile
#define _Naivis_NaiveDoc_Attribute_HeaderFile

#include <TPrsStd_AISPresentation.hxx>
#include <TopLoc_Location.hxx>

#include "NaiveDoc_Object.hxx"

class NaiveDoc_Attribute {
public:
  static NaiveDoc_Id GetId(const Handle(NaiveDoc_Object) & theObj);

  static TCollection_AsciiString
  GetName(const Handle(NaiveDoc_Object) & theObj,
          Standard_Boolean theIsInstanceName = Standard_False);

  static Handle(TPrsStd_AISPresentation) GetPrs(const NaiveDoc_Id &theId);

  static Handle(TPrsStd_AISPresentation)
      GetPrs(const Handle(NaiveDoc_Object) & theObj);

  static TopoDS_Shape GetShape(const Handle(NaiveDoc_Object) & theObj,
                               TopLoc_Location &theLoc);
};

#endif
