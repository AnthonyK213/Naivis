local naivecgl = {}
naivecgl.bndshape = {}
naivecgl.tessellation = {}

--------------------------------------------------------------------------------
--                                  FFI                                       --
--------------------------------------------------------------------------------

local ffi = require("ffi")

---@type ffi.namespace*
local dylib_ = nil

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
  if dylib_ then
    return
  end

  ffi.cdef [[
typedef void *Naive_Handle;

typedef enum {
  Naive_Ok,
  Naive_Err,
} Naive_Code;

typedef enum {
  Naive_ConvexHull2D_Quickhull,
  Naive_ConvexHull2D_Incremental,
  Naive_ConvexHull2D_GrahamScan,
} Naive_ConvexHull2D_Algorithm;

typedef enum {
  Naive_ConvexHull2D_Done,
  Naive_ConvexHull2D_InitDone,
  Naive_ConvexHull2D_Failed,
  Naive_ConvexHull2D_InsufficientPoint,
  Naive_ConvexHull2D_PointsAreColinear,
  Naive_ConvexHull2D_AlgoNotImplemented,
} Naive_ConvexHull2D_Status;

typedef enum {
  Naive_ConvexHull3D_Quickhull,
  Naive_ConvexHull3D_Incremental,
  Naive_ConvexHull3D_DivideAndConquer,
} Naive_ConvexHull3D_Algorithm;

typedef enum {
  Naive_ConvexHull3D_Done,
  Naive_ConvexHull3D_InitDone,
  Naive_ConvexHull3D_Failed,
  Naive_ConvexHull3D_InsufficientPoint,
  Naive_ConvexHull3D_PointsAreColinear,
  Naive_ConvexHull3D_PointsAreCoplanar,
  Naive_ConvexHull3D_AlgoNotImplemented,
} Naive_ConvexHull3D_Status;

typedef struct { double x, y; } Naive_Vector2d_T;
typedef struct { double x, y, z; } Naive_Vector3d_T;
typedef Naive_Vector2d_T Naive_Point2d_T;
typedef Naive_Vector3d_T Naive_Point3d_T;
typedef struct { double t0, t1; } Naive_Interval_T;
typedef struct { Naive_Point3d_T origin; Naive_Vector3d_T xAxis, yAxis; } Naive_Plane_T;
typedef struct { Naive_Point3d_T from, to; } Naive_Line_T;
typedef struct { int32_t n0, n1, n2; } Naive_Triangle_T;

/// Naive_Poly {{{

Naive_Handle Naive_Poly_New(const int32_t nbVertices, const Naive_Point3d_T *theVertices, const int32_t nbTriangles, const Naive_Triangle_T *theTriangles);

int32_t Naive_Poly_NbVertices(const Naive_Handle theHandle);

void Naive_Poly_Vertices(const Naive_Handle theHandle, Naive_Point3d_T *theVertices);

int32_t Naive_Poly_NbTriangles(const Naive_Handle theHandle);

void Naive_Poly_Triangles(const Naive_Handle theHandle, Naive_Triangle_T *theTriangles);

void Naive_Poly_Release(Naive_Handle theHandle);

/// }}}

/// BndShape {{{

Naive_Handle Naive_BndShape_ConvexHull2D_New(const Naive_Point2d_T *thePoints, int32_t nbPoints, Naive_ConvexHull2D_Algorithm theAlgo);

void Naive_BndShape_ConvexHull2D_SetAlgorithm(Naive_Handle theHandle, Naive_ConvexHull2D_Algorithm theAlgo);

void Naive_BndShape_ConvexHull2D_Perform(Naive_Handle theHandle);

void Naive_BndShape_ConvexHull2D_Add(Naive_Handle theHandle, Naive_Point2d_T thePoint, bool thePerform);

Naive_ConvexHull2D_Status Naive_BndShape_ConvexHull2D_Status(const Naive_Handle theHandle);

int32_t Naive_BndShape_ConvexHull2D_NbConvexPoints(const Naive_Handle theHandle);

Naive_Code Naive_BndShape_ConvexHull2D_ConvexIndices(const Naive_Handle theHandle, int32_t *theConvexIndices);

Naive_Code Naive_BndShape_ConvexHull2D_ConvexPoints(const Naive_Handle theHandle, Naive_Point2d_T *theConvexIndices);

void Naive_BndShape_ConvexHull2D_Release(Naive_Handle theHandle);

Naive_Handle Naive_BndShape_EnclosingDisc_New();

void Naive_BndShape_EnclosingDisc_Rebuild(Naive_Handle theHandle, int32_t nbPoints, const Naive_Point2d_T *thePoints);

int32_t Naive_BndShape_EnclosingDisc_Circle(const Naive_Handle theHandle, Naive_Point2d_T *theOrigin, double *theR);

void Naive_BndShape_EnclosingDisc_Release(Naive_Handle theHandle);

/// }}}

/// Tessellation {{{

Naive_Handle Naive_Tessellation_TetraSphere(const Naive_Point3d_T *theCenter, double theRadius, int32_t theLevel);

/// }}}

/// Release {{{

void Naive_Release_Int32Array(int32_t *theArray);

void Naive_Release_DoubleArray(double *theArray);

/// }}}
]]

  dylib_ = ffi.load(getDylibPath("NaiveCGL"))

  if dylib_ then
    self:initEnums()
  else
    error("Failed to initialize NaiveCGL")
  end
end

---@private
function naivecgl:initEnums()
  if not dylib_ then
    return
  end

  self.Naive_Ok = dylib_.Naive_Ok
  self.Naive_Err = dylib_.Naive_Err

  self.Naive_ConvexHull2D_Quickhull = dylib_.Naive_ConvexHull2D_Quickhull
  self.Naive_ConvexHull2D_Incremental = dylib_.Naive_ConvexHull2D_Incremental
  self.Naive_ConvexHull2D_GrahamScan = dylib_.Naive_ConvexHull2D_GrahamScan

  self.Naive_ConvexHull2D_Done = dylib_.Naive_ConvexHull2D_Done
  self.Naive_ConvexHull2D_InitDone = dylib_.Naive_ConvexHull2D_InitDone
  self.Naive_ConvexHull2D_Failed = dylib_.Naive_ConvexHull2D_Failed
  self.Naive_ConvexHull2D_InsufficientPoint = dylib_.Naive_ConvexHull2D_InsufficientPoint
  self.Naive_ConvexHull2D_PointsAreColinear = dylib_.Naive_ConvexHull2D_PointsAreColinear
  self.Naive_ConvexHull2D_AlgoNotImplemented = dylib_.Naive_ConvexHull2D_AlgoNotImplemented

  self.Naive_ConvexHull3D_Quickhull = dylib_.Naive_ConvexHull3D_Quickhull
  self.Naive_ConvexHull3D_Incremental = dylib_.Naive_ConvexHull3D_Incremental
  self.Naive_ConvexHull3D_DivideAndConquer = dylib_.Naive_ConvexHull3D_DivideAndConquer

  self.Naive_ConvexHull3D_Done = dylib_.Naive_ConvexHull3D_Done
  self.Naive_ConvexHull3D_InitDone = dylib_.Naive_ConvexHull3D_InitDone
  self.Naive_ConvexHull3D_Failed = dylib_.Naive_ConvexHull3D_Failed
  self.Naive_ConvexHull3D_InsufficientPoint = dylib_.Naive_ConvexHull3D_InsufficientPoint
  self.Naive_ConvexHull3D_PointsAreColinear = dylib_.Naive_ConvexHull3D_PointsAreColinear
  self.Naive_ConvexHull3D_PointsAreCoplanar = dylib_.Naive_ConvexHull3D_PointsAreCoplanar
  self.Naive_ConvexHull3D_AlgoNotImplemented = dylib_.Naive_ConvexHull3D_AlgoNotImplemented
end

--------------------------------------------------------------------------------
--                                 LuaOCCT                                    --
--------------------------------------------------------------------------------

local gp_Pnt = LuaOCCT.gp.gp_Pnt
local Poly_Triangle = LuaOCCT.Poly.Poly_Triangle
local Poly_Triangulation = LuaOCCT.Poly.Poly_Triangulation

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

  local aH = dylib_.Naive_Poly_New(nbNodes, aNodes, nbTris, aTris)
  return naivecgl.Naive_Poly.take(aH)
end

---@param theH ffi.cdata*
---@return naivecgl.Naive_Poly
function naivecgl.Naive_Poly.take(theH)
  local poly = {
    myH = ffi.gc(theH, function(theHandle)
      dylib_.Naive_Poly_Release(theHandle)
    end),
  }
  setmetatable(poly, naivecgl.Naive_Poly)
  return poly
end

---Number of vertices.
---@return integer
function naivecgl.Naive_Poly:NbVertices()
  return dylib_.Naive_Poly_NbVertices(self.myH)
end

---Number of triangles.
---@return integer
function naivecgl.Naive_Poly:NbTriangles()
  return dylib_.Naive_Poly_NbTriangles(self.myH)
end

---Vertices.
---@return number[][]
function naivecgl.Naive_Poly:Vertices()
  local nbVertices = self:NbVertices()
  local aVertices = ffi.new("Naive_Point3d_T[?]", nbVertices)
  dylib_.Naive_Poly_Vertices(self.myH, aVertices)

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
  dylib_.Naive_Poly_Triangles(self.myH, aTriangles)

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
    dylib_.Naive_Poly_Release(ffi.gc(self.myH, nil))
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
  theAlgo = theAlgo or naivecgl.Naive_ConvexHull2D_Quickhull

  local nbPoints = #thePoints
  local aPoints = ffi.new("Naive_Point2d_T[?]", nbPoints)
  for i = 1, nbPoints do
    aPoints[i - 1].x = thePoints[i][1]
    aPoints[i - 1].y = thePoints[i][2]
  end

  local aH = dylib_.Naive_BndShape_ConvexHull2D_New(aPoints, nbPoints, theAlgo)

  local o = {
    myH = ffi.gc(aH, function(theHandle)
      dylib_.Naive_BndShape_ConvexHull2D_Release(theHandle)
    end)
  }
  setmetatable(o, naivecgl.bndshape.ConvexHull2D)
  return o
end

function naivecgl.bndshape.ConvexHull2D:SetAlgorithm(theAlgo)
  dylib_.Naive_BndShape_ConvexHull2D_SetAlgorithm(self.myH, theAlgo)
end

---Perform the algorithm.
function naivecgl.bndshape.ConvexHull2D:Perform()
  dylib_.Naive_BndShape_ConvexHull2D_Perform(self.myH)
end

---Add a point.
---@param thePnt table
---@param thePerform boolean?
function naivecgl.bndshape.ConvexHull2D:Add(thePnt, thePerform)
  if thePerform == nil then
    thePerform = false
  end

  local aPnt = ffi.new("Naive_Point2d_T", thePnt)
  dylib_.Naive_BndShape_ConvexHull2D_Add(self.myH, aPnt, thePerform)
end

---Get status.
---@return unknown
function naivecgl.bndshape.ConvexHull2D:Status()
  return dylib_.Naive_BndShape_ConvexHull2D_Status(self.myH)
end

---Get the convex indices.
---@return integer[]
function naivecgl.bndshape.ConvexHull2D:ConvexIndices()
  local aRes = {}

  if self:Status() == naivecgl.Naive_ConvexHull2D_Done then
    local count = dylib_.Naive_BndShape_ConvexHull2D_NbConvexPoints(self.myH)
    local indices = ffi.new("int32_t[?]", count)
    dylib_.Naive_BndShape_ConvexHull2D_ConvexIndices(self.myH, indices)

    for i = 1, count do
      aRes[i] = indices[i - 1] + 1
    end
  end

  return aRes
end

function naivecgl.bndshape.ConvexHull2D:Dispose()
  if self.myH then
    dylib_.Naive_BndShape_ConvexHull2D_Release(ffi.gc(self.myH, nil))
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
  local aH = dylib_.Naive_BndShape_EnclosingDisc_New()

  local o = {
    myH = ffi.gc(aH, function(theHandle)
      dylib_.Naive_BndShape_EnclosingDisc_Release(theHandle)
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

  dylib_.Naive_BndShape_EnclosingDisc_Rebuild(self.myH, nbPoints, aPoints)
end

---Get the cicle.
---@return boolean
---@return number origin.x
---@return number origin.y
---@return number r
function naivecgl.bndshape.EnclosingDisc:Circle()
  local anOrigin = ffi.new("Naive_Point2d_T")
  local aR = ffi.new("double[1]", 0)
  local ok = dylib_.Naive_BndShape_EnclosingDisc_Circle(self.myH, anOrigin, aR)
  return ok == 1, anOrigin.x, anOrigin.y, aR[0]
end

function naivecgl.bndshape.EnclosingDisc:Dispose()
  if self.myH then
    dylib_.Naive_BndShape_EnclosingDisc_Release(ffi.gc(self.myH, nil))
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
  if not dylib_ then
    return
  end

  local aCenter = ffi.new("Naive_Point3d_T", { theCenter:X(), theCenter:Y(), theCenter:Z() })
  local aHandle = dylib_.Naive_Tessellation_TetraSphere(aCenter, theRadius, theLevel)

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
