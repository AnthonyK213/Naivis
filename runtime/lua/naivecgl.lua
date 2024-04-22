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
typedef void *Naive_H;

typedef enum {
  Naive_Ok = 0,
  Naive_Err,
  Naive_Initialized,
  Naive_NullException,
  Naive_NotImplemented,
  Naive_ConvexHull_InsufficientPoint,
  Naive_ConvexHull_PointsAreCollinear,
  Naive_ConvexHull_PointsAreCoplanar,
} Naive_Code;

typedef enum {
  Naive_ConvexHull2D_Quickhull = 0,
  Naive_ConvexHull2D_Incremental,
  Naive_ConvexHull2D_GrahamScan,
} Naive_ConvexHull2D_Algorithm;

typedef enum {
  Naive_ConvexHull3D_Quickhull = 0,
  Naive_ConvexHull3D_Incremental,
  Naive_ConvexHull3D_DivideAndConquer,
} Naive_ConvexHull3D_Algorithm;

typedef struct {
  double x, y;
} Naive_XY_T;

typedef struct {
  double x, y, z;
} Naive_XYZ_T;

typedef Naive_XY_T Naive_Vector2d_T;
typedef Naive_XY_T Naive_Point2d_T;
typedef Naive_XYZ_T Naive_Vector3d_T;
typedef Naive_XYZ_T Naive_Point3d_T;

typedef struct {
  double t0, t1;
} Naive_Interval_T;

typedef struct {
  Naive_Point3d_T origin;
  Naive_Vector3d_T xAxis, yAxis;
} Naive_Plane_T;

typedef struct {
  Naive_Plane_T plane;
  double radius;
} Naive_Circle_T;

typedef struct {
  Naive_Point3d_T from, to;
} Naive_Line_T;

typedef struct {
  Naive_Plane_T plane;
  Naive_Interval_T x, y;
} Naive_Rectangle_T;

typedef struct {
  Naive_Circle_T circle;
  double height;
} Naive_Cylinder_T;

typedef struct {
  int32_t n0, n1, n2;
} Naive_Triangle_T;
  ]]

  -- NaiveCGL_c.h
  ffi.cdef [[
/// Naive_NurbsCurve {{{

Naive_H Naive_NurbsCurve_New(const int32_t nbPoles, const Naive_Point3d_T *thePoles, const int32_t nbWeights, const double *theWeights, const int32_t nbKnots, const double *theKnots, const int32_t nbMults, const int32_t *theMults, const int32_t theDegree);

int32_t Naive_NurbsCurve_Degree(const Naive_H theHandle);

int32_t Naive_NurbsCurve_NbPoles(const Naive_H theHandle);

bool Naive_NurbsCurve_Pole(const Naive_H theHandle, const int32_t theI, Naive_Point3d_T *thePole);

double Naive_NurbsCurve_Weight(const Naive_H theHandle, const int32_t theI);

int32_t Naive_NurbsCurve_NbKnots(const Naive_H theHandle);

double Naive_NurbsCurve_Knot(const Naive_H theHandle, const int32_t theI);

int32_t Naive_NurbsCurve_Multiplicity(const Naive_H theHandle, const int32_t theI);

double Naive_NurbsCurve_FirstParameter(const Naive_H theHandle);

double Naive_NurbsCurve_LastParameter(const Naive_H theHandle);

bool Naive_NurbsCurve_PointAt(const Naive_H theHandle, const double theT, Naive_Point3d_T *theP);

bool Naive_NurbsCurve_TangentAt(const Naive_H theHandle, const double theT, Naive_Vector3d_T *theV);

bool Naive_NurbsCurve_DerivativeAt(const Naive_H theHandle, const double theT, int32_t theN, int32_t *nbD, Naive_Vector3d_T *theD);

bool Naive_NurbsCurve_IncreaseMultiplicity(Naive_H theHandle, const int32_t theI, const int32_t theM);

bool Naive_NurbsCurve_InsertKnot(Naive_H theHandle, const double theT, const int32_t theM);

void Naive_NurbsCurve_Release(Naive_H theHandle);

/// }}}

/// Naive_NurbsSurface {{{

Naive_H Naive_NurbsSurface_New(const int32_t nbUPoles, const int32_t nbVPoles, const Naive_Point3d_T *thePoles, const int32_t nbUWeights, const int32_t nbVWeights, const double *theWeights, const int32_t nbUKnots, const double *theUKnots, const int32_t nbVKnots, const double *theVKnots, const int32_t nbUMults, const int32_t *theUMults, const int32_t nbVMults, const int32_t *theVMults, const int32_t theUDegree, const int32_t theVDegree);

int32_t Naive_NurbsSurface_UDegree(const Naive_H theHandle);

int32_t Naive_NurbsSurface_VDegree(const Naive_H theHandle);

bool Naive_NurbsSurface_PointAt(const Naive_H theHandle, const double theU, const double theV, Naive_Point3d_T *theP);

bool Naive_NurbsSurface_Evaluate(const Naive_H theHandle, const double theU, const double theV, int32_t theN, int32_t *nbD, Naive_Vector3d_T *theD);

void Naive_NurbsSurface_Release(Naive_H theHandle);

/// }}}

/// Naive_Poly {{{

Naive_H Naive_Poly_New(const int32_t nbVertices, const Naive_Point3d_T *theVertices, const int32_t nbTriangles, const Naive_Triangle_T *theTriangles);

int32_t Naive_Poly_NbVertices(const Naive_H theHandle);

void Naive_Poly_Vertices(const Naive_H theHandle, Naive_Point3d_T *theVertices);

int32_t Naive_Poly_NbTriangles(const Naive_H theHandle);

void Naive_Poly_Triangles(const Naive_H theHandle, Naive_Triangle_T *theTriangles);

void Naive_Poly_Release(Naive_H theHandle);

/// }}}

/// BndShape {{{

Naive_H Naive_BndShape_ConvexHull2D_New(int32_t nbPoints, const Naive_Point2d_T *thePoints, Naive_ConvexHull2D_Algorithm theAlgo);

void Naive_BndShape_ConvexHull2D_SetAlgorithm(Naive_H theHandle, Naive_ConvexHull2D_Algorithm theAlgo);

void Naive_BndShape_ConvexHull2D_Perform(Naive_H theHandle);

void Naive_BndShape_ConvexHull2D_Add(Naive_H theHandle, Naive_Point2d_T thePoint, bool thePerform);

Naive_Code Naive_BndShape_ConvexHull2D_Status(const Naive_H theHandle);

int32_t Naive_BndShape_ConvexHull2D_NbConvexPoints(const Naive_H theHandle);

Naive_Code Naive_BndShape_ConvexHull2D_ConvexIndices(const Naive_H theHandle, int32_t *theConvexIndices);

Naive_Code Naive_BndShape_ConvexHull2D_ConvexPoints(const Naive_H theHandle, Naive_Point2d_T *theConvexPoints);

void Naive_BndShape_ConvexHull2D_Release(Naive_H theHandle);

Naive_H Naive_BndShape_EnclosingDisc_New();

void Naive_BndShape_EnclosingDisc_Rebuild(Naive_H theHandle, int32_t nbPoints, const Naive_Point2d_T *thePoints);

bool Naive_BndShape_EnclosingDisc_Circle(const Naive_H theHandle, Naive_Point2d_T *theOrigin, double *theR);

void Naive_BndShape_EnclosingDisc_Release(Naive_H theHandle);

/// }}}

/// Tessellation {{{

Naive_H Naive_Tessellation_TetraSphere(const Naive_Point3d_T *theCenter, double theRadius, int32_t theLevel);

/// }}}
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
naivecgl.Naive_XY = { myType = "Naive_XY_T" }

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
naivecgl.Naive_XYZ = { myType = "Naive_XYZ_T" }

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
naivecgl.Naive_Int32Array = declareArray("int32_t")

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

  local aH = naivecgl.NS.Naive_NurbsCurve_New(
    aPoles:Size(), aPoles:Data(), aWeights:Size(), aWeights:Data(),
    aKnots:Size(), aKnots:Data(), aMults:Size(), aMults:Data(),
    theDegree)

  local nurbsCurve = {
    myH = ffi.gc(aH, function(theHandle)
      naivecgl.NS.Naive_NurbsCurve_Release(theHandle)
    end)
  }
  setmetatable(nurbsCurve, naivecgl.Naive_NurbsCurve)
  return nurbsCurve
end

---
---@return integer
function naivecgl.Naive_NurbsCurve:Degree()
  return naivecgl.NS.Naive_NurbsCurve_Degree(self.myH)
end

---
---@return integer
function naivecgl.Naive_NurbsCurve:NbPoles()
  return naivecgl.NS.Naive_NurbsCurve_NbPoles(self.myH)
end

---
---@param theI integer
---@return naivecgl.Naive_XYZ?
function naivecgl.Naive_NurbsCurve:Pole(theI)
  local aP = ffi.new(naivecgl.Naive_XYZ.myType)
  if naivecgl.NS.Naive_NurbsCurve_Pole(self.myH, theI, aP) then
    return naivecgl.Naive_XYZ.take(aP)
  end
end

---
---@param theI integer
---@return number
function naivecgl.Naive_NurbsCurve:Weight(theI)
  return naivecgl.NS.Naive_NurbsCurve_Weight(self.myH, theI)
end

---
---@return integer
function naivecgl.Naive_NurbsCurve:NbKnots()
  return naivecgl.NS.Naive_NurbsCurve_NbKnots(self.myH)
end

---
---@param theI integer
---@return number
function naivecgl.Naive_NurbsCurve:Knot(theI)
  return naivecgl.NS.Naive_NurbsCurve_Knot(self.myH, theI)
end

---
---@param theI integer
---@return integer
function naivecgl.Naive_NurbsCurve:Multiplicity(theI)
  return naivecgl.NS.Naive_NurbsCurve_Multiplicity(self.myH, theI)
end

---
---@return number
function naivecgl.Naive_NurbsCurve:FirstParameter()
  return naivecgl.NS.Naive_NurbsCurve_FirstParameter(self.myH)
end

---
---@return number
function naivecgl.Naive_NurbsCurve:LastParameter()
  return naivecgl.NS.Naive_NurbsCurve_LastParameter(self.myH)
end

---Point at parameter |theT|.
---@param theT number
---@return naivecgl.Naive_XYZ?
function naivecgl.Naive_NurbsCurve:PointAt(theT)
  local aP = ffi.new("Naive_Point3d_T")
  if naivecgl.NS.Naive_NurbsCurve_PointAt(self.myH, theT, aP) then
    return naivecgl.Naive_XYZ.take(aP)
  end
end

---
---@param theT number
---@return naivecgl.Naive_XYZ?
function naivecgl.Naive_NurbsCurve:TangentAt(theT)
  local aV = ffi.new("Naive_Vector3d_T")
  if naivecgl.NS.Naive_NurbsCurve_TangentAt(self.myH, theT, aV) then
    return naivecgl.Naive_XYZ.take(aV)
  end
end

---
---@param theT number
---@param theN integer
---@return boolean
---@return naivecgl.Naive_XYZArray
function naivecgl.Naive_NurbsCurve:DerivativeAt(theT, theN)
  local nbD = ffi.new("int32_t[1]", 0)
  if not naivecgl.NS.Naive_NurbsCurve_DerivativeAt(self.myH, theT, theN, nbD, nil) then
    return false, {}
  end
  local aD = ffi.new("Naive_Vector3d_T[?]", nbD[0])
  if not naivecgl.NS.Naive_NurbsCurve_DerivativeAt(self.myH, theT, theN, nbD, aD) then
    return false, {}
  end
  return true, naivecgl.Naive_XYZArray:take(aD, nbD[0])
end

---
---@param theI integer
---@param theM integer
---@return boolean
function naivecgl.Naive_NurbsCurve:IncreaseMultiplicity(theI, theM)
  return naivecgl.NS.Naive_NurbsCurve_IncreaseMultiplicity(self.myH, theI, theM)
end

---
---@param theT number
---@param theM integer
---@return boolean
function naivecgl.Naive_NurbsCurve:InsertKnot(theT, theM)
  return naivecgl.NS.Naive_NurbsCurve_InsertKnot(self.myH, theT, theM)
end

---Dispose.
function naivecgl.Naive_NurbsCurve:Dispose()
  if self.myH then
    naivecgl.NS.Naive_NurbsCurve_Release(ffi.gc(self.myH, nil))
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

  local aH = naivecgl.NS.Naive_NurbsSurface_New(
    nbUP, nbVP, aPoles:Data(),
    nbUW, nbVW, aWeights:Data(),
    aUKnots:Size(), aUKnots:Data(), aVKnots:Size(), aVKnots:Data(),
    aUMults:Size(), aUMults:Data(), aVMults:Size(), aVMults:Data(),
    theUDegree, theVDegree)

  local nurbsSurface = {
    myH = ffi.gc(aH, function(theHandle)
      naivecgl.NS.Naive_NurbsSurface_Release(theHandle)
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
  local aP = ffi.new("Naive_Point3d_T")
  if naivecgl.NS.Naive_NurbsSurface_PointAt(self.myH, theU, theV, aP) then
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
  local nbD = ffi.new("int32_t[1]", 0)
  if not naivecgl.NS.Naive_NurbsSurface_Evaluate(self.myH, theU, theV, theN, nbD, nil) then
    return false, {}
  end
  local aD = ffi.new("Naive_Vector3d_T[?]", nbD[0])
  if not naivecgl.NS.Naive_NurbsSurface_Evaluate(self.myH, theU, theV, theN, nbD, aD) then
    return false, {}
  end
  return true, naivecgl.Naive_XYZArray:take(aD, nbD[0])
end

function naivecgl.Naive_NurbsSurface:Dispose()
  if self.myH then
    naivecgl.NS.Naive_NurbsSurface_Release(ffi.gc(self.myH, nil))
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
  local aNodes = ffi.new("Naive_Point3d_T[?]", nbNodes)
  for i = 1, nbNodes do
    aNodes[i - 1] = theNodes[i]:Data()
  end

  local nbTris = #theTris
  local aTris = ffi.new("Naive_Triangle_T[?]", nbTris)
  for i = 1, nbTris do
    aTris[i - 1].n0 = theTris[i][1] - 1
    aTris[i - 1].n1 = theTris[i][2] - 1
    aTris[i - 1].n2 = theTris[i][3] - 1
  end

  local aH = naivecgl.NS.Naive_Poly_New(nbNodes, aNodes, nbTris, aTris)
  return naivecgl.Naive_Poly.take(aH)
end

---@param theH ffi.cdata*
---@return naivecgl.Naive_Poly
function naivecgl.Naive_Poly.take(theH)
  local poly = {
    myH = ffi.gc(theH, function(theHandle)
      naivecgl.NS.Naive_Poly_Release(theHandle)
    end),
  }
  setmetatable(poly, naivecgl.Naive_Poly)
  return poly
end

---Number of vertices.
---@return integer
function naivecgl.Naive_Poly:NbVertices()
  return naivecgl.NS.Naive_Poly_NbVertices(self.myH)
end

---Number of triangles.
---@return integer
function naivecgl.Naive_Poly:NbTriangles()
  return naivecgl.NS.Naive_Poly_NbTriangles(self.myH)
end

---Vertices.
---@return naivecgl.Naive_XYZArray
function naivecgl.Naive_Poly:Vertices()
  local nbVertices = self:NbVertices()
  local aVertices = ffi.new("Naive_Point3d_T[?]", nbVertices)
  naivecgl.NS.Naive_Poly_Vertices(self.myH, aVertices)
  return naivecgl.Naive_XYZArray:take(aVertices, nbVertices)
end

---Triangles (1-indexed).
---@return integer[][]
function naivecgl.Naive_Poly:Triangles()
  local nbTriangles = self:NbTriangles()
  local aTriangles = ffi.new("Naive_Triangle_T[?]", nbTriangles)
  naivecgl.NS.Naive_Poly_Triangles(self.myH, aTriangles)

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
    naivecgl.NS.Naive_Poly_Release(ffi.gc(self.myH, nil))
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
  theAlgo = theAlgo or naivecgl.NS.Naive_ConvexHull2D_Quickhull

  local aPoints = naivecgl.Naive_XYArray:new(thePoints)
  local aH = naivecgl.NS.Naive_BndShape_ConvexHull2D_New(aPoints:Size(), aPoints:Data(), theAlgo)

  local o = {
    myH = ffi.gc(aH, function(theHandle)
      naivecgl.NS.Naive_BndShape_ConvexHull2D_Release(theHandle)
    end)
  }
  setmetatable(o, naivecgl.bndshape.ConvexHull2D)
  return o
end

function naivecgl.bndshape.ConvexHull2D:SetAlgorithm(theAlgo)
  naivecgl.NS.Naive_BndShape_ConvexHull2D_SetAlgorithm(self.myH, theAlgo)
end

---Perform the algorithm.
function naivecgl.bndshape.ConvexHull2D:Perform()
  naivecgl.NS.Naive_BndShape_ConvexHull2D_Perform(self.myH)
end

---Add a point.
---@param thePnt table
---@param thePerform boolean?
function naivecgl.bndshape.ConvexHull2D:Add(thePnt, thePerform)
  if thePerform == nil then
    thePerform = false
  end

  local aPnt = ffi.new("Naive_Point2d_T", thePnt)
  naivecgl.NS.Naive_BndShape_ConvexHull2D_Add(self.myH, aPnt, thePerform)
end

---Get status.
---@return any
function naivecgl.bndshape.ConvexHull2D:Status()
  return naivecgl.NS.Naive_BndShape_ConvexHull2D_Status(self.myH)
end

---Get the convex indices.
---@return integer[]
function naivecgl.bndshape.ConvexHull2D:ConvexIndices()
  local aRes = {}

  if self:Status() == naivecgl.NS.Naive_Ok then
    local count = naivecgl.NS.Naive_BndShape_ConvexHull2D_NbConvexPoints(self.myH)
    local indices = ffi.new("int32_t[?]", count)
    naivecgl.NS.Naive_BndShape_ConvexHull2D_ConvexIndices(self.myH, indices)

    for i = 1, count do
      aRes[i] = indices[i - 1] + 1
    end
  end

  return aRes
end

function naivecgl.bndshape.ConvexHull2D:Dispose()
  if self.myH then
    naivecgl.NS.Naive_BndShape_ConvexHull2D_Release(ffi.gc(self.myH, nil))
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
  local aH = naivecgl.NS.Naive_BndShape_EnclosingDisc_New()

  local o = {
    myH = ffi.gc(aH, function(theHandle)
      naivecgl.NS.Naive_BndShape_EnclosingDisc_Release(theHandle)
    end)
  }
  setmetatable(o, naivecgl.bndshape.EnclosingDisc)
  return o
end

---Rebuild the disc.
---@param thePoints naivecgl.Naive_XY[]
function naivecgl.bndshape.EnclosingDisc:ReBuild(thePoints)
  local aPoints = naivecgl.Naive_XYArray:new(thePoints)
  naivecgl.NS.Naive_BndShape_EnclosingDisc_Rebuild(self.myH, aPoints:Size(), aPoints:Data())
end

---Get the cicle.
---@return boolean
---@return naivecgl.Naive_XY origin
---@return number r
function naivecgl.bndshape.EnclosingDisc:Circle()
  local anOrigin = ffi.new("Naive_Point2d_T")
  local aR = ffi.new("double[1]", 0)
  local ok = naivecgl.NS.Naive_BndShape_EnclosingDisc_Circle(self.myH, anOrigin, aR)
  return ok, naivecgl.Naive_XY.take(anOrigin), aR[0]
end

function naivecgl.bndshape.EnclosingDisc:Dispose()
  if self.myH then
    naivecgl.NS.Naive_BndShape_EnclosingDisc_Release(ffi.gc(self.myH, nil))
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
  local aHandle = naivecgl.NS.Naive_Tessellation_TetraSphere(theCenter:Data(), theRadius, theLevel)
  return naivecgl.Naive_Poly.take(aHandle)
end

--------------------------------------------------------------------------------
--                                 Export                                     --
--------------------------------------------------------------------------------

naivecgl:init()

return naivecgl
