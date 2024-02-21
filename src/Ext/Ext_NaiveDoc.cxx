#include <AIS_ColoredShape.hxx>
#include <AIS_Shape.hxx>
#include <AIS_TexturedShape.hxx>
#include <MeshVS_Mesh.hxx>
#include <XCAFPrs_AISObject.hxx>
#include <XCAFPrs_DocumentExplorer.hxx>
#include <XCAFPrs_DocumentNode.hxx>

#include "Ext_NaiveDoc.hxx"
#include <NaiveDoc/NaiveDoc_Attribute.hxx>
#include <NaiveDoc/NaiveDoc_Document.hxx>

class Ext_DocumentExplorer : public Standard_Transient,
                             public XCAFPrs_DocumentExplorer {
public:
  using XCAFPrs_DocumentExplorer::XCAFPrs_DocumentExplorer;
};

void Ext_NaiveDoc(lua_State *L) {
  LuaBridge__G(L)
      .Begin_Namespace(Naivis)

      .Begin_Class(NaiveDoc_Document)
      .addConstructorFrom<Handle(NaiveDoc_Document), void()>()
      .Bind_Property_Readonly(NaiveDoc_Document, Objects)
      .Bind_Method(NaiveDoc_Document, ImportStep)
      .Bind_Method(NaiveDoc_Document, ExportStep)
      .Bind_Method(NaiveDoc_Document, Undo)
      .Bind_Method(NaiveDoc_Document, Redo)
      .Bind_Method(NaiveDoc_Document, UpdateView)
      /// FIXME: Crash at the last iteration? WTF?
      // .Bind_Method(NaiveDoc_Document, GetXcafExplorer)
      /// WORKAROUND A: Encapsule |XCAFPrs_DocumentExplorer| with
      /// |Standard_Transient| to take advantange of |handle|'s ability which
      /// shares life time with lua properly. But why?
      .addFunction(
          "GetXcafExplorer",
          +[](const NaiveDoc_Document &theSelf,
              Standard_Integer theFlag) -> Handle(Ext_DocumentExplorer) {
            return new Ext_DocumentExplorer(theSelf.Document(), 0);
          })
      /// WORKAROUND B: Returns a list of nodes instead of an
      /// |XCAFPrs_DocumentExplorer|.
      // .addFunction(
      //     "GetDocumentNodes",
      //     +[](const NaiveDoc_Document &theSelf,
      //         Standard_Integer theFlag) -> std::vector<XCAFPrs_DocumentNode>
      //         {
      //       std::vector<XCAFPrs_DocumentNode> aRes{};
      //       for (XCAFPrs_DocumentExplorer anExpl =
      //                theSelf.GetXcafExplorer(theFlag);
      //            anExpl.More(); anExpl.Next()) {
      //         aRes.push_back(anExpl.Current());
      //       }
      //       return aRes;
      //     })
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

      .Begin_Class(XCAFPrs_DocumentNode)
      .addProperty("Id", &XCAFPrs_DocumentNode::Id)
      .addProperty("Label", &XCAFPrs_DocumentNode::Label)
      .addProperty("RefLabel", &XCAFPrs_DocumentNode::RefLabel)
      // .addProperty("Style", &XCAFPrs_DocumentNode::Style)
      .addProperty("Location", &XCAFPrs_DocumentNode::Location)
      .addProperty("LocalTrsf", &XCAFPrs_DocumentNode::LocalTrsf)
      // .addProperty("ChildIter", &XCAFPrs_DocumentNode::ChildIter)
      .addProperty("IsAssembly", &XCAFPrs_DocumentNode::IsAssembly)
      .addFunction(
          "__eq",
          +[](const XCAFPrs_DocumentNode &theSelf,
              const XCAFPrs_DocumentNode &theOther) {
            return XCAFPrs_DocumentNode::IsEqual(theSelf, theOther);
          })
      .addFunction(
          "__tostring",
          +[](const XCAFPrs_DocumentNode &theSelf) -> std::string {
            return theSelf.Id.ToCString();
          })
      .End_Class()

      // .Begin_Class(XCAFPrs_DocumentExplorer)
      // .Bind_Method(XCAFPrs_DocumentExplorer, More)
      // .Bind_Method(XCAFPrs_DocumentExplorer, Next)
      // .addFunction("Current",
      //              luabridge::overload<>(&XCAFPrs_DocumentExplorer::Current),
      //              luabridge::overload<Standard_Integer>(
      //                  &XCAFPrs_DocumentExplorer::Current))
      // .Bind_Method(XCAFPrs_DocumentExplorer, CurrentDepth)
      // .End_Class()

      .Begin_Derive(Ext_DocumentExplorer, Standard_Transient)
      .addConstructorFrom<Handle(Ext_DocumentExplorer), void()>()
      .Bind_Method(Ext_DocumentExplorer, More)
      .Bind_Method(Ext_DocumentExplorer, Next)
      .addFunction(
          "Current", luabridge::overload<>(&Ext_DocumentExplorer::Current),
          luabridge::overload<Standard_Integer>(&Ext_DocumentExplorer::Current))
      .Bind_Method(Ext_DocumentExplorer, CurrentDepth)
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
      .addConstructorFrom<Handle(XCAFPrs_AISObject),
                          void(const NaiveDoc_Id &)>()
      .Bind_DownCast1(XCAFPrs_AISObject, NaiveDoc_Object)
      .End_Derive()

      .Begin_Derive(MeshVS_Mesh, NaiveDoc_Object)
      .addConstructorFrom<Handle(MeshVS_Mesh), void(const Standard_Boolean)>()
      .Bind_DownCast1(MeshVS_Mesh, NaiveDoc_Object)
      .End_Derive()

      .End_Namespace();
}
