#include "Util_Mesh.hxx"

#include <BRepMesh_IncrementalMesh.hxx>
#include <BRep_TFace.hxx>
#include <BRep_Tool.hxx>
#include <MeshVS_Drawer.hxx>
#include <MeshVS_DrawerAttribute.hxx>
#include <MeshVS_ElementalColorPrsBuilder.hxx>
#include <MeshVS_MeshPrsBuilder.hxx>
#include <TColStd_HPackedMapOfInteger.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>

#include <luaocct/LOUtil_Shape.hxx>

namespace Util_Mesh {

Handle(Poly_Triangulation) NaivePoly3DToMesh(const Naive_Poly &thePoly) {
  const auto &vertices = thePoly.Vertices();
  const auto &triangles = thePoly.Triangles();

  if (vertices.empty() || triangles.empty()) {
    return nullptr;
  }

  TColgp_Array1OfPnt aPoints{1, (Standard_Integer)vertices.size()};
  Poly_Array1OfTriangle aTriangles{1, (Standard_Integer)triangles.size()};

  for (Standard_Integer i = 0; i < vertices.size(); ++i) {
    aPoints.SetValue(i + 1,
                     {vertices[i].x(), vertices[i].y(), vertices[i].z()});
  }

  for (Standard_Integer i = 0; i < thePoly.Triangles().size(); ++i) {
    aTriangles.SetValue(i + 1, {triangles[i].x() + 1, triangles[i].y() + 1,
                                triangles[i].z() + 1});
  }

  return new Poly_Triangulation(aPoints, aTriangles);
}

Naive_H_Poly MeshToNaivePoly3D(const Handle(Poly_Triangulation) & theMesh) {
  if (theMesh.IsNull())
    return nullptr;

  const Standard_Integer nbNodes = theMesh->NbNodes();
  const Standard_Integer nbTris = theMesh->NbTriangles();
  Naive_Point3d_List vertices{};
  Naive_List<Naive_Triangle> triangles{};
  vertices.reserve(nbNodes);
  triangles.reserve(nbTris);

  for (Standard_Integer i = 1; i <= nbNodes; ++i) {
    gp_Pnt aNode = theMesh->Node(i);
    vertices.push_back({aNode.X(), aNode.Y(), aNode.Z()});
  }

  for (Standard_Integer i = 1; i <= nbTris; ++i) {
    const Poly_Triangle &aTri = theMesh->Triangle(i);
    /// TraingleSoup index starts from 0!
    triangles.push_back({aTri(1) - 1, aTri(2) - 1, aTri(3) - 1});
  }

  return std::make_shared<Naive_Poly>(std::move(vertices),
                                      std::move(triangles));
}

Handle(MeshVS_Mesh) CreateMeshVS(const Handle(Poly_Triangulation) & theMesh) {
  if (theMesh.IsNull())
    return nullptr;

  Handle(XSDRAWSTLVRML_DataSource_1) aDataSource =
      new XSDRAWSTLVRML_DataSource_1(theMesh);

  Handle(MeshVS_Mesh) aMeshPrs = new MeshVS_Mesh();
  aMeshPrs->SetDataSource(aDataSource);

  Handle(MeshVS_ElementalColorPrsBuilder) aColorPrsBuilder =
      new MeshVS_ElementalColorPrsBuilder(
          aMeshPrs, MeshVS_DMF_Shading | MeshVS_DMF_ElementalColorDataPrs,
          aDataSource);

  for (TColStd_PackedMapOfInteger::Iterator anIter(
           aDataSource->GetAllElements());
       anIter.More(); anIter.Next()) {
    aColorPrsBuilder->SetColor1(
        anIter.Key(), Quantity_Color(0.5, 0.8, 0.88, Quantity_TOC_sRGB));
  }

  aMeshPrs->AddBuilder(aColorPrsBuilder, Standard_False);

  Handle(MeshVS_MeshPrsBuilder) aPrsBuilder = new MeshVS_MeshPrsBuilder(
      aMeshPrs, MeshVS_DMF_WireFrame, aDataSource, -1, MeshVS_BP_Default);
  aMeshPrs->AddBuilder(aPrsBuilder, Standard_True);

  aMeshPrs->GetDrawer()->SetBoolean(MeshVS_DA_ShowEdges, Standard_False);
  aMeshPrs->GetDrawer()->SetBoolean(MeshVS_DA_Reflection, Standard_True);
  aMeshPrs->GetDrawer()->SetBoolean(MeshVS_DA_SmoothShading, Standard_True);
  aMeshPrs->GetDrawer()->SetBoolean(MeshVS_DA_DisplayNodes, Standard_False);
  aMeshPrs->GetDrawer()->SetBoolean(MeshVS_DA_ColorReflection, Standard_True);

  Handle(TColStd_HPackedMapOfInteger) aMeshNodes =
      new TColStd_HPackedMapOfInteger();
  const Standard_Integer aLen = theMesh->NbNodes();

  for (Standard_Integer anIndex = 1; anIndex <= aLen; ++anIndex) {
    aMeshNodes->ChangeMap().Add(anIndex);
  }

  aMeshPrs->SetHiddenNodes(aMeshNodes);
  aMeshPrs->SetSelectableNodes(aMeshNodes);
  aMeshPrs->SetDisplayMode(MeshVS_DMF_Shading);
  aMeshPrs->SetMeshSelMethod(MeshVS_MSM_PRECISE);
  aMeshPrs->HilightAttributes()->SetColor(Quantity_NOC_YELLOW1);

  return aMeshPrs;
}

} // namespace Util_Mesh
