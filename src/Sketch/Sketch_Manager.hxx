#ifndef _Naivis_Sketch_Manager_HeaderFile
#define _Naivis_Sketch_Manager_HeaderFile

#include <Standard.hxx>
#include <Standard_Handle.hxx>
#include <Standard_Type.hxx>
#include <Standard_Transient.hxx>

class Sketch_Manager;
DEFINE_STANDARD_HANDLE(Sketch_Manager, Standard_Transient)

class Sketch_Manager : public Standard_Transient {
public:
  Sketch_Manager();
  
  ~Sketch_Manager();

public:
  DEFINE_STANDARD_RTTIEXT(Sketch_Manager, Standard_Transient)

protected:
private:
};

#endif
