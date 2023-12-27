#ifndef _Naivis_NaiveDoc_Object_HeaderFile
#define _Naivis_NaiveDoc_Object_HeaderFile

#include <AIS_InteractiveObject.hxx>
#include <Standard.hxx>
#include <Standard_Handle.hxx>
#include <Standard_Type.hxx>

class NaiveDoc_Object;
DEFINE_STANDARD_HANDLE(NaiveDoc_Object, Standard_Transient)

class NaiveDoc_Object : public Standard_Transient {
public:
  NaiveDoc_Object();

  ~NaiveDoc_Object();

public:
  DEFINE_STANDARD_RTTIEXT(NaiveDoc_Object, Standard_Transient)

protected:
private:
  Handle(AIS_InteractiveObject) myObject;
};

#endif
