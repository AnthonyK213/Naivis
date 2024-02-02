#ifndef _Naivis_NaiveDoc_Attribute_HeaderFile
#define _Naivis_NaiveDoc_Attribute_HeaderFile

#include <TPrsStd_AISPresentation.hxx>

#include "NaiveDoc_Object.hxx"

class NaiveDoc_Attribute : public Standard_Transient {
public:
  NaiveDoc_Attribute();

  ~NaiveDoc_Attribute();

  DEFINE_STANDARD_RTTIEXT(NaiveDoc_Attribute, Standard_Transient)

public:
  static NaiveDoc_Id GetId(const Handle(NaiveDoc_Object) & theObj);

  static Handle(TPrsStd_AISPresentation) GetPrs(const NaiveDoc_Id &theId);

  static Handle(TPrsStd_AISPresentation)
      GetPrs(const Handle(NaiveDoc_Object) & theObj);
};

DEFINE_STANDARD_HANDLE(NaiveDoc_Attribute, Standard_Transient)

#endif
