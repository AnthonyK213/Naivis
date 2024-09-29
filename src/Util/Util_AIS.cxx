#include "Util_AIS.hxx"

#include <AIS_PointCloud.hxx>
#include <AIS_Shape.hxx>
#include <BRepLib.hxx>
#include <BRep_Tool.hxx>
#include <Geom_Circle.hxx>
#include <Geom_Curve.hxx>
#include <Geom_Line.hxx>
#include <MeshVS_DataSource.hxx>
#include <MeshVS_Mesh.hxx>
#include <TDF_AttributeIterator.hxx>
#include <TDataStd_Integer.hxx>
#include <TopoDS.hxx>

#include <NaiveDoc/NaiveDoc_Attribute.hxx>
#include <NaiveDoc/NaiveDoc_Object.hxx>

#include <luaocct/LOUtil_AIS.hxx>

#include <QVariant>

namespace Util_AIS {

QStringList GetObjectProperties(const Handle(AIS_InteractiveObject) & theObj,
                                const QVariantHash &theExtra) {
  QStringList aProps{};

  if (theObj.IsNull())
    return aProps;

  aProps.push_back("Name");
  TCollection_AsciiString aName = NaiveDoc_Attribute::GetName(theObj);
  aProps.push_back(aName.ToCString());

  aProps.push_back("Type");

  if (theObj->IsKind(STANDARD_TYPE(AIS_Shape))) {
    auto anShape = Handle(AIS_Shape)::DownCast(theObj)->Shape();
    TopAbs_ShapeEnum anShapeType = anShape.ShapeType();

    switch (anShapeType) {
    case TopAbs_COMPOUND: {
      aProps.push_back("Compound");
      break;
    }

    case TopAbs_COMPSOLID: {
      aProps.push_back("CompSolid");
      break;
    }

    case TopAbs_SOLID: {
      aProps.push_back("Solid");
      break;
    }

    case TopAbs_SHELL: {
      aProps.push_back("Shell");
      break;
    }

    case TopAbs_FACE: {
      aProps.push_back("Face");
      break;
    }

    case TopAbs_WIRE: {
      aProps.push_back("Wire");
      break;
    }

    case TopAbs_EDGE: {
      const TopoDS_Edge &anEdge = TopoDS::Edge(anShape);
      if (!BRepLib::BuildCurve3d(anEdge)) {
        aProps.push_back("Invalid edge");
        break;
      }
      Standard_Real s, e;
      Handle(Geom_Curve) aCurve = BRep_Tool::Curve(anEdge, s, e);

      auto aCrvType = aCurve->DynamicType();

      if (aCrvType == STANDARD_TYPE(Geom_Line)) {
        aProps.push_back("Line");
      } else if (aCrvType == STANDARD_TYPE(Geom_Circle)) {
        aProps.push_back("Circle");
        auto aCircle = Handle(Geom_Circle)::DownCast(aCurve);
        const gp_Pnt &anOrg = aCircle->Location();
        aProps.push_back("Center");
        aProps.push_back(QString("{%1, %2, %3}")
                             .arg(anOrg.X())
                             .arg(anOrg.Y())
                             .arg(anOrg.Z()));
        aProps.push_back("Radius");
        aProps.push_back(QString::number(aCircle->Radius()));
      } else {
        aProps.push_back("Edge");
      }

      break;
    }

    case TopAbs_VERTEX: {
      aProps.push_back("Point");
      gp_Pnt aPnt = BRep_Tool::Pnt(TopoDS::Vertex(anShape));
      aProps.push_back("X");
      aProps.push_back(QString::number(aPnt.X()));
      aProps.push_back("Y");
      aProps.push_back(QString::number(aPnt.Y()));
      aProps.push_back("Z");
      aProps.push_back(QString::number(aPnt.Z()));
      break;
    }

    default: {
      aProps.push_back("Shape");
      break;
    }
    }
  } else if (theObj->IsKind(STANDARD_TYPE(MeshVS_Mesh))) {
    aProps.push_back("Mesh");
    auto aMesh = Handle(MeshVS_Mesh)::DownCast(theObj);
    Handle(MeshVS_DataSource) aDataSource = aMesh->GetDataSource();
    Standard_Integer nbNodes = aDataSource->GetAllNodes().Extent();
    Standard_Integer nbElems = aDataSource->GetAllElements().Extent();
    aProps.push_back("Vertices");
    aProps.push_back(QString::number(nbNodes));
    aProps.push_back("Faces");
    aProps.push_back(QString::number(nbElems));
  } else if (theObj->IsKind(STANDARD_TYPE(AIS_PointCloud))) {
    aProps.push_back("Point Cloud");
    auto aPoints = Handle(AIS_PointCloud)::DownCast(theObj);
    Standard_Integer nbPoints = aPoints->GetPoints()->VertexNumber();
    aProps.push_back("Count");
    aProps.push_back(QString::number(nbPoints));
  } else {
    aProps.push_back("UNKNOWN");
  }

  NaiveDoc_Id anId = NaiveDoc_Attribute::GetId(theObj);
  if (!anId.IsNull() && anId.HasAttribute()) {
    for (TDF_AttributeIterator anIter(anId); anIter.More(); anIter.Next()) {
      Handle(TDF_Attribute) anAttr = anIter.Value();
      const Standard_GUID &aGuid = anAttr->ID();
      char anAttrId[37];
      aGuid.ToCString(anAttrId);
      QString anIdStr = anAttrId;
      if (!theExtra.contains(anIdStr))
        continue;
      QString anIdName = theExtra[anIdStr].toString();
      aProps.push_back(anIdName);

      /* TODO: `ToString` for other attribute types. */
      if (anAttr->IsKind(STANDARD_TYPE(TDataStd_Integer))) {
        auto anIntAttr = Handle(TDataStd_Integer)::DownCast(anAttr);
        aProps.push_back(QString::number(anIntAttr->Get()));
      }
    }
  }

  return aProps;
}

NaiveDoc_ObjectList GetSelections(const Handle(AIS_InteractiveContext) &
                                  theCtx) {
  return LOUtil_AIS::GetSelections(theCtx);
}

} /* namespace Util_AIS */
