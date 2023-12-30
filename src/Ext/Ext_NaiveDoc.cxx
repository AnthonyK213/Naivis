#include "Ext_NaiveDoc.hxx"
#include <NaiveDoc/NaiveDoc_Document.hxx>

#include <TopoDS_Edge.hxx>

void Ext_NaiveDoc(lua_State *L) {
  LuaBridge__G(L)
      .Begin_Namespace(Naivis)

      .Begin_Class(NaiveDoc_Document)
      .Bind_Method(NaiveDoc_Document, ImportStep)
      .Bind_Method(NaiveDoc_Document, Undo)
      .Bind_Method(NaiveDoc_Document, Redo)
      .Bind_Method(NaiveDoc_Document, UpdateView)
      .addFunction("AddObject",
                   luabridge::overload<const TopoDS_Shape &, Standard_Boolean>(
                       &NaiveDoc_Document::AddObject))
      .End_Class()

      .Begin_Class(NaiveDoc_Object)
      .Bind_Property(NaiveDoc_Object, Name, SetName)
      .Bind_Property_Readonly(NaiveDoc_Object, Id)
      .addFunction("SetId", luabridge::overload<>(&NaiveDoc_Object::SetId))
      .End_Class()

      .End_Namespace();
}
