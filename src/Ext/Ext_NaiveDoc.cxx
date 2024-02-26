#include <AIS_ColoredShape.hxx>
#include <AIS_Shape.hxx>
#include <AIS_TexturedShape.hxx>
#include <MeshVS_Mesh.hxx>
#include <XCAFPrs_AISObject.hxx>
#include <XCAFPrs_DocumentExplorer.hxx>

#include "Ext_NaiveDoc.hxx"
#include <NaiveDoc/NaiveDoc_Attribute.hxx>
#include <NaiveDoc/NaiveDoc_Document.hxx>

class NaiveDoc_DocumentExplorer : public Standard_Transient,
                                  public XCAFPrs_DocumentExplorer {
public:
  using XCAFPrs_DocumentExplorer::XCAFPrs_DocumentExplorer;
};

void Ext_NaiveDoc(lua_State *L) {
  LuaBridge__G(L)
      .Begin_Namespace(Naivis)
      .Begin_Namespace(NaiveDoc)

      .Begin_Class(NaiveDoc_Document)
      .addConstructorFrom<Handle(NaiveDoc_Document), void()>()
      .Bind_Property_Readonly(NaiveDoc_Document, Objects)
      .Bind_Method(NaiveDoc_Document, ImportStep)
      .Bind_Method(NaiveDoc_Document, ExportStep)
      .Bind_Method(NaiveDoc_Document, Undo)
      .Bind_Method(NaiveDoc_Document, Redo)
      .Bind_Method(NaiveDoc_Document, UpdateView)
      /// NOTE: Crash at the last iteration? WTF?
      // .Bind_Method(NaiveDoc_Document, GetXcafExplorer)
      /// WORKAROUND: Encapsule |XCAFPrs_DocumentExplorer| with
      /// |Standard_Transient| to take advantange of |handle|'s ability which
      /// shares life time with lua properly. But why?
      .addFunction(
          "GetXcafExplorer",
          +[](const NaiveDoc_Document &theSelf,
              Standard_Integer theFlag) -> Handle(NaiveDoc_DocumentExplorer) {
            return new NaiveDoc_DocumentExplorer(theSelf.Document(), 0);
          })
      .Bind_Method(NaiveDoc_Document, DumpXcafDocumentTree)
      .End_Class()

      .Begin_Class(NaiveDoc_Id)
      .addConstructor<void()>()
      .Bind_Method(NaiveDoc_Id, IsNull)
      .Bind_Method(NaiveDoc_Id, IsEqual)
      .Bind_Property_Readonly(NaiveDoc_Id, Depth)
      .addFunction(
          "__eq",
          +[](const NaiveDoc_Id &theSelf, const NaiveDoc_Id &theOther) {
            return theSelf == theOther;
          })
      .addFunction(
          "__tostring",
          +[](const NaiveDoc_Id &theSelf) {
            std::ostringstream oss{};
            oss << theSelf;
            return oss.str();
          })
      .End_Class()

      .Begin_Class(NaiveDoc_DocumentNode)
      .addProperty("Id", &NaiveDoc_DocumentNode::Id)
      .addProperty("Label", &NaiveDoc_DocumentNode::Label)
      .addProperty("RefLabel", &NaiveDoc_DocumentNode::RefLabel)
      // .addProperty("Style", &NaiveDoc_DocumentNode::Style)
      .addProperty("Location", &NaiveDoc_DocumentNode::Location)
      .addProperty("LocalTrsf", &NaiveDoc_DocumentNode::LocalTrsf)
      // .addProperty("ChildIter", &NaiveDoc_DocumentNode::ChildIter)
      .addProperty("IsAssembly", &NaiveDoc_DocumentNode::IsAssembly)
      .addFunction(
          "__eq",
          +[](const NaiveDoc_DocumentNode &theSelf,
              const NaiveDoc_DocumentNode &theOther) {
            return NaiveDoc_DocumentNode::IsEqual(theSelf, theOther);
          })
      .addFunction(
          "__tostring",
          +[](const NaiveDoc_DocumentNode &theSelf) -> std::string {
            return theSelf.Id.ToCString();
          })
      .End_Class()

      .Begin_Derive(NaiveDoc_DocumentExplorer, Standard_Transient)
      .addConstructorFrom<Handle(NaiveDoc_DocumentExplorer), void()>()
      .Bind_Method(NaiveDoc_DocumentExplorer, More)
      .Bind_Method(NaiveDoc_DocumentExplorer, Next)
      .addFunction("Current",
                   luabridge::overload<>(&NaiveDoc_DocumentExplorer::Current),
                   luabridge::overload<Standard_Integer>(
                       &NaiveDoc_DocumentExplorer::Current))
      .Bind_Method(NaiveDoc_DocumentExplorer, CurrentDepth)
      .End_Class()

      .Begin_Class(NaiveDoc_Attribute)
      .addStaticFunction("GetId", &NaiveDoc_Attribute::GetId)
      .addStaticFunction(
          "GetName",
          luabridge::overload<const NaiveDoc_Id &, Standard_Boolean>(
              &NaiveDoc_Attribute::GetName),
          luabridge::overload<const Handle(NaiveDoc_Object) &,
                              Standard_Boolean>(&NaiveDoc_Attribute::GetName))
      .addStaticFunction(
          "GetShape",
          luabridge::overload<const NaiveDoc_Id &>(
              &NaiveDoc_Attribute::GetShape),
          luabridge::overload<const Handle(NaiveDoc_Object) &,
                              TopLoc_Location &>(&NaiveDoc_Attribute::GetShape))
      .addStaticFunction(
          "GetInteger",
          +[](const NaiveDoc_Id &theId, const Standard_GUID &theGuid)
              -> std::tuple<Standard_Boolean, Standard_Integer> {
            Standard_Integer aValue = 0;
            Standard_Integer aRes =
                NaiveDoc_Attribute::GetInteger(theId, theGuid, aValue);
            return {aRes, aValue};
          },
          +[](const Handle(NaiveDoc_Object) & theObj,
              const Standard_GUID &theGuid)
              -> std::tuple<Standard_Boolean, Standard_Integer> {
            Standard_Integer aValue = 0;
            Standard_Integer aRes =
                NaiveDoc_Attribute::GetInteger(theObj, theGuid, aValue);
            return {aRes, aValue};
          })
      .addStaticFunction(
          "SetInteger",
          luabridge::overload<const NaiveDoc_Id &, const Standard_GUID &,
                              Standard_Integer>(
              &NaiveDoc_Attribute::SetInteger),
          luabridge::overload<const Handle(NaiveDoc_Object) &,
                              const Standard_GUID &, Standard_Integer>(
              &NaiveDoc_Attribute::SetInteger))
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
      .addFunction("DeleteObject",
                   luabridge::overload<const NaiveDoc_Id &, Standard_Boolean>(
                       &NaiveDoc_ObjectTable::DeleteObject),
                   luabridge::overload<const Handle(NaiveDoc_Object) &,
                                       Standard_Boolean>(
                       &NaiveDoc_ObjectTable::DeleteObject))
      .addFunction(
          "DeleteObjects",
          luabridge::overload<const NaiveDoc_IdList &, Standard_Boolean>(
              &NaiveDoc_ObjectTable::DeleteObjects),
          luabridge::overload<const NaiveDoc_ObjectList &, Standard_Boolean>(
              &NaiveDoc_ObjectTable::DeleteObjects))
      .addFunction("ShowObject",
                   luabridge::overload<const NaiveDoc_Id &, Standard_Boolean>(
                       &NaiveDoc_ObjectTable::ShowObject),
                   luabridge::overload<const Handle(NaiveDoc_Object) &,
                                       Standard_Boolean>(
                       &NaiveDoc_ObjectTable::ShowObject))
      .addFunction(
          "ShowObjects",
          luabridge::overload<const NaiveDoc_IdList &, Standard_Boolean>(
              &NaiveDoc_ObjectTable::ShowObjects),
          luabridge::overload<const NaiveDoc_ObjectList &, Standard_Boolean>(
              &NaiveDoc_ObjectTable::ShowObjects))
      .Bind_Method(NaiveDoc_ObjectTable, ShowAll)
      .addFunction("HideObject",
                   luabridge::overload<const NaiveDoc_Id &, Standard_Boolean>(
                       &NaiveDoc_ObjectTable::HideObject),
                   luabridge::overload<const Handle(NaiveDoc_Object) &,
                                       Standard_Boolean>(
                       &NaiveDoc_ObjectTable::HideObject))
      .addFunction(
          "HideObjects",
          luabridge::overload<const NaiveDoc_IdList &, Standard_Boolean>(
              &NaiveDoc_ObjectTable::HideObjects),
          luabridge::overload<const NaiveDoc_ObjectList &, Standard_Boolean>(
              &NaiveDoc_ObjectTable::HideObjects))
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
      .addFunction("SelectObject",
                   luabridge::overload<const NaiveDoc_Id &, Standard_Boolean,
                                       Standard_Boolean>(
                       &NaiveDoc_ObjectTable::SelectObject),
                   luabridge::overload<const Handle(NaiveDoc_Object) &,
                                       Standard_Boolean, Standard_Boolean>(
                       &NaiveDoc_ObjectTable::SelectObject))
      .addFunction("SelectObjects",
                   luabridge::overload<const NaiveDoc_IdList &,
                                       Standard_Boolean, Standard_Boolean>(
                       &NaiveDoc_ObjectTable::SelectObjects),
                   luabridge::overload<const NaiveDoc_ObjectList &,
                                       Standard_Boolean, Standard_Boolean>(
                       &NaiveDoc_ObjectTable::SelectObjects))
      .Bind_Method(NaiveDoc_ObjectTable, SelectAll)
      .Bind_Method(NaiveDoc_ObjectTable, UnselectAll)
      .Bind_Method(NaiveDoc_ObjectTable, SelectedObjects)
      .End_Class()

      .End_Namespace()
      .End_Namespace();

  // LuaBridge__G(L)
  //     .Begin_Namespace(LuaOCCT)
  //     .Begin_Namespace(AIS)

  //     .Begin_Derive(AIS_Shape, NaiveDoc_Object)
  //     .addConstructorFrom<Handle(AIS_Shape), void(const TopoDS_Shape &)>()
  //     .Bind_Property_Readonly(AIS_Shape, Shape)
  //     .Bind_DownCast1(AIS_Shape, NaiveDoc_Object)
  //     .End_Derive()

  //     .Begin_Derive(AIS_ColoredShape, AIS_Shape)
  //     .addConstructorFrom<Handle(AIS_ColoredShape), void(const TopoDS_Shape &),
  //                         void(const Handle(AIS_Shape) &)>()
  //     .Bind_DownCast1(AIS_ColoredShape, NaiveDoc_Object)
  //     .End_Derive()

  //     .Begin_Derive(XCAFPrs_AISObject, AIS_ColoredShape)
  //     .addConstructorFrom<Handle(XCAFPrs_AISObject),
  //                         void(const NaiveDoc_Id &)>()
  //     .Bind_DownCast1(XCAFPrs_AISObject, NaiveDoc_Object)
  //     .End_Derive()

  //     .Begin_Derive(MeshVS_Mesh, NaiveDoc_Object)
  //     .addConstructorFrom<Handle(MeshVS_Mesh), void(const Standard_Boolean)>()
  //     .Bind_DownCast1(MeshVS_Mesh, NaiveDoc_Object)
  //     .End_Derive()

  //     .End_Namespace()
  //     .End_Namespace();
}
