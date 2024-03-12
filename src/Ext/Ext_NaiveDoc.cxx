#include <AIS_ColoredShape.hxx>
#include <AIS_Shape.hxx>
#include <AIS_TexturedShape.hxx>
#include <MeshVS_Mesh.hxx>
#include <XCAFPrs_AISObject.hxx>
#include <XCAFPrs_DocumentExplorer.hxx>

#include "Ext_NaiveDoc.hxx"
#include <NaiveDoc/NaiveDoc_Document.hxx>
#include <NaiveDoc/NaiveDoc_Object.hxx>

class NaiveDoc_DocumentExplorer : public Standard_Transient,
                                  public XCAFPrs_DocumentExplorer {
public:
  using XCAFPrs_DocumentExplorer::XCAFPrs_DocumentExplorer;
};

void Ext_NaiveDoc(lua_State *L) {
  LuaBridge__G(L)
      .Begin_Namespace(Naivis)
      .Begin_Namespace(NaiveDoc)

      .Begin_Derive(NaiveDoc_Document, LODoc_Document)
      .addConstructorFrom<Handle(NaiveDoc_Document), void()>()
      .End_Derive()

      .Begin_Class(NaiveDoc_Object)
      .Bind_Property_Readonly(NaiveDoc_Object, Type)
      .Bind_Property_Readonly(NaiveDoc_Object, Signature)
      .End_Class()

      .End_Namespace()
      .End_Namespace();
}
