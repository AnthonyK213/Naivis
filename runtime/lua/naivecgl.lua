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
} Naive_Vector2d_T;

typedef struct {
  double x, y, z;
} Naive_Vector3d_T;

typedef Naive_Vector2d_T Naive_Point2d_T;
typedef Naive_Vector3d_T Naive_Point3d_T;

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

/// Naive_NurbsCurve {{{

Naive_H Naive_NurbsCurve_New(const int32_t nbPoles, const Naive_Point3d_T *thePoles, const int32_t nbWeights, const double *theWeights, const int32_t nbKnots, const double *theKnots, const int32_t nbMults, const int32_t *theMults, const int32_t theDegree);

int32_t Naive_NurbsCurve_Degree(const Naive_H theHandle);

double Naive_NurbsCurve_FirstParameter(const Naive_H theHandle);

double Naive_NurbsCurve_LastParameter(const Naive_H theHandle);

bool Naive_NurbsCurve_PointAt(const Naive_H theHandle, const double theT, Naive_Point3d_T *theP);

bool Naive_NurbsCurve_TangentAt(const Naive_H theHandle, const double theT, Naive_Vector3d_T *theV);

void Naive_NurbsCurve_Release(Naive_H theHandle);

/// }}}

/// Naive_NurbsSurface {{{

Naive_H Naive_NurbsSurface_New(const int32_t nbUPoles, const int32_t nbVPoles, const Naive_Point3d_T *thePoles, const int32_t nbUWeights, const int32_t nbVWeights, const double *theWeights, const int32_t nbUKnots, const double *theUKnots, const int32_t nbVKnots, const double *theVKnots, const int32_t nbUMults, const int32_t *theUMults, const int32_t nbVMults, const int32_t *theVMults, const int32_t theUDegree, const int32_t theVDegree);

int32_t Naive_NurbsSurface_UDegree(const Naive_H theHandle);

int32_t Naive_NurbsSurface_VDegree(const Naive_H theHandle);

bool Naive_NurbsSurface_PointAt(const Naive_H theHandle, const double theU, const double theV, Naive_Point3d_T *theP);

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

Naive_H Naive_BndShape_ConvexHull2D_New(const Naive_Point2d_T *thePoints, int32_t nbPoints, Naive_ConvexHull2D_Algorithm theAlgo);

void Naive_BndShape_ConvexHull2D_SetAlgorithm(Naive_H theHandle, Naive_ConvexHull2D_Algorithm theAlgo);

void Naive_BndShape_ConvexHull2D_Perform(Naive_H theHandle);

void Naive_BndShape_ConvexHull2D_Add(Naive_H theHandle, Naive_Point2d_T thePoint, bool thePerform);

Naive_Code Naive_BndShape_ConvexHull2D_Status(const Naive_H theHandle);

int32_t Naive_BndShape_ConvexHull2D_NbConvexPoints(const Naive_H theHandle);

Naive_Code Naive_BndShape_ConvexHull2D_ConvexIndices(const Naive_H theHandle, int32_t *theConvexIndices);

Naive_Code Naive_BndShape_ConvexHull2D_ConvexPoints(const Naive_H theHandle, Naive_Point2d_T *theConvexIndices);

void Naive_BndShape_ConvexHull2D_Release(Naive_H theHandle);

Naive_H Naive_BndShape_EnclosingDisc_New();

void Naive_BndShape_EnclosingDisc_Rebuild(Naive_H theHandle, int32_t nbPoints, const Naive_Point2d_T *thePoints);

bool Naive_BndShape_EnclosingDisc_Circle(const Naive_H theHandle, Naive_Point2d_T *theOrigin, double *theR);

void Naive_BndShape_EnclosingDisc_Release(Naive_H theHandle);

/// }}}

/// Tessellation {{{

Naive_H Naive_Tessellation_TetraSphere(const Naive_Point3d_T *theCenter, double theRadius, int32_t theLevel);

/// }}}

/// Release {{{

void Naive_Release_Int32Array(const int32_t *theArray);

void Naive_Release_DoubleArray(const double *theArray);

/// }}}
]]

  self.NS = ffi.load(getDylibPath("NaiveCGL"))
end

--------------------------------------------------------------------------------
--                                 LuaOCCT                                    --
--------------------------------------------------------------------------------

local gp_Pnt = LuaOCCT.gp.gp_Pnt
local Poly_Triangle = LuaOCCT.Poly.Poly_Triangle
local Poly_Triangulation = LuaOCCT.Poly.Poly_Triangulation

--------------------------------------------------------------------------------
--                            Naive_NurbsCurve                                --
--------------------------------------------------------------------------------

---@class naivecgl.Naive_NurbsCurve
---@field private myH ffi.cdata*
naivecgl.Naive_NurbsCurve = {}

---@private
naivecgl.Naive_NurbsCurve.__index = naivecgl.Naive_NurbsCurve

---Constructor.
---@param thePoles number[][]
---@param theWeights number[]
---@param theKnots number[]
---@param theMults integer[]
---@param theDegree integer
---@return naivecgl.Naive_NurbsCurve
function naivecgl.Naive_NurbsCurve.new(thePoles, theWeights, theKnots, theMults, theDegree)
  local nbPoles = #thePoles
  local aPoles = ffi.new("Naive_Point3d_T[?]", nbPoles)
  for i = 1, nbPoles do
    aPoles[i - 1].x = thePoles[i][1]
    aPoles[i - 1].y = thePoles[i][2]
    aPoles[i - 1].z = thePoles[i][3]
  end
  local nbWeights = #theWeights
  local aWeights = ffi.new("double[?]", nbWeights)
  for i = 1, nbWeights do
    aWeights[i - 1] = theWeights[i];
  end
  local nbKnots = #theKnots
  local aKnots = ffi.new("double[?]", nbKnots)
  for i = 1, nbKnots do
    aKnots[i - 1] = theKnots[i]
  end
  local nbMults = #theMults
  local aMults = ffi.new("int[?]", nbMults)
  for i = 1, nbMults do
    aMults[i - 1] = theMults[i]
  end
  local aH = naivecgl.NS.Naive_NurbsCurve_New(
    nbPoles, aPoles, nbWeights, aWeights,
    nbKnots, aKnots, nbMults, aMults,
    theDegree)

  local nurbsCurve = {
    myH = ffi.gc(aH, function(theHandle)
      naivecgl.NS.Naive_NurbsCurve_Release(theHandle)
    end)
  }
  setmetatable(nurbsCurve, naivecgl.Naive_NurbsCurve)
  return nurbsCurve
end

---Point at parameter |theT|.
---@param theT number
---@return gp_Pnt?
function naivecgl.Naive_NurbsCurve:PointAt(theT)
  local aP = ffi.new("Naive_Point3d_T")
  if naivecgl.NS.Naive_NurbsCurve_PointAt(self.myH, theT, aP) then
    return gp_Pnt(aP.x, aP.y, aP.z)
  end
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
---@param thePoles number[][][]
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
  local aPoles = ffi.new("Naive_Point3d_T[?]", nbUP * nbVP)
  for i, p in ipairs(aFlatPoles) do
    aPoles[i - 1].x = p[1]
    aPoles[i - 1].y = p[2]
    aPoles[i - 1].z = p[3]
  end

  local nbUW, nbVW, aFlatWeights = flattenArray2(theWeights)
  local aWeights = ffi.new("double[?]", nbUW * nbVW)
  for i, w in ipairs(aFlatWeights) do
    aWeights[i - 1] = w
  end

  local nbUKnots = #theUKnots
  local aUKnots = ffi.new("double[?]", nbUKnots)
  for i, k in ipairs(theUKnots) do
    aUKnots[i - 1] = k
  end

  local nbVKnots = #theVKnots
  local aVKnots = ffi.new("double[?]", nbVKnots)
  for i, k in ipairs(theVKnots) do
    aVKnots[i - 1] = k
  end

  local nbUMults = #theUMults
  local aUMults = ffi.new("int32_t[?]", nbUMults)
  for i, k in ipairs(theUMults) do
    aUMults[i - 1] = k
  end

  local nbVMults = #theVMults
  local aVMults = ffi.new("int32_t[?]", nbVMults)
  for i, k in ipairs(theVMults) do
    aVMults[i - 1] = k
  end

  local aH = naivecgl.NS.Naive_NurbsSurface_New(
    nbUP, nbVP, aPoles,
    nbUW, nbVW, aWeights,
    nbUKnots, aUKnots, nbVKnots, aVKnots,
    nbUMults, aUMults, nbVMults, aVMults,
    theUDegree, theVDegree)

  local nurbsSurface = {
    myH = ffi.gc(aH, function(theHandle)
      naivecgl.NS.Naive_NurbsSurface_Release(theHandle)
    end)
  }
  setmetatable(nurbsSurface, naivecgl.Naive_NurbsSurface)
  return nurbsSurface
end

function naivecgl.Naive_NurbsSurface:PointAt(theU, theV)
  local aP = ffi.new("Naive_Point3d_T")
  if naivecgl.NS.Naive_NurbsSurface_PointAt(self.myH, theU, theV, aP) then
    return gp_Pnt(aP.x, aP.y, aP.z)
  end
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
---@param theNodes number[][]
---@param theTris integer[][] 1-indexed
---@return naivecgl.Naive_Poly
function naivecgl.Naive_Poly.new(theNodes, theTris)
  local nbNodes = #theNodes
  local aNodes = ffi.new("Naive_Point3d_T[?]", nbNodes)
  for i = 1, nbNodes do
    aNodes[i - 1].x = theNodes[i][1]
    aNodes[i - 1].y = theNodes[i][2]
    aNodes[i - 1].z = theNodes[i][3]
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
---@return number[][]
function naivecgl.Naive_Poly:Vertices()
  local nbVertices = self:NbVertices()
  local aVertices = ffi.new("Naive_Point3d_T[?]", nbVertices)
  naivecgl.NS.Naive_Poly_Vertices(self.myH, aVertices)

  local aRes = {}
  for i = 1, nbVertices do
    aRes[i] = {
      aVertices[i - 1].x,
      aVertices[i - 1].y,
      aVertices[i - 1].z,
    }
  end

  return aRes
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
---@param thePoints number[][]
---@param theAlgo any
---@return naivecgl.bndshape.ConvexHull2D
function naivecgl.bndshape.ConvexHull2D.new(thePoints, theAlgo)
  theAlgo = theAlgo or naivecgl.NS.Naive_ConvexHull2D_Quickhull

  local nbPoints = #thePoints
  local aPoints = ffi.new("Naive_Point2d_T[?]", nbPoints)
  for i = 1, nbPoints do
    aPoints[i - 1].x = thePoints[i][1]
    aPoints[i - 1].y = thePoints[i][2]
  end

  local aH = naivecgl.NS.Naive_BndShape_ConvexHull2D_New(aPoints, nbPoints, theAlgo)

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
---@return unknown
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
---@param thePoints number[][]
function naivecgl.bndshape.EnclosingDisc:ReBuild(thePoints)
  local nbPoints = #thePoints;
  local aPoints = ffi.new("Naive_Point2d_T[?]", nbPoints)
  for i = 1, nbPoints do
    aPoints[i - 1].x = thePoints[i][1]
    aPoints[i - 1].y = thePoints[i][2]
  end

  naivecgl.NS.Naive_BndShape_EnclosingDisc_Rebuild(self.myH, nbPoints, aPoints)
end

---Get the cicle.
---@return boolean
---@return number origin.x
---@return number origin.y
---@return number r
function naivecgl.bndshape.EnclosingDisc:Circle()
  local anOrigin = ffi.new("Naive_Point2d_T")
  local aR = ffi.new("double[1]", 0)
  local ok = naivecgl.NS.Naive_BndShape_EnclosingDisc_Circle(self.myH, anOrigin, aR)
  return ok, anOrigin.x, anOrigin.y, aR[0]
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
---@param theCenter gp_Pnt
---@param theRadius number
---@param theLevel integer
---@return Poly_Triangulation?
function naivecgl.tessellation.Naive_Tessellation_TetraSphere(theCenter, theRadius, theLevel)
  if not naivecgl.NS then return end

  local aCenter = ffi.new("Naive_Point3d_T", { theCenter:X(), theCenter:Y(), theCenter:Z() })
  local aHandle = naivecgl.NS.Naive_Tessellation_TetraSphere(aCenter, theRadius, theLevel)

  if not aHandle then
    return
  end

  local tetrasphere = naivecgl.Naive_Poly.take(aHandle)
  local aVertices = tetrasphere:Vertices()
  local aTriangles = tetrasphere:Triangles()
  tetrasphere:Dispose()

  local vList = {}
  for i, v in ipairs(aVertices) do
    vList[i] = gp_Pnt(v[1], v[2], v[3])
  end

  local tList = {}
  for i, t in ipairs(aTriangles) do
    tList[i] = Poly_Triangle(t[1], t[2], t[3])
  end

  return Poly_Triangulation(vList, tList)
end

--------------------------------------------------------------------------------
--                                 Export                                     --
--------------------------------------------------------------------------------

naivecgl:init()

return naivecgl
