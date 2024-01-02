#include "util_curve.h"

#include <GeomLib.hxx>
#include <ShapeAnalysis_Curve.hxx>

namespace luaocct {
namespace util {
namespace Curve {

Handle(Geom_Curve) Duplicate(const Handle(Geom_Curve) & theCurve) {
  return Handle(Geom_Curve)::DownCast(theCurve->Copy());
}

Standard_Boolean IsClosed(const Handle(Geom_Curve) & theCurve,
                          const Standard_Real theTolerance) {
  return ShapeAnalysis_Curve::IsClosed(theCurve, theTolerance);
}

Standard_Real GetLength(const Handle(Geom_Curve) & theCurve,
                        const Standard_Real t0, const Standard_Real t1) {
  GeomAdaptor_Curve aCrv(theCurve);
  return CPnts_AbscissaPoint::Length(aCrv, t0, t1);
}

Standard_Real GetLength(const Handle(Geom_Curve) & theCurve) {
  return GetLength(theCurve, theCurve->FirstParameter(),
                   theCurve->LastParameter());
}

Standard_Boolean LengthParameter(const Handle(Geom_Curve) & theCurve,
                                 const Standard_Real theSegmentLength,
                                 Standard_Real &theT,
                                 const Standard_Real theTolerance) {
  Standard_Real aLength = GetLength(theCurve);

  if (theSegmentLength > aLength || theSegmentLength < 0.0 ||
      aLength < gp::Resolution()) {
    return Standard_False;
  }

  GeomAdaptor_Curve aCurve(theCurve);
  CPnts_AbscissaPoint aPnt{aCurve, theSegmentLength, aCurve.FirstParameter(),
                           theTolerance};

  if (!aPnt.IsDone()) {
    return Standard_False;
  }

  theT = aPnt.Parameter();

  return Standard_True;
}

Standard_Boolean NormalizedLengthParameter(const Handle(Geom_Curve) & theCurve,
                                           const Standard_Real theS,
                                           Standard_Real &theT,
                                           const Standard_Real theTolerance) {
  Standard_Real aSegmentLength = GetLength(theCurve) * theS;
  return LengthParameter(theCurve, aSegmentLength, theT, theTolerance);
}

Standard_Boolean PointAtNormalizedLength(const Handle(Geom_Curve) & theCurve,
                                         const Standard_Real theS,
                                         gp_Pnt &thePnt,
                                         const Standard_Real theTolerance) {
  Standard_Real t;

  if (!NormalizedLengthParameter(theCurve, theS, t, theTolerance))
    return Standard_False;

  thePnt = theCurve->Value(t);

  return Standard_True;
}

static Handle(Geom_Curve)
    extend(const Handle(Geom_Curve) & theCurve, CurveEnd theSide,
           const Standard_Real theLength, const Standard_Integer theCont) {
  if (theLength < gp::Resolution()) {
    return Duplicate(theCurve);
  }

  Standard_Real t = theSide == CurveEnd::Start ? theCurve->FirstParameter()
                                               : theCurve->LastParameter();

  gp_Pnt pntEnd;
  gp_Vec dirEnd;
  theCurve->D1(t, pntEnd, dirEnd);
  dirEnd.Normalize();

  if (theSide == CurveEnd::Start)
    dirEnd.Reverse();

  gp_Pnt target = pntEnd.XYZ() + dirEnd.XYZ() * theLength;
  auto aCurve = Handle(Geom_BoundedCurve)::DownCast(theCurve->Copy());

  if (aCurve.IsNull()) {
    return Duplicate(aCurve);
  } else {
    GeomLib::ExtendCurveToPoint(aCurve, target, 1, theSide == CurveEnd::End);
    return aCurve;
  }
}

Handle(Geom_Curve)
    Extend(const Handle(Geom_Curve) & theCurve, CurveEnd theSide,
           const Standard_Real theLength, const Standard_Integer theCont) {
  if (IsClosed(theCurve)) {
    return Duplicate(theCurve);
  }

  switch (theSide) {
  case CurveEnd::None:
    return nullptr;

  case CurveEnd::Both: {
    Handle(Geom_Curve) aCrv =
        extend(theCurve, CurveEnd::Start, theLength, theCont);
    return extend(aCrv, CurveEnd::End, theLength, theCont);
  }

  default:
    return extend(theCurve, theSide, theLength, theCont);
  }
}

Handle(Geom_TrimmedCurve)
    Trim(const Handle(Geom_Curve) & theCurve, const Standard_Real theT0,
         const Standard_Real theT1) {
  if (theT1 - theT0 <= Precision::Confusion() ||
      theT0 < theCurve->FirstParameter() || theT1 > theCurve->LastParameter()) {
    return nullptr;
  }

  return new Geom_TrimmedCurve(theCurve, theT0, theT1);
}

static Handle(Geom_TrimmedCurve)
    trimByLength(const Handle(Geom_Curve) & theCurve, CurveEnd theSide,
                 const Standard_Real theLength) {
  GeomAdaptor_Curve aCurve(theCurve);
  Standard_Real abscissa = theSide == CurveEnd::Start
                               ? theLength
                               : (GetLength(theCurve) - theLength);
  GCPnts_AbscissaPoint aPnt{aCurve, abscissa, aCurve.FirstParameter()};

  if (!aPnt.IsDone())
    return nullptr;

  Standard_Real t = aPnt.Parameter();

  if (theSide == CurveEnd::Start) {
    return Trim(theCurve, t, theCurve->LastParameter());
  } else {
    return Trim(theCurve, theCurve->FirstParameter(), t);
  }
}

Handle(Geom_TrimmedCurve)
    TrimByLength(const Handle(Geom_Curve) & theCurve, CurveEnd theSide,
                 const Standard_Real theLength) {
  switch (theSide) {
  case CurveEnd::None:
    return nullptr;

  case CurveEnd::Both: {
    auto aCrv = trimByLength(theCurve, CurveEnd::Start, theLength);
    return trimByLength(aCrv, CurveEnd::End, theLength);
  }

  default:
    return trimByLength(theCurve, theSide, theLength);
  }
}

} // namespace Curve
} // namespace util
} // namespace luaocct
