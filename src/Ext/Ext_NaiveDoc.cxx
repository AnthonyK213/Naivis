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

      .Begin_Class(TDF_Label)
      .addConstructor<void()>()
      .Bind_Method(TDF_Label, IsNull)
      .Bind_Method(TDF_Label, IsEqual)
      .Bind_Property_Readonly(TDF_Label, Depth)
      .addFunction(
          "__eq",
          +[](const TDF_Label &theSelf, const TDF_Label &theOther) {
            return theSelf == theOther;
          })
      .addFunction(
          "__tostring",
          +[](const TDF_Label &theSelf) {
            std::ostringstream oss{};
            oss << theSelf;
            return oss.str();
          })
      .End_Class()

      .Begin_Class(NaiveDoc_Object)
      .Bind_Property_Readonly(NaiveDoc_Object, Type)
      .Bind_Property_Readonly(NaiveDoc_Object, Signature)
      .End_Class()

      .Begin_Class(NaiveDoc_ObjectTable)
      .Bind_Method(NaiveDoc_ObjectTable, Clear)
      .Bind_Method(NaiveDoc_ObjectTable, Find)
      .Bind_Method(NaiveDoc_ObjectTable, AddShape)
      .Bind_Method(NaiveDoc_ObjectTable, AddMesh)
      // .addFunction("DeleteObject",
      //              luabridge::overload<const Handle(NaiveDoc_Object) &,
      //                                  Standard_Boolean>(
      //                  &NaiveDoc_ObjectTable::DeleteObject),
      //              luabridge::overload<const NaiveDoc_Id &,
      //              Standard_Boolean>(
      //                  &NaiveDoc_ObjectTable::DeleteObject))
      // .addFunction(
      //     "DeleteObjects",
      //     luabridge::overload<const NaiveDoc_ObjectList &, Standard_Boolean>(
      //         &NaiveDoc_ObjectTable::DeleteObjects))
      // .addFunction("ShowObject",
      //              luabridge::overload<const Handle(NaiveDoc_Object) &,
      //                                  Standard_Boolean>(
      //                  &NaiveDoc_ObjectTable::ShowObject),
      //              luabridge::overload<const NaiveDoc_Id &,
      //              Standard_Boolean>(
      //                  &NaiveDoc_ObjectTable::ShowObject))
      // .addFunction(
      //     "ShowObjects",
      //     luabridge::overload<const NaiveDoc_ObjectList &, Standard_Boolean>(
      //         &NaiveDoc_ObjectTable::ShowObjects))
      // .Bind_Method(NaiveDoc_ObjectTable, ShowAll)
      // .addFunction("HideObject",
      //              luabridge::overload<const Handle(NaiveDoc_Object) &,
      //                                  Standard_Boolean>(
      //                  &NaiveDoc_ObjectTable::HideObject),
      //              luabridge::overload<const NaiveDoc_Id &,
      //              Standard_Boolean>(
      //                  &NaiveDoc_ObjectTable::HideObject))
      // .addFunction(
      //     "HideObjects",
      //     luabridge::overload<const NaiveDoc_ObjectList &, Standard_Boolean>(
      //         &NaiveDoc_ObjectTable::HideObjects))
      // .addFunction("PurgeObject",
      //              luabridge::overload<const Handle(NaiveDoc_Object) &,
      //                                  Standard_Boolean>(
      //                  &NaiveDoc_ObjectTable::PurgeObject),
      //              luabridge::overload<const NaiveDoc_Id &,
      //              Standard_Boolean>(
      //                  &NaiveDoc_ObjectTable::PurgeObject))
      // .addFunction(
      //     "PurgeObjects",
      //     luabridge::overload<const NaiveDoc_ObjectList &, Standard_Boolean>(
      //         &NaiveDoc_ObjectTable::PurgeObjects))
      // .addFunction("SelectObject",
      //              luabridge::overload<const Handle(NaiveDoc_Object) &,
      //                                  Standard_Boolean, Standard_Boolean>(
      //                  &NaiveDoc_ObjectTable::SelectObject),
      //              luabridge::overload<const NaiveDoc_Id &, Standard_Boolean,
      //                                  Standard_Boolean>(
      //                  &NaiveDoc_ObjectTable::SelectObject))
      // .addFunction("SelectObjects",
      //              luabridge::overload<const NaiveDoc_ObjectList &,
      //                                  Standard_Boolean, Standard_Boolean>(
      //                  &NaiveDoc_ObjectTable::SelectObjects))
      .Bind_Method(NaiveDoc_ObjectTable, SelectAll)
      .Bind_Method(NaiveDoc_ObjectTable, UnselectAll)
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
