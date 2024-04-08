#include "Ext_Ghost.hxx"

#include <Ghost/Ghost_Attribute.hxx>
#include <Ghost/Ghost_Document.hxx>

void Ext_Ghost(lua_State *L) {
  LuaBridge__G(L)
      .Begin_Namespace(Naivis)
      .Begin_Namespace(Ghost)

      .Begin_Derive(Ghost_Document, Standard_Transient)
      .addConstructorFrom<opencascade::handle<Ghost_Document>,
                          void(const Handle(AIS_InteractiveContext) &)>()
      .Bind_Method(Ghost_Document, AddShape)
      .Bind_Method(Ghost_Document, AddMesh)
      .Bind_Method(Ghost_Document, AddVector)
      .Bind_Method(Ghost_Document, AddAx3)
      .End_Derive()

      .Begin_Derive(Ghost_Attribute, LODoc_Attribute)
      .addConstructorFrom<opencascade::handle<Ghost_Attribute>, void()>()
      .End_Derive()

      .End_Namespace()
      .End_Namespace();
}
