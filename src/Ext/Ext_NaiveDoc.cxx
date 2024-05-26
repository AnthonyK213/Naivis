#include <AIS_ColoredShape.hxx>
#include <AIS_Shape.hxx>
#include <AIS_TexturedShape.hxx>
#include <MeshVS_Mesh.hxx>
#include <XCAFPrs_AISObject.hxx>
#include <XCAFPrs_DocumentExplorer.hxx>

#include "Ext_Bind.hxx"
#include <NaiveDoc/NaiveDoc_Document.hxx>
#include <NaiveDoc/NaiveDoc_Object.hxx>

void Ext_NaiveDoc(lua_State *L) {
  LuaBridge__G(L)
      .Begin_Namespace(Naivis)
      .Begin_Namespace(NaiveDoc)

      /* NOTE: Multiple inheritance is not supported by Luabridge3 yet,
       * **DO NOT** use this:
       * ```
       * Begin_Derive(NaiveDoc_Document, LODoc_Document)
       * ```
       * which will parses instance with a wrong offset.
       */
      .Begin_Class(NaiveDoc_Document)
      .addConstructorFrom<opencascade::handle<NaiveDoc_Document>, void()>()
      .Bind_Method(NaiveDoc_Document, InitHeadless)
      .Bind_Method(NaiveDoc_Document, Close)
      .Bind_Method(NaiveDoc_Document, DocumentExplorer)
      .Bind_Method(NaiveDoc_Document, DumpXcafDocumentTree)
      .Bind_Method(NaiveDoc_Document, ExportStep)
      .Bind_Method(NaiveDoc_Document, ExportStl)
      .Bind_Method(NaiveDoc_Document, ImportStep)
      .Bind_Method(NaiveDoc_Document, ImportStl)
      .Bind_Method(NaiveDoc_Document, Objects)
      .Bind_Method(NaiveDoc_Document, Redo)
      .Bind_Method(NaiveDoc_Document, Undo)
      .Bind_Method(NaiveDoc_Document, UpdateView)
      .End_Class()

      .Begin_Class(NaiveDoc_Object)
      .Bind_Property_Readonly(NaiveDoc_Object, Type)
      .Bind_Property_Readonly(NaiveDoc_Object, Signature)
      .End_Class()

      .End_Namespace()
      .End_Namespace();
}
