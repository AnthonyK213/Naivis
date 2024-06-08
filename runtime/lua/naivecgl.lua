local naivecgl = {}
naivecgl.bndshape = {}
naivecgl.tessellation = {}

--------------------------------------------------------------------------------
--                                  FFI                                       --
--------------------------------------------------------------------------------

local ffi = require("ffi")

---@type ffi.namespace*
naivecgl.NS = nil

local function getDylibPath(theName)
  if jit.os == "Windows" then
    return theName .. ".dll"
  elseif jit.os == "Linux" then
    return "lib" .. theName .. ".so"
  else
    error("Unsupported system!")
  end
end

---@private
function naivecgl:init()
  if self.NS then
    return
  end

  -- NaiveCGL_c_types.h
  ffi.cdef [[
typedef enum {
  Naive_Code_ok = 0,
  Naive_Code_err,
  Naive_Code_not_implemented,
  Naive_Code_initialized = 1000,
  Naive_Code_null_exception = 1500,
  Naive_Code_invalid_value,
  Naive_Code_invalid_handle,
  Naive_Code_no_intersection,
  Naive_Code_points_are_collinear = 2000,
  Naive_Code_points_are_coplanar,
  Naive_Code_index_out_of_range,
  Naive_Code_value_out_of_range,
  Naive_Code_insufficient_points,
  Naive_Code_insufficient_knots,
  Naive_Code_zero_interval,
  Naive_Code_periodic_open,
  Naive_Code_periodic_not_smooth,
  Naive_Code_cant_make_nurbs,
  Naive_Code_weight_le_0,
  Naive_Code_bad_knots,
  Naive_Code_poles_weights_not_match,
  Naive_Code_knots_mults_not_match,
  Naive_Code_invalid_mults,
} Naive_Code;

typedef enum {
  Naive_Algorithm_quick_hull = 0,
  Naive_Algorithm_incremental,
  Naive_Algorithm_graham_scan,
  Naive_Algorithm_divide_and_conquer,
} Naive_Algorithm;

/* Naive_H */

typedef void *Naive_Handle_t;

/* Naive_Code_t */

typedef int Naive_Code_t;

/* Naive_Logical_t */

typedef unsigned char Naive_Logical_t;

/* Naive_XY_t */

typedef struct {
  double x, y;
} Naive_XY_t;

/* Naive_XYZ_t */

typedef struct {
  double x, y, z;
} Naive_XYZ_t;

/* Naive_Vector2d_t */

typedef Naive_XY_t Naive_Vector2d_t;

/* Naive_Point2d_t */

typedef Naive_XY_t Naive_Point2d_t;

/* Naive_Vector3d_t */

typedef Naive_XYZ_t Naive_Vector3d_t;

/* Naive_Point3d_t */

typedef Naive_XYZ_t Naive_Point3d_t;

/* Naive_Interval_t */

typedef struct {
  double t0, t1;
} Naive_Interval_t;

/* Naive_Triangle_t */

typedef struct {
  int n0, n1, n2;
} Naive_Triangle_t;

/* Naive_Axis1_t */

typedef struct {
  Naive_Point3d_t location;
  Naive_Vector3d_t axis;
} Naive_Axis1_t;

/* Naive_Axis2_t */

typedef struct {
  Naive_Point3d_t location;
  Naive_Vector3d_t axis;
  Naive_Vector3d_t ref_direction;
} Naive_Axis2_t;

/* Naive_Line_t */

typedef struct {
  Naive_Axis1_t basis_set;
} Naive_Line_t;

/* Naive_Plane_t */

typedef struct {
  Naive_Axis2_t basis_set;
} Naive_Plane_t;
]]

  -- NaiveCGL_c.h
  ffi.cdef [[
/* Naive_Math_Constant */

double Naive_Math_Constant_pi(void);

double Naive_Math_Constant_epsilon(void);

double Naive_Math_Constant_min_real(void);

double Naive_Math_Constant_max_real(void);

double Naive_Math_Constant_unset_real(void);

/* Naive_Math_Util */

Naive_Logical_t
Naive_Math_Util_is_valid_real(const double /* real */);

/* Naive_Transient */

Naive_Code_t Naive_Transient_release(Naive_Handle_t /* handle */);

/* Naive_Geometry */

Naive_Code_t Naive_Geometry_is_valid(
    const Naive_Handle_t /* handle */, Naive_Logical_t *const /* is_valid */);

Naive_Code_t Naive_Geometry_clone(const Naive_Handle_t /* handle */,
                                            Naive_Handle_t *const /* clone */);

/* Naive_Plane */

Naive_Code_t Naive_Plane_new(const Naive_Plane_t * /* plane_sf */,
                                       Naive_Handle_t *const /* plane */);

Naive_Code_t Naive_Plane_ask(const Naive_Handle_t /* handle */,
                                       Naive_Plane_t *const /* plane_sf */);

Naive_Code_t Naive_Plane_distance(const Naive_Handle_t /* handle */,
                                            const Naive_Point3d_t * /* point */,
                                            double *const /* distance */);

/* Naive_Line */

Naive_Code_t Naive_Line_new(const Naive_Line_t * /* line_sf */,
                                      Naive_Handle_t *const /* line */);

/* Naive_NurbsCurve */

Naive_Code_t Naive_NurbsCurve_new(
    const int /* n_poles */, const Naive_Point3d_t * /* poles */,
    const int /* n_weights */, const double * /* weights */,
    const int /* n_knots */, const double * /* knots */,
    const int /* n_mults */, const int * /* mults */, const int /* degree */,
    Naive_Handle_t *const /* nurbs_curve */);

Naive_Code_t Naive_NurbsCurve_ask_degree(
    const Naive_Handle_t /* handle */, int *const /* degree */);

Naive_Code_t Naive_NurbsCurve_ask_poles(
    const Naive_Handle_t /* handle */, int *const /* n_poles */,
    Naive_Point3d_t *const /* poles */);

Naive_Code_t Naive_NurbsCurve_ask_weights(
    const Naive_Handle_t /* handle */, int *const /* n_weights */,
    double *const /* weights */);

Naive_Code_t
Naive_NurbsCurve_ask_knots(const Naive_Handle_t /* handle */,
                           int *const /* n_knots */, double *const /* knots */);

Naive_Code_t
Naive_NurbsCurve_ask_mults(const Naive_Handle_t /* handle */,
                           int *const /* n_mults */, int *const /* mults */);

Naive_Code_t Naive_NurbsCurve_ask_bound(
    const Naive_Handle_t /* handle */, Naive_Interval_t *const /* bound */);

Naive_Code_t Naive_NurbsCurve_evaluate(
    const Naive_Handle_t /* handle */, const double /* t */,
    const int /* n_derivative */, int *const /* n_result */,
    Naive_Vector3d_t *const /* result */);

Naive_Code_t Naive_NurbsCurve_curvature_at(
    const Naive_Handle_t /* handle */, const double /* t */,
    Naive_Vector3d_t *const /* curvature */);

Naive_Code_t Naive_NurbsCurve_increase_degree(
    Naive_Handle_t /* handle */, const int /* degree */);

Naive_Code_t Naive_NurbsCurve_increase_multiplicity(
    Naive_Handle_t /* handle */, const int /* index */, const int /* mult */);

Naive_Code_t Naive_NurbsCurve_insert_knot(Naive_Handle_t /* handle */,
                                                    const double /* t */,
                                                    const int /* mult */);

/* Reduces the multiplicity of the knot of index |index| to |mult|. */
Naive_Code_t Naive_NurbsCurve_remove_knot(Naive_Handle_t /* handle */,
                                                    const int /* index */,
                                                    const int /* mult */);

/* Naive_NurbsSurface */

Naive_Code_t Naive_NurbsSurface_new(
    const int /* n_poles_u */, const int /* n_poles_v */,
    const Naive_Point3d_t * /* poles */, const int /* n_weights_u */,
    const int /* n_weights_v */, const double * /* weights */,
    const int /* n_knots_u */, const double * /* knots_u */,
    const int /* n_knots_v */, const double * /* knots_v */,
    const int /* n_mults_u */, const int * /* mults_u */,
    const int /* n_mults_v */, const int * /* mults_v */,
    const int /* degree_u */, const int /* degree_v */,
    Naive_Handle_t *const /* nurbs_surface */);

Naive_Code_t Naive_NurbsSurface_ask_degree(
    const Naive_Handle_t /* handle */, int *const /* degree_u */,
    int *const /* degree_v */);

Naive_Code_t Naive_NurbsSurface_evaluate(
    const Naive_Handle_t /* handle */, const double /* u */,
    const double /* v */, int /* n_derivative */, int *const /* n_result */,
    Naive_Vector3d_t *const /* result */);

/* Naive_Poly */

Naive_Code_t Naive_Poly_new(const int /* n_vertices */,
                                      const Naive_Point3d_t * /* vertices */,
                                      const int /* n_triangles */,
                                      const Naive_Triangle_t * /* triangles */,
                                      Naive_Handle_t *const /* poly */);

Naive_Code_t Naive_Poly_is_valid(
    const Naive_Handle_t /* handle */, Naive_Logical_t *const /* is_valid */);

Naive_Code_t Naive_Poly_clone(const Naive_Handle_t /* handle */,
                                        Naive_Handle_t *const /* clone */);

Naive_Code_t Naive_Poly_ask_vertices(
    const Naive_Handle_t /* handle */, int *const /* n_vertices */,
    Naive_Point3d_t *const /* vertices */);

Naive_Code_t Naive_Poly_ask_triangles(
    const Naive_Handle_t /* handle */, int *const /* n_triangles */,
    Naive_Triangle_t *const /* triangles */);

/* Naive_BndShape_ConvexHull2D */

Naive_Code_t Naive_BndShape_ConvexHull2D_new(
    int /* n_points */, const Naive_Point2d_t * /* points */,
    Naive_Algorithm /* algo */, Naive_Handle_t *const /* covex_hull_2d */);

Naive_Code_t Naive_BndShape_ConvexHull2D_set_algorithm(
    Naive_Handle_t /* handle */, Naive_Algorithm /* algo */);

Naive_Code_t
    Naive_BndShape_ConvexHull2D_perform(Naive_Handle_t /* handle */);

Naive_Code_t Naive_BndShape_ConvexHull2D_add_point(
    Naive_Handle_t /* handle */, Naive_Point2d_t /* point */,
    Naive_Logical_t /* to_perform */);

Naive_Code_t Naive_BndShape_ConvexHull2D_ask_result(
    const Naive_Handle_t /* handle */, int *const /* n_convex_points */,
    int *const /* convex_indices */,
    Naive_Point2d_t *const /* convex_points */);

/* Naive_BndShape_EnclosingDisc */

Naive_Code_t
Naive_BndShape_EnclosingDisc_new(Naive_Handle_t *const /* enclosing_disc */);

Naive_Code_t Naive_BndShape_EnclosingDisc_rebuild(
    Naive_Handle_t /* handle */, int /* n_points */,
    const Naive_Point2d_t * /* points */);

Naive_Code_t Naive_BndShape_EnclosingDisc_ask_circle(
    const Naive_Handle_t /* handle */, Naive_Point2d_t *const /* origin */,
    double *const /* radius */);

/* Naive_Intersect_Intersection */

Naive_Code_t Naive_Intersect_Intersection_line_plane(
    const Naive_Handle_t /* line */, const Naive_Handle_t /* plane */,
    double *const /* t_line */);

/* Naive_Tessellation */

Naive_Code_t Naive_Tessellation_tetrasphere(
    const Naive_Point3d_t * /* center */, const double /* radius */,
    const int /* level */, Naive_Handle_t *const /* poly */);
]]

  self.NS = ffi.load(getDylibPath("NaiveCGL"))

  if not self.NS then
    error("Failed to initialize NaiveCGL")
  end
end

--------------------------------------------------------------------------------
--                                Primitive                                   --
--------------------------------------------------------------------------------

---@class naivecgl.Naive_XY
---@field private myH ffi.cdata*
---@operator call:naivecgl.Naive_XY
naivecgl.Naive_XY = { myType = "Naive_XY_t" }

---@private
naivecgl.Naive_XY.__index = naivecgl.Naive_XY

---Constructor.
---@param theX number?
---@param theY number?
---@return naivecgl.Naive_XY
function naivecgl.Naive_XY.new(theX, theY)
  return naivecgl.Naive_XY.take(ffi.new(naivecgl.Naive_XY.myType, {
    x = theX or 0, y = theY or 0
  }))
end

setmetatable(naivecgl.Naive_XY, {
  __call = function(o, ...)
    return o.new(...)
  end
})

function naivecgl.Naive_XY.take(theH)
  local vec = { myH = theH }
  setmetatable(vec, naivecgl.Naive_XY)
  return vec
end

---
---@return number
function naivecgl.Naive_XY:X()
  return self.myH.x
end

---
---@return number
function naivecgl.Naive_XY:Y()
  return self.myH.y
end

---
---@return ffi.cdata*
function naivecgl.Naive_XY:Data()
  return self.myH
end

---@class naivecgl.Naive_XYZ
---@field private myH ffi.cdata*
---@operator call:naivecgl.Naive_XYZ
naivecgl.Naive_XYZ = { myType = "Naive_XYZ_t" }

---@private
naivecgl.Naive_XYZ.__index = naivecgl.Naive_XYZ

---Constructor.
---@param theX number?
---@param theY number?
---@param theZ number?
---@return naivecgl.Naive_XYZ
function naivecgl.Naive_XYZ.new(theX, theY, theZ)
  return naivecgl.Naive_XYZ.take(ffi.new(naivecgl.Naive_XYZ.myType, {
    x = theX or 0, y = theY or 0, z = theZ or 0
  }))
end

setmetatable(naivecgl.Naive_XYZ, {
  __call = function(o, ...)
    return o.new(...)
  end
})

---
---@param theH ffi.cdata*
---@return naivecgl.Naive_XYZ
function naivecgl.Naive_XYZ.take(theH)
  local pnt = { myH = theH }
  setmetatable(pnt, naivecgl.Naive_XYZ)
  return pnt
end

---
---@return number
function naivecgl.Naive_XYZ:X()
  return self.myH.x
end

---
---@return number
function naivecgl.Naive_XYZ:Y()
  return self.myH.y
end

---
---@return number
function naivecgl.Naive_XYZ:Z()
  return self.myH.z
end

---
---@return ffi.cdata*
function naivecgl.Naive_XYZ:Data()
  return self.myH
end

function naivecgl.Naive_XYZ:Magnitude()
  return math.sqrt(self:X() ^ 2 + self:Y() ^ 2 + self:Z() ^ 2)
end

--------------------------------------------------------------------------------
--                               Collections                                  --
--------------------------------------------------------------------------------

---
---@param theObj any
---@return ffi.cdata*
local function getFFIType(theObj)
  if type(theObj) == "table" then
    return theObj.myType
  elseif type(theObj) == "string" then
    return theObj
  else
    error("Unknown ffi type")
  end
end

---
---@param theObj any
---@return any
local function getFFIData(theObj)
  if type(theObj) == "table" then
    return theObj:Data()
  else
    return theObj
  end
end

---1-indexed
---@class naivecgl.Naive_Array<T>
---@field private myType any
---@field private myH ffi.cdata*
---@field private mySize integer
naivecgl.Naive_Array = {}

---@private
naivecgl.Naive_Array.__index = naivecgl.Naive_Array

---Constructor.
---@generic T
---@param theList T[]
---@return naivecgl.Naive_Array
function naivecgl.Naive_Array:new(theList)
  local aSize = #theList
  local aH = ffi.new(ffi.typeof(getFFIType(self.myType) .. "[?]"), aSize)
  for i = 1, aSize do
    aH[i - 1] = getFFIData(theList[i])
  end
  return self:take(aH, aSize)
end

---
---@param theH ffi.cdata*?
---@param theSize integer
---@return naivecgl.Naive_Array
function naivecgl.Naive_Array:take(theH, theSize)
  local arr = { myH = theH, mySize = theH == nil and 0 or theSize }
  setmetatable(arr, self)
  return arr
end

---
---@return integer
function naivecgl.Naive_Array:Size()
  return self.mySize
end

---
---@return ffi.cdata*
function naivecgl.Naive_Array:Data()
  return self.myH
end

---
---@generic T
---@param theIndex integer
---@return T
function naivecgl.Naive_Array:Value(theIndex)
  if theIndex < 1 or theIndex > self.mySize then
    error("Out of range")
  end
  if type(self.myType) == "table" then
    return self.myType.take(self.myH[theIndex - 1])
  else
    return self.myH[theIndex - 1]
  end
end

function naivecgl.Naive_Array:Unset()
  return self:take(nil, 0)
end

---
---@param theType table|string
local function declareArray(theType)
  local arr = { myType = theType }
  setmetatable(arr, naivecgl.Naive_Array)
  arr.__index = arr
  return arr
end

---@class naivecgl.Naive_Int32Array : naivecgl.Naive_Array<integer>
naivecgl.Naive_Int32Array = declareArray("int")

---@class naivecgl.Naive_DoubleArray : naivecgl.Naive_Array<number>
naivecgl.Naive_DoubleArray = declareArray("double")

---@class naivecgl.Naive_XYArray : naivecgl.Naive_Array<naivecgl.Naive_XY>
naivecgl.Naive_XYArray = declareArray(naivecgl.Naive_XY)

---@class naivecgl.Naive_XYZArray : naivecgl.Naive_Array<naivecgl.Naive_XYZ>
naivecgl.Naive_XYZArray = declareArray(naivecgl.Naive_XYZ)

--------------------------------------------------------------------------------
--                            Naive_NurbsCurve                                --
--------------------------------------------------------------------------------

---@class naivecgl.Naive_NurbsCurve
---@field private myH ffi.cdata*
naivecgl.Naive_NurbsCurve = {}

---@private
naivecgl.Naive_NurbsCurve.__index = naivecgl.Naive_NurbsCurve

---Constructor.
---@param thePoles naivecgl.Naive_XYZ[]
---@param theWeights number[]
---@param theKnots number[]
---@param theMults integer[]
---@param theDegree integer
---@return naivecgl.Naive_NurbsCurve
function naivecgl.Naive_NurbsCurve.new(thePoles, theWeights, theKnots, theMults, theDegree)
  local aPoles = naivecgl.Naive_XYZArray:new(thePoles)
  local aWeights = naivecgl.Naive_DoubleArray:new(theWeights)
  local aKnots = naivecgl.Naive_DoubleArray:new(theKnots)
  local aMults = naivecgl.Naive_Int32Array:new(theMults)

  local aH = ffi.new("Naive_Handle_t[1]")
  local aCode = naivecgl.NS.Naive_NurbsCurve_new(
    aPoles:Size(), aPoles:Data(), aWeights:Size(), aWeights:Data(),
    aKnots:Size(), aKnots:Data(), aMults:Size(), aMults:Data(),
    theDegree, aH)

  local nurbsCurve = {
    myH = ffi.gc(aH[0], function(theHandle)
      naivecgl.NS.Naive_Transient_release(theHandle)
    end)
  }
  setmetatable(nurbsCurve, naivecgl.Naive_NurbsCurve)
  return nurbsCurve
end

---
---@return integer
function naivecgl.Naive_NurbsCurve:Degree()
  local aDegree = ffi.new("int[1]")
  local aCode = naivecgl.NS.Naive_NurbsCurve_ask_degree(self.myH, aDegree)
  return aDegree[0]
end

---
---@return integer
function naivecgl.Naive_NurbsCurve:NbPoles()
  local nbPoles = ffi.new("int[1]")
  local aCode = naivecgl.NS.Naive_NurbsCurve_ask_poles(self.myH, nbPoles, nil)
  return nbPoles[0]
end

---
---@param theI integer
---@return naivecgl.Naive_XYZArray?
function naivecgl.Naive_NurbsCurve:Poles()
  local nbPoles = ffi.new("int[1]")
  if naivecgl.NS.Naive_NurbsCurve_ask_poles(self.myH, nbPoles, nil) ~= 0 then
    return
  end
  local aP = ffi.new("Naive_Point3d_t[?]", nbPoles[0])
  if naivecgl.NS.Naive_NurbsCurve_ask_poles(self.myH, nbPoles, aP) ~= 0 then
    return
  end
  return naivecgl.Naive_XYZArray:take(aP, nbPoles[0])
end

---
---@param theI integer
---@return number
function naivecgl.Naive_NurbsCurve:Weights()
  local aWeight = ffi.new("double[1]")
  local aCode = naivecgl.NS.Naive_NurbsCurve_Weight(self.myH, theI, aWeight)
  return aWeight[0]
end

---
---@return integer
function naivecgl.Naive_NurbsCurve:NbKnots()
  local nbKnots = ffi.new("int[1]")
  local aCode = naivecgl.NS.Naive_NurbsCurve_ask_knots(self.myH, nbKnots, nil)
  return nbKnots[0]
end

---
---@param theI integer
---@return naivecgl.Naive_DoubleArray?
function naivecgl.Naive_NurbsCurve:Knots()
  local nbKnots = ffi.new("int[1]")
  if naivecgl.NS.Naive_NurbsCurve_ask_knots(self.myH, nbKnots, nil) ~= 0 then
    return
  end
  local aK = ffi.new("double[?]", nbKnots[0])
  if naivecgl.NS.Naive_NurbsCurve_ask_knots(self.myH, nbKnots, aK) ~= 0 then
    return
  end
  return naivecgl.Naive_DoubleArray:take(aK, nbKnots[0])
end

---
---@param theI integer
---@return integer
function naivecgl.Naive_NurbsCurve:Multiplicities(theI)
  local aMult = ffi.new("int[1]")
  local aCode = naivecgl.NS.Naive_NurbsCurve_Multiplicity(self.myH, theI, aMult)
  return aMult[0]
end

---
---@return number
function naivecgl.Naive_NurbsCurve:FirstParameter()
  local aBound = ffi.new("Naive_Interval_t")
  local aCode = naivecgl.NS.Naive_NurbsCurve_ask_bound(self.myH, aBound)
  return aBound.t0
end

---
---@return number
function naivecgl.Naive_NurbsCurve:LastParameter()
  local aBound = ffi.new("Naive_Interval_t")
  local aCode = naivecgl.NS.Naive_NurbsCurve_ask_bound(self.myH, aBound)
  return aBound.t1
end

---Point at parameter |theT|.
---@param theT number
---@return naivecgl.Naive_XYZ?
function naivecgl.Naive_NurbsCurve:PointAt(theT)
  local nbD = ffi.new("int[1]")
  local aP = ffi.new(naivecgl.Naive_XYZ.myType)
  if naivecgl.NS.Naive_NurbsCurve_evaluate(self.myH, theT, 0, nbD, aP) == 0 then
    return naivecgl.Naive_XYZ.take(aP)
  end
end

---
---@param theT number
---@return naivecgl.Naive_XYZ?
function naivecgl.Naive_NurbsCurve:TangentAt(theT)
  local nbD = ffi.new("int[1]")
  local aP = ffi.new("Naive_Vector3d_t[2]")
  if naivecgl.NS.Naive_NurbsCurve_evaluate(self.myH, theT, 1, nbD, aP) == 0 then
    return naivecgl.Naive_XYZ.take(aP[1])
  end
end

---
---@param theT number
---@param theN integer
---@return boolean
---@return naivecgl.Naive_XYZArray
function naivecgl.Naive_NurbsCurve:DerivativeAt(theT, theN)
  local nbD = ffi.new("int[1]", 0)
  if naivecgl.NS.Naive_NurbsCurve_evaluate(self.myH, theT, theN, nbD, nil) ~= 0 then
    return false, {}
  end
  local aD = ffi.new("Naive_Vector3d_t[?]", nbD[0])
  if naivecgl.NS.Naive_NurbsCurve_evaluate(self.myH, theT, theN, nbD, aD) ~= 0 then
    return false, {}
  end
  return true, naivecgl.Naive_XYZArray:take(aD, nbD[0])
end

---
---@param theT number
---@return naivecgl.Naive_XYZ?
function naivecgl.Naive_NurbsCurve:CurvatureAt(theT)
  local aV = ffi.new("Naive_Vector3d_t")
  if naivecgl.NS.Naive_NurbsCurve_curvature_at(self.myH, theT, aV) == 0 then
    return naivecgl.Naive_XYZ.take(aV)
  end
end

---
---@param theI integer
---@param theM integer
---@return integer
function naivecgl.Naive_NurbsCurve:IncreaseMultiplicity(theI, theM)
  return naivecgl.NS.Naive_NurbsCurve_increase_multiplicity(self.myH, theI, theM)
end

---
---@param theT number
---@param theM integer
---@return integer
function naivecgl.Naive_NurbsCurve:InsertKnot(theT, theM)
  return naivecgl.NS.Naive_NurbsCurve_insert_knot(self.myH, theT, theM)
end

---Dispose.
function naivecgl.Naive_NurbsCurve:Dispose()
  if self.myH then
    naivecgl.NS.Naive_Transient_release(ffi.gc(self.myH, nil))
    self.myH = nil
  end
end

--------------------------------------------------------------------------------
--                            Naive_NurbsCurve                                --
--------------------------------------------------------------------------------

---@class naivecgl.Naive_NurbsSurface
---@field private myH ffi.cdata*
naivecgl.Naive_NurbsSurface = {}

---@private
naivecgl.Naive_NurbsSurface.__index = naivecgl.Naive_NurbsSurface

---
---@generic T
---@param theArr2 T[][]
---@return integer
---@return integer
---@return T[]
local function flattenArray2(theArr2)
  if #theArr2 == 0 then
    return 0, 0, {}
  end

  local nbU = #theArr2
  local nbV = #(theArr2[1])

  local aRes = {}
  for i = 1, nbU, 1 do
    if #(theArr2[i]) ~= nbV then
      return 0, 0, {}
    end

    for j = 1, nbV, 1 do
      table.insert(aRes, theArr2[i][j])
    end
  end

  return nbU, nbV, aRes
end

---Constructor.
---@param thePoles naivecgl.Naive_XYZ[][]
---@param theWeights number[][]
---@param theUKnots number[]
---@param theVKnots number[]
---@param theUMults integer[]
---@param theVMults integer[]
---@param theUDegree integer
---@param theVDegree integer
---@return naivecgl.Naive_NurbsSurface
function naivecgl.Naive_NurbsSurface.new(thePoles, theWeights,
                                         theUKnots, theVKnots,
                                         theUMults, theVMults,
                                         theUDegree, theVDegree)
  local nbUP, nbVP, aFlatPoles = flattenArray2(thePoles)
  local aPoles = naivecgl.Naive_XYZArray:new(aFlatPoles)

  local nbUW, nbVW, aFlatWeights = flattenArray2(theWeights)
  local aWeights = naivecgl.Naive_DoubleArray:new(aFlatWeights)

  local aUKnots = naivecgl.Naive_DoubleArray:new(theUKnots)
  local aVKnots = naivecgl.Naive_DoubleArray:new(theVKnots)
  local aUMults = naivecgl.Naive_Int32Array:new(theUMults)
  local aVMults = naivecgl.Naive_Int32Array:new(theVMults)

  local aH = ffi.new("Naive_Handle_t[1]")
  local aCode = naivecgl.NS.Naive_NurbsSurface_new(
    nbUP, nbVP, aPoles:Data(),
    nbUW, nbVW, aWeights:Data(),
    aUKnots:Size(), aUKnots:Data(), aVKnots:Size(), aVKnots:Data(),
    aUMults:Size(), aUMults:Data(), aVMults:Size(), aVMults:Data(),
    theUDegree, theVDegree, aH)

  local nurbsSurface = {
    myH = ffi.gc(aH[0], function(theHandle)
      naivecgl.NS.Naive_Transient_release(theHandle)
    end)
  }
  setmetatable(nurbsSurface, naivecgl.Naive_NurbsSurface)
  return nurbsSurface
end

---
---@param theU number
---@param theV number
---@return naivecgl.Naive_XYZ?
function naivecgl.Naive_NurbsSurface:PointAt(theU, theV)
  local nbD = ffi.new("int[1]")
  local aP = ffi.new(naivecgl.Naive_XYZ.myType)
  if naivecgl.NS.Naive_NurbsSurface_evaluate(self.myH, theU, theV, 0, nbD, aP) == 0 then
    return naivecgl.Naive_XYZ.take(aP)
  end
end

---
---@param theU number
---@param theV number
---@param theN integer
---@return boolean
---@return naivecgl.Naive_XYZArray
function naivecgl.Naive_NurbsSurface:Evaluate(theU, theV, theN)
  local nbD = ffi.new("int[1]", 0)
  if naivecgl.NS.Naive_NurbsSurface_evaluate(self.myH, theU, theV, theN, nbD, nil) ~= 0 then
    return false, {}
  end
  local aD = ffi.new("Naive_Vector3d_t[?]", nbD[0])
  if naivecgl.NS.Naive_NurbsSurface_evaluate(self.myH, theU, theV, theN, nbD, aD) ~= 0 then
    return false, {}
  end
  return true, naivecgl.Naive_XYZArray:take(aD, nbD[0])
end

function naivecgl.Naive_NurbsSurface:Dispose()
  if self.myH then
    naivecgl.NS.Naive_Transient_release(ffi.gc(self.myH, nil))
    self.myH = nil
  end
end

--------------------------------------------------------------------------------
--                               Naive_Poly                                   --
--------------------------------------------------------------------------------

---@class naivecgl.Naive_Poly
---@field private myH ffi.cdata*
naivecgl.Naive_Poly = {}

---@private
naivecgl.Naive_Poly.__index = naivecgl.Naive_Poly

---Constructor.
---@param theNodes naivecgl.Naive_XYZ[]
---@param theTris integer[][] 1-indexed
---@return naivecgl.Naive_Poly
function naivecgl.Naive_Poly.new(theNodes, theTris)
  local nbNodes = #theNodes
  local aNodes = ffi.new("Naive_Point3d_t[?]", nbNodes)
  for i = 1, nbNodes do
    aNodes[i - 1] = theNodes[i]:Data()
  end

  local nbTris = #theTris
  local aTris = ffi.new("Naive_Triangle_t[?]", nbTris)
  for i = 1, nbTris do
    aTris[i - 1].n0 = theTris[i][1] - 1
    aTris[i - 1].n1 = theTris[i][2] - 1
    aTris[i - 1].n2 = theTris[i][3] - 1
  end

  local aH = ffi.new("Naive_Handle_t[1]")
  local aCode = naivecgl.NS.Naive_Poly_new(nbNodes, aNodes, nbTris, aTris, aH)
  return naivecgl.Naive_Poly.take(aH[0])
end

---@param theH ffi.cdata*
---@return naivecgl.Naive_Poly
function naivecgl.Naive_Poly.take(theH)
  local poly = {
    myH = ffi.gc(theH, function(theHandle)
      naivecgl.NS.Naive_Transient_release(theHandle)
    end),
  }
  setmetatable(poly, naivecgl.Naive_Poly)
  return poly
end

---Number of vertices.
---@return integer
function naivecgl.Naive_Poly:NbVertices()
  local nbVerts = ffi.new("int[1]")
  local aCode = naivecgl.NS.Naive_Poly_ask_vertices(self.myH, nbVerts, nil)
  return nbVerts[0]
end

---Number of triangles.
---@return integer
function naivecgl.Naive_Poly:NbTriangles()
  local nbTris = ffi.new("int[1]")
  local aCode = naivecgl.NS.Naive_Poly_ask_triangles(self.myH, nbTris, nil)
  return nbTris[0]
end

---Vertices.
---@return naivecgl.Naive_XYZArray
function naivecgl.Naive_Poly:Vertices()
  local nbVertices = self:NbVertices()
  local aVertices = ffi.new("Naive_Point3d_t[?]", nbVertices)
  local nbVerts = ffi.new("int[1]")
  local aCode = naivecgl.NS.Naive_Poly_ask_vertices(self.myH, nbVerts, aVertices)
  return naivecgl.Naive_XYZArray:take(aVertices, nbVertices)
end

---Triangles (1-indexed).
---@return integer[][]
function naivecgl.Naive_Poly:Triangles()
  local nbTriangles = self:NbTriangles()
  local aTriangles = ffi.new("Naive_Triangle_t[?]", nbTriangles)
  local nbTris = ffi.new("int[1]")
  naivecgl.NS.Naive_Poly_ask_triangles(self.myH, nbTris, aTriangles)

  local aRes = {}
  for i = 1, nbTriangles do
    aRes[i] = {
      aTriangles[i - 1].n0 + 1,
      aTriangles[i - 1].n1 + 1,
      aTriangles[i - 1].n2 + 1,
    }
  end

  return aRes
end

function naivecgl.Naive_Poly:Dispose()
  if self.myH then
    naivecgl.NS.Naive_Transient_release(ffi.gc(self.myH, nil))
    self.myH = nil
  end
end

--------------------------------------------------------------------------------
--                    naivecgl::bndshape::ConvexHull2D                        --
--------------------------------------------------------------------------------

---@class naivecgl.bndshape.ConvexHull2D
---@field private myH ffi.cdata*
naivecgl.bndshape.ConvexHull2D = {}

---@private
naivecgl.bndshape.ConvexHull2D.__index = naivecgl.bndshape.ConvexHull2D

---Constructor.
---@param thePoints naivecgl.Naive_XY[]
---@param theAlgo any
---@return naivecgl.bndshape.ConvexHull2D
function naivecgl.bndshape.ConvexHull2D.new(thePoints, theAlgo)
  theAlgo = theAlgo or naivecgl.NS.Naive_Algorithm_quick_hull

  local aPoints = naivecgl.Naive_XYArray:new(thePoints)
  local aH = ffi.new("Naive_Handle_t[1]")
  local aCode = naivecgl.NS.Naive_BndShape_ConvexHull2D_new(aPoints:Size(), aPoints:Data(), theAlgo, aH)

  local ch2d = {
    myH = ffi.gc(aH[0], function(theHandle)
      naivecgl.NS.Naive_Transient_release(theHandle)
    end)
  }
  setmetatable(ch2d, naivecgl.bndshape.ConvexHull2D)
  return ch2d
end

---comment
---@param theAlgo any
---@return integer
function naivecgl.bndshape.ConvexHull2D:SetAlgorithm(theAlgo)
  return naivecgl.NS.Naive_BndShape_ConvexHull2D_set_algorithm(self.myH, theAlgo)
end

---Perform the algorithm.
---@return integer
function naivecgl.bndshape.ConvexHull2D:Perform()
  return naivecgl.NS.Naive_BndShape_ConvexHull2D_perform(self.myH)
end

---Add a point.
---@param thePnt table
---@param thePerform boolean?
function naivecgl.bndshape.ConvexHull2D:Add(thePnt, thePerform)
  if thePerform == nil then
    thePerform = false
  end

  local aPnt = ffi.new("Naive_Point2d_t", thePnt)
  return naivecgl.NS.Naive_BndShape_ConvexHull2D_add_point(self.myH, aPnt, thePerform)
end

---Get the convex indices.
---@return integer[]
function naivecgl.bndshape.ConvexHull2D:ConvexIndices()
  local aRes = {}

  local aCode
  local nbRes = ffi.new "int[1]"
  aCode = naivecgl.NS.Naive_BndShape_ConvexHull2D_ask_result(self.myH, nbRes, nil, nil)
  print(nbRes[0])
  local indices = ffi.new("int[?]", nbRes[0])
  aCode = naivecgl.NS.Naive_BndShape_ConvexHull2D_ask_result(self.myH, nbRes, indices, nil)

  for i = 1, nbRes[0] do
    aRes[i] = indices[i - 1] + 1
  end

  return aRes
end

function naivecgl.bndshape.ConvexHull2D:Dispose()
  if self.myH then
    naivecgl.NS.Naive_Transient_release(ffi.gc(self.myH, nil))
    self.myH = nil
  end
end

--------------------------------------------------------------------------------
--                    naivecgl::bndshape::EnclosingDisc                       --
--------------------------------------------------------------------------------

---@class naivecgl.bndshape.EnclosingDisc
---@field private myH ffi.cdata*
naivecgl.bndshape.EnclosingDisc = {}

---@private
naivecgl.bndshape.EnclosingDisc.__index = naivecgl.bndshape.EnclosingDisc

---Constructor.
---@return naivecgl.bndshape.EnclosingDisc
function naivecgl.bndshape.EnclosingDisc.new()
  local aH = ffi.new "Naive_Handle_t[1]"
  local aCode = naivecgl.NS.Naive_BndShape_EnclosingDisc_new(aH)

  local ed = {
    myH = ffi.gc(aH[0], function(theHandle)
      naivecgl.NS.Naive_Transient_release(theHandle)
    end)
  }
  setmetatable(ed, naivecgl.bndshape.EnclosingDisc)
  return ed
end

---Rebuild the disc.
---@param thePoints naivecgl.Naive_XY[]
function naivecgl.bndshape.EnclosingDisc:ReBuild(thePoints)
  local aPoints = naivecgl.Naive_XYArray:new(thePoints)
  return naivecgl.NS.Naive_BndShape_EnclosingDisc_rebuild(self.myH, aPoints:Size(), aPoints:Data())
end

---Get the cicle.
---@return integer
---@return naivecgl.Naive_XY origin
---@return number r
function naivecgl.bndshape.EnclosingDisc:Circle()
  local anOrigin = ffi.new("Naive_Point2d_t")
  local aR = ffi.new("double[1]", 0)
  local aCode = naivecgl.NS.Naive_BndShape_EnclosingDisc_ask_circle(self.myH, anOrigin, aR)
  return aCode, naivecgl.Naive_XY.take(anOrigin), aR[0]
end

function naivecgl.bndshape.EnclosingDisc:Dispose()
  if self.myH then
    naivecgl.NS.Naive_Transient_release(ffi.gc(self.myH, nil))
    self.myH = nil
  end
end

--------------------------------------------------------------------------------
--                    naivecgl::tessellation::TetraSphere                     --
--------------------------------------------------------------------------------

---Calculates the tetrasphere with a tessellation level.
---@param theCenter naivecgl.Naive_XYZ
---@param theRadius number
---@param theLevel integer
---@return naivecgl.Naive_Poly
function naivecgl.tessellation.Naive_Tessellation_TetraSphere(theCenter, theRadius, theLevel)
  local aPoly = ffi.new "Naive_Handle_t[1]"
  local aCode = naivecgl.NS.Naive_Tessellation_tetrasphere(theCenter:Data(), theRadius, theLevel, aPoly)
  return naivecgl.Naive_Poly.take(aPoly[0])
end

--------------------------------------------------------------------------------
--                                 Export                                     --
--------------------------------------------------------------------------------

naivecgl:init()

return naivecgl
