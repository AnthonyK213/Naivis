#ifndef _LuaOCCT_util_curve_Header
#define _LuaOCCT_util_curve_Header

#include <GCPnts_AbscissaPoint.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <Geom_BoundedCurve.hxx>
#include <Geom_Curve.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <TColGeom_Array1OfCurve.hxx>

namespace luaocct {
namespace util {
namespace Curve {

enum class CurveEnd : Standard_Integer { None, Start, End, Both };

Handle(Geom_Curve) Duplicate(const Handle(Geom_Curve) & theCurve);

Standard_Boolean
IsClosed(const Handle(Geom_Curve) & theCurve,
         const Standard_Real theTolerance = Precision::Confusion());

Standard_Real GetLength(const Handle(Geom_Curve) & theCurve,
                        const Standard_Real t0, const Standard_Real t1);

Standard_Real GetLength(const Handle(Geom_Curve) & theCurve);

Standard_Boolean
LengthParameter(const Handle(Geom_Curve) & theCurve,
                const Standard_Real theSegmentLength, Standard_Real &theT,
                const Standard_Real theTolerance = Precision::Confusion());

Standard_Boolean NormalizedLengthParameter(
    const Handle(Geom_Curve) & theCurve, const Standard_Real theS,
    Standard_Real &theT,
    const Standard_Real theTolerance = Precision::Confusion());

Standard_Boolean PointAtNormalizedLength(
    const Handle(Geom_Curve) & theCurve, const Standard_Real theS,
    gp_Pnt &thePnt, const Standard_Real theTolerance = Precision::Confusion());

Handle(Geom_Curve)
    Extend(const Handle(Geom_Curve) & theCurve, CurveEnd theSide,
           const Standard_Real theLength, const Standard_Integer theCont);

Handle(Geom_TrimmedCurve)
    Trim(const Handle(Geom_Curve) & theCurve, const Standard_Real theT0,
         const Standard_Real theT1);

Handle(Geom_TrimmedCurve)
    TrimByLength(const Handle(Geom_Curve) & theCurve, CurveEnd theSide,
                 const Standard_Real theLength);

Standard_Boolean
IsLinear(const Handle(Geom_Curve) & theCurve,
         const Standard_Real theTolerance = Precision::Confusion());

TColStd_Array1OfReal ClosestParameters(const Handle(Geom_Curve) & theCurve,
                                       const gp_Pnt &thePnt,
                                       const Standard_Boolean theExtremeOnly);

TColStd_Array1OfReal DivideByCount(const Handle(Geom_Curve) & theCurve,
                                   const Standard_Integer &theNbSegments,
                                   const Standard_Boolean theIncludeEnds);

TColGeom_Array1OfCurve Explode(const Handle(Geom_Curve) & theCurve);

} // namespace Curve
} // namespace util
} // namespace luaocct

#endif
