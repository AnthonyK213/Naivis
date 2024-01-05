#include "Ext_NaiveDoc.hxx"
#include <NaiveDoc/NaiveDoc_Document.hxx>

#include <AIS_ColoredShape.hxx>
#include <AIS_Shape.hxx>
#include <AIS_TexturedShape.hxx>
#include <MeshVS_Mesh.hxx>
#include <XCAFPrs_AISObject.hxx>

void Ext_NaiveDoc(lua_State *L) {
  LuaBridge__G(L)
      .Begin_Namespace(Naivis)

      .Begin_Class(NaiveDoc_Document)
      .Bind_Property_Readonly(NaiveDoc_Document, Objects)
      .Bind_Method(NaiveDoc_Document, ImportStep)
      .Bind_Method(NaiveDoc_Document, Undo)
      .Bind_Method(NaiveDoc_Document, Redo)
      .Bind_Method(NaiveDoc_Document, UpdateView)
      .End_Class()

      .Begin_Class(NaiveDoc_Object)
      .Bind_Property_Readonly(NaiveDoc_Object, Type)
      .Bind_Property_Readonly(NaiveDoc_Object, Signature)
      .addFunction("Id", &NaiveDoc_Object_GetId)
      .addFunction("Name", &NaiveDoc_Object_GetName)
      .End_Class()

      .Begin_Class(NaiveDoc_ObjectTable)
      .Bind_Method(NaiveDoc_ObjectTable, AddShape)
      .Bind_Method(NaiveDoc_ObjectTable, AddMesh)
      .addFunction("DeleteObject",
                   luabridge::overload<const Handle(NaiveDoc_Object) &,
                                       Standard_Boolean>(
                       &NaiveDoc_ObjectTable::DeleteObject),
                   luabridge::overload<const NaiveDoc_Id &, Standard_Boolean>(
                       &NaiveDoc_ObjectTable::DeleteObject))
      .addFunction(
          "DeleteObjects",
          luabridge::overload<const NaiveDoc_ObjectList &, Standard_Boolean>(
              &NaiveDoc_ObjectTable::DeleteObjects))
      .Bind_Method(NaiveDoc_ObjectTable, SelectedObjects)
      .End_Class()

      .Begin_Derive(AIS_Shape, NaiveDoc_Object)
      .addConstructorFrom<Handle(AIS_Shape), void(const TopoDS_Shape &)>()
      .Bind_Property_Readonly(AIS_Shape, Shape)
      .Bind_DownCast1(AIS_Shape, NaiveDoc_Object)
      .End_Derive()

      .Begin_Derive(AIS_ColoredShape, AIS_Shape)
      .addConstructorFrom<Handle(AIS_ColoredShape), void(const TopoDS_Shape &),
                          void(const Handle(AIS_Shape) &)>()
      .Bind_DownCast1(AIS_ColoredShape, NaiveDoc_Object)
      .End_Derive()

      .Begin_Derive(XCAFPrs_AISObject, AIS_ColoredShape)
      .addConstructorFrom<Handle(XCAFPrs_AISObject), void(const TDF_Label &)>()
      .Bind_DownCast1(XCAFPrs_AISObject, NaiveDoc_Object)
      .End_Derive()

      .Begin_Derive(MeshVS_Mesh, NaiveDoc_Object)
      .addConstructorFrom<Handle(MeshVS_Mesh), void(const Standard_Boolean)>()
      .Bind_DownCast1(MeshVS_Mesh, NaiveDoc_Object)
      .End_Derive()

      .End_Namespace();
}
