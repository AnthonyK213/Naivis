#include "Ext_Ghost.hxx"

#include <Ghost/Ghost_AttrOfVector.hxx>
#include <Ghost/Ghost_Attribute.hxx>
#include <Ghost/Ghost_Document.hxx>

template <>
struct luabridge::Stack<Ghost_ObjectType>
    : luabridge::Enum<
          Ghost_ObjectType, Ghost_ObjectType::Ghost_OTNone,
          Ghost_ObjectType::Ghost_OTShape, Ghost_ObjectType::Ghost_OTMesh,
          Ghost_ObjectType::Ghost_OTVector, Ghost_ObjectType::Ghost_OTAx3> {};

void Ext_Ghost(lua_State *L) {
  LuaBridge__G(L)
      .Begin_Namespace(Naivis)
      .Begin_Namespace(Ghost)

      .Begin_Namespace("Ghost_ObjectType")
      .Bind_Enum(Ghost_ObjectType, Ghost_OTNone)
      .Bind_Enum(Ghost_ObjectType, Ghost_OTShape)
      .Bind_Enum(Ghost_ObjectType, Ghost_OTMesh)
      .Bind_Enum(Ghost_ObjectType, Ghost_OTVector)
      .Bind_Enum(Ghost_ObjectType, Ghost_OTAx3)
      .End_Namespace()

      .Begin_Class(Ghost_Id)
      .addConstructor<void(), void(const Ghost_Id &)>()
      .addFunction(
          "IsNull", +[](const Ghost_Id &self) { return self.isNull(); })
      .addFunction(
          "ToString", +[](const Ghost_Id &self) { return self.toString(); })
      .End_Class()

      .Begin_Derive(Ghost_Object, Standard_Transient)
      .Bind_DownCast(Ghost_Object)
      .End_Derive()

      .Begin_Derive(Ghost_Attribute, LODoc_Attribute)
      .addConstructorFrom<opencascade::handle<Ghost_Attribute>, void(),
                          void(const Ghost_Attribute &)>()
      .Bind_Method(Ghost_Attribute, Type)
      .Bind_Method(Ghost_Attribute, Id)
      .Bind_Method_Static(Ghost_Attribute, Get)
      .Bind_Method_Static(Ghost_Attribute, Set)
      .Bind_DownCast(Ghost_Attribute)
      .End_Derive()

      .Begin_Derive(Ghost_AttrOfVector, Ghost_Attribute)
      .addConstructorFrom<opencascade::handle<Ghost_AttrOfVector>, void(),
                          void(const Ghost_AttrOfVector &)>()
      .Bind_Method(Ghost_AttrOfVector, BodyR)
      .Bind_Method(Ghost_AttrOfVector, SetBodyR)
      .Bind_Method(Ghost_AttrOfVector, TipR)
      .Bind_Method(Ghost_AttrOfVector, SetTipR)
      .Bind_Method(Ghost_AttrOfVector, TipL)
      .Bind_Method(Ghost_AttrOfVector, SetTipL)
      .Bind_Method_Static(Ghost_AttrOfVector, Get)
      .Bind_Method_Static(Ghost_AttrOfVector, Set)
      .Bind_DownCast(Ghost_Attribute)
      .End_Derive()

      .Begin_Derive(Ghost_Document, Standard_Transient)
      .addConstructorFrom<opencascade::handle<Ghost_Document>,
                          void(const Handle(AIS_InteractiveContext) &)>()
      .Bind_Method(Ghost_Document, AddShape)
      .Bind_Method(Ghost_Document, AddMesh)
      .Bind_Method(Ghost_Document, AddVector)
      .Bind_Method(Ghost_Document, AddAx3)
      .Bind_Method(Ghost_Document, DeleteObject)
      .Bind_Method(Ghost_Document, DeleteObjects)
      .Bind_Method(Ghost_Document, Find)
      .Bind_Method(Ghost_Document, Clear)
      .End_Derive()

      .End_Namespace()
      .End_Namespace();
}
