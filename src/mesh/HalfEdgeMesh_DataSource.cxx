#include "HalfEdgeMesh_DataSource.hxx"

#include <Precision.hxx>

IMPLEMENT_STANDARD_RTTIEXT(HalfEdgeMesh_DataSource, MeshVS_DataSource)

HalfEdgeMesh_DataSource::HalfEdgeMesh_DataSource(
    const Handle(Poly_Triangulation) & aMesh)
    : myIsValid(Standard_False) {
  if (aMesh.IsNull())
    return;

  myTriangulation = aMesh;

  const Standard_Integer aNbNodes = aMesh->NbNodes();
  const Standard_Integer aNbTris = aMesh->NbTriangles();

  std::vector<Naive_Vector3d> vertices{};
  std::vector<Naive_Vector3i> triangles{};

  vertices.reserve(aNbNodes);
  triangles.reserve(aNbTris);

  for (Standard_Integer i = 1; i <= aNbNodes; ++i) {
    gp_Pnt aNode = aMesh->Node(i);
    vertices.push_back({aNode.X(), aNode.Y(), aNode.Z()});
  }

  for (Standard_Integer i = 1; i <= aNbTris; ++i) {
    const Poly_Triangle &aTri = aMesh->Triangle(i);
    /// TraingleSoup index starts from 0!
    triangles.push_back({aTri(1) - 1, aTri(2) - 1, aTri(3) - 1});
  }

  naivecgl::shape::TriangleSoup soup{std::move(vertices), std::move(triangles)};

  if (!soup.isValid())
    return;

  myMesh = std::make_shared<HalfEdgeMesh_Mesh>(soup);

  myNodeCoords = new TColStd_HArray2OfReal(1, aNbNodes, 1, 3);

  Standard_Integer idxNode = 1;
  for (const auto &item : myMesh->vertices()) {
    myNodes.Add(item.first);
    const auto &xyz = item.second.coordinates();

    myNodeCoords->SetValue(idxNode, 1, xyz(0));
    myNodeCoords->SetValue(idxNode, 2, xyz(1));
    myNodeCoords->SetValue(idxNode, 3, xyz(2));

    idxNode++;
  }

  myElemNormals = new TColStd_HArray2OfReal(1, aNbTris, 1, 3);
  myElemNodes = new TColStd_HArray2OfInteger(1, aNbTris, 1, 3);

  Standard_Integer idxElem = 1;
  for (const auto &item : myMesh->faces()) {
    myElements.Add(item.first);

    const auto &aFace = item.second;

    Naive_Vector3d V[3];

    Standard_Integer idxEdge = 1;
    for (auto it = aFace.edgeIter(); it.more(); it.next(), ++idxEdge) {
      if (idxEdge > 3)
        return;

      const auto *aEdge = it.current();
      myElemNodes->SetValue(idxElem, idxEdge, aEdge->origin()->id());

      V[idxEdge - 1] = aEdge->origin()->coordinates();
    }

    Naive_Vector3d aV1 = V[1] - V[0];
    Naive_Vector3d aV2 = V[2] - V[1];

    Naive_Vector3d aN = aV1.cross(aV2);
    if (aN.squaredNorm() > Precision::SquareConfusion())
      aN.normalize();
    else
      aN = {0., 0., 0.};

    myElemNormals->SetValue(idxElem, 1, aN(0));
    myElemNormals->SetValue(idxElem, 2, aN(1));
    myElemNormals->SetValue(idxElem, 3, aN(2));

    idxElem++;
  }

  myIsValid = Standard_True;
}

Standard_Boolean HalfEdgeMesh_DataSource::GetGeom(
    const Standard_Integer ID, const Standard_Boolean IsElement,
    TColStd_Array1OfReal &Coords, Standard_Integer &NbNodes,
    MeshVS_EntityType &Type) const {
  if (!myIsValid)
    return Standard_False;

  if (IsElement) {
    if (ID >= 1 && ID <= myElements.Extent()) {
      Type = MeshVS_ET_Face;
      NbNodes = 3;

      for (Standard_Integer i = 1, k = 1; i <= 3; ++i) {
        Standard_Integer IdxNode = myElemNodes->Value(ID, i);
        for (Standard_Integer j = 1; j <= 3; ++j, ++k) {
          Coords(k) = myNodeCoords->Value(IdxNode, j);
        }
      }

      return Standard_True;
    } else {
      return Standard_False;
    }
  } else if (ID >= 1 && ID <= myNodes.Extent()) {
    Type = MeshVS_ET_Node;
    NbNodes = 1;

    Coords(1) = myNodeCoords->Value(ID, 1);
    Coords(2) = myNodeCoords->Value(ID, 2);
    Coords(3) = myNodeCoords->Value(ID, 3);

    return Standard_True;
  } else {
    return Standard_False;
  }
}

Standard_Boolean
HalfEdgeMesh_DataSource::GetGeomType(const Standard_Integer,
                                     const Standard_Boolean IsElement,
                                     MeshVS_EntityType &Type) const {
  if (IsElement) {
    Type = MeshVS_ET_Face;
    return Standard_True;
  } else {
    Type = MeshVS_ET_Node;
    return Standard_True;
  }
}

Standard_Address
HalfEdgeMesh_DataSource::GetAddr(const Standard_Integer,
                                 const Standard_Boolean) const {
  return NULL;
}

Standard_Boolean HalfEdgeMesh_DataSource::GetNodesByElement(
    const Standard_Integer ID, TColStd_Array1OfInteger &theNodeIDs,
    Standard_Integer & /*theNbNodes*/) const {
  if (!myIsValid)
    return Standard_False;

  if (ID >= 1 && ID <= myElements.Extent() && theNodeIDs.Length() >= 3) {
    Standard_Integer aLow = theNodeIDs.Lower();
    theNodeIDs(aLow) = myElemNodes->Value(ID, 1);
    theNodeIDs(aLow + 1) = myElemNodes->Value(ID, 2);
    theNodeIDs(aLow + 2) = myElemNodes->Value(ID, 3);
    return Standard_True;
  }
  return Standard_False;
}

const TColStd_PackedMapOfInteger &HalfEdgeMesh_DataSource::GetAllNodes() const {
  return myNodes;
}

const TColStd_PackedMapOfInteger &
HalfEdgeMesh_DataSource::GetAllElements() const {
  return myElements;
}

Standard_Boolean HalfEdgeMesh_DataSource::GetNormal(const Standard_Integer Id,
                                                    const Standard_Integer Max,
                                                    Standard_Real &nx,
                                                    Standard_Real &ny,
                                                    Standard_Real &nz) const {
  if (!myIsValid)
    return Standard_False;

  if (Id >= 1 && Id <= myElements.Extent() && Max >= 3) {
    nx = myElemNormals->Value(Id, 1);
    ny = myElemNormals->Value(Id, 2);
    nz = myElemNormals->Value(Id, 3);
    return Standard_True;
  } else
    return Standard_False;
}
