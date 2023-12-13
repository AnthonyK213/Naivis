#include "util.h"

namespace naivis {
namespace mesh {

MeshBuilder::MeshBuilder() {}

MeshBuilder::~MeshBuilder() {}

bool MeshBuilder::add(Handle(Poly_Triangulation) theMesh,
                      const TopLoc_Location &theLocation) {
  Standard_Integer nbNodes = static_cast<Standard_Integer>(myNodes.size());

  for (Standard_Integer i = 1; i <= theMesh->NbNodes(); ++i) {
    gp_Pnt aNode = theMesh->Node(i);
    aNode.Transform(theLocation);
    myNodes.push_back(aNode);
  }

  for (Standard_Integer i = 1; i <= theMesh->NbTriangles(); ++i) {
    const Poly_Triangle &triangle = theMesh->Triangle(i);
    myTriangles.push_back(
        {nbNodes + triangle(1), nbNodes + triangle(2), nbNodes + triangle(3)});
  }

  return true;
}

void MeshBuilder::clear() {
  myNodes.clear();
  myTriangles.clear();
}

Handle(Poly_Triangulation) MeshBuilder::mesh() const {
  if (myNodes.empty() || myTriangles.empty()) {
    return nullptr;
  }

  TColgp_Array1OfPnt aPoints{1, (int)myNodes.size()};
  Poly_Array1OfTriangle aTriangles{1, (int)myTriangles.size()};

  for (int i = 0; i < myNodes.size(); ++i) {
    aPoints.SetValue(i + 1, myNodes[i]);
  }

  for (int i = 0; i < myTriangles.size(); ++i) {
    aTriangles.SetValue(i + 1, myTriangles[i]);
  }

  return new Poly_Triangulation(aPoints, aTriangles);
}

void MeshBuilder::transform(const gp_Trsf &theT) {
  for (gp_Pnt &aNode : myNodes) {
    aNode.Transform(theT);
  }
}

Handle(Poly_Triangulation)
    shapeToMesh(const TopoDS_Shape &theShape,
                const IMeshTools_Parameters &theMeshParams,
                const TopLoc_Location &theLocation) {
  if (theShape.IsNull())
    return nullptr;

  MeshBuilder aMeshBuilder{};

  /// TopoDS_Face mesh backup, to preserve the triangulation for the view.
  std::vector<Poly_ListOfTriangulation> polysBackup{};
  std::vector<Handle(Poly_Triangulation)> activePolyBackup{};

  /// Backup the meshes then clear them, or the param may not take effect.
  for (TopExp_Explorer ex(theShape, TopAbs_FACE); ex.More(); ex.Next()) {
    TopoDS_Face face = TopoDS::Face(ex.Current());
    Handle(BRep_TFace) tFace = Handle(BRep_TFace)::DownCast(face.TShape());

    if (tFace.IsNull())
      continue;

    polysBackup.push_back(tFace->Triangulations());
    activePolyBackup.push_back(tFace->ActiveTriangulation());

    tFace->Triangulations(Poly_ListOfTriangulation(), nullptr);
  }

  BRepMesh_IncrementalMesh aMesher(theShape, theMeshParams);

  Standard_Integer faceIndex = 0;

  for (TopExp_Explorer ex(theShape, TopAbs_FACE); ex.More(); ex.Next()) {
    TopoDS_Face face = TopoDS::Face(ex.Current());

    TopLoc_Location loc;
    Handle(Poly_Triangulation) facing = BRep_Tool::Triangulation(face, loc);

    /// Recover the original mesh.
    Handle(BRep_TFace) tFace = Handle(BRep_TFace)::DownCast(face.TShape());

    if (!tFace.IsNull()) {
      tFace->Triangulations(polysBackup[faceIndex],
                            activePolyBackup[faceIndex]);
      faceIndex++;
    }

    if (facing.IsNull())
      continue;

    aMeshBuilder.add(facing, loc);
  }

  aMeshBuilder.transform(theLocation.Transformation());

  Handle(Poly_Triangulation) aMesh = aMeshBuilder.mesh();
  aMeshBuilder.clear();

  if (aMesh.IsNull())
    return nullptr;

  aMesh->ComputeNormals();

  return aMesh;
}

Handle(Poly_Triangulation) naivePoly3DToMesh(const Naive_Poly &thePoly) {
  const auto &vertices = thePoly.vertices();
  const auto &triangles = thePoly.triangles();

  if (vertices.empty() || triangles.empty()) {
    return nullptr;
  }

  TColgp_Array1OfPnt aPoints{1, (int)vertices.size()};
  Poly_Array1OfTriangle aTriangles{1, (int)triangles.size()};

  for (int i = 0; i < vertices.size(); ++i) {
    aPoints.SetValue(i + 1,
                     {vertices[i].x(), vertices[i].y(), vertices[i].z()});
  }

  for (int i = 0; i < thePoly.triangles().size(); ++i) {
    aTriangles.SetValue(i + 1, {triangles[i].x() + 1, triangles[i].y() + 1,
                                triangles[i].z() + 1});
  }

  return new Poly_Triangulation(aPoints, aTriangles);
}

Handle(MeshVS_Mesh) createMeshVS(const Handle(Poly_Triangulation) & theMesh) {
  if (theMesh.IsNull())
    return nullptr;

  Handle(XSDRAWSTLVRML_DataSource_1) aDataSource =
      new XSDRAWSTLVRML_DataSource_1(theMesh);

  Handle(MeshVS_Mesh) aMeshPrs = new MeshVS_Mesh();
  aMeshPrs->SetDataSource(aDataSource);

  Handle(MeshVS_MeshPrsBuilder) aBuilder = new MeshVS_MeshPrsBuilder(aMeshPrs);
  aMeshPrs->AddBuilder(aBuilder, true);

  Handle(TColStd_HPackedMapOfInteger) aMeshNodes =
      new TColStd_HPackedMapOfInteger();
  const Standard_Integer aLen = theMesh->NbNodes();

  for (Standard_Integer anIndex = 1; anIndex <= aLen; ++anIndex) {
    aMeshNodes->ChangeMap().Add(anIndex);
  }

  aMeshPrs->SetHiddenNodes(aMeshNodes);
  aMeshPrs->SetSelectableNodes(aMeshNodes);
  aMeshPrs->SetDisplayMode(MeshVS_DMF_Shading);

  return aMeshPrs;
}

} // namespace mesh
} // namespace naivis
