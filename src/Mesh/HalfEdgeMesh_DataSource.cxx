#include "HalfEdgeMesh_DataSource.hxx"

#include <Util/Util_Mesh.hxx>

#include <Precision.hxx>

IMPLEMENT_STANDARD_RTTIEXT(HalfEdgeMesh_DataSource, MeshVS_DataSource)

HalfEdgeMesh_DataSource::HalfEdgeMesh_DataSource(
    const Handle(Poly_Triangulation) & theMesh)
    : myIsValid(Standard_False) {
  if (theMesh.IsNull())
    return;

  Naive_H_Poly soup = Util_Mesh::MeshToNaivePoly3D(theMesh);

  if (!soup || !soup->IsValid())
    return;

  myMesh = std::make_shared<Naive_Mesh>(*soup);

  if (!myMesh->IsValid())
    return;

  Standard_Integer nbNodes =
      static_cast<Standard_Integer>(myMesh->NbVertices());
  Naive_Integer_List aVertices = myMesh->GetAllVertices();
  myNodeCoords = new TColStd_HArray2OfReal(1, nbNodes, 1, 3);

  Standard_Integer idxNode = 1;
  for (Standard_Integer item : aVertices) {
    myNodes.Add(item);
    const Naive_Point3d &xyz = myMesh->GetVertex(item)->Coord();

    myNodeCoords->SetValue(idxNode, 1, xyz.X());
    myNodeCoords->SetValue(idxNode, 2, xyz.Y());
    myNodeCoords->SetValue(idxNode, 3, xyz.Z());

    idxNode++;
  }

  Standard_Integer nbTris = static_cast<Standard_Integer>(myMesh->NbFaces());
  Naive_Integer_List aTriangles = myMesh->GetAllFaces();
  myElemNormals = new TColStd_HArray2OfReal(1, nbTris, 1, 3);
  myElemNodes = new TColStd_HArray2OfInteger(1, nbTris, 1, 3);

  Standard_Integer idxElem = 1;
  for (const auto &item : aTriangles) {
    myElements.Add(item);
    const auto *aFace = myMesh->GetFace(item);

    Naive_Point3d V[3];

    Standard_Integer idxEdge = 1;
    for (auto it = aFace->EdgeIter(); it.More(); it.Next(), ++idxEdge) {
      if (idxEdge > 3)
        return;

      const auto *aEdge = it.Current();
      myElemNodes->SetValue(idxElem, idxEdge, aEdge->Origin()->Id());
      V[idxEdge - 1] = aEdge->Origin()->Coord();
    }

    Naive_Vector3d aV1 = V[1] - V[0];
    Naive_Vector3d aV2 = V[2] - V[1];

    Naive_Vector3d aN = aV1.Crossed(aV2);
    if (aN.SquareLength() > Precision::SquareConfusion())
      aN.Normalize();
    else
      aN = {0., 0., 0.};

    myElemNormals->SetValue(idxElem, 1, aN.X());
    myElemNormals->SetValue(idxElem, 2, aN.Y());
    myElemNormals->SetValue(idxElem, 3, aN.Z());

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

Handle(Poly_Triangulation) HalfEdgeMesh_DataSource::GetTriangulation() const {
  if (!myIsValid)
    return nullptr;

  return Util_Mesh::NaivePoly3DToMesh(*myMesh->Soup());
}
