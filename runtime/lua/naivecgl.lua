local ffi = require("ffi")

local gp_Pnt = LuaOCCT.gp.gp_Pnt
local Poly_Triangle = LuaOCCT.Poly.Poly_Triangle
local Poly_Triangulation = LuaOCCT.Poly.Poly_Triangulation

local naivecgl = {}

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

function naivecgl:init()
  if dylib_ then
    return
  end

  ffi.cdef [[
typedef void Naive_Mesh;
typedef void Naive_Poly;
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

typedef struct { double x, y; } Naive_Point2d_T;
typedef struct { double x, y, z; } Naive_Point3d_T;
typedef struct { int32_t n0, n1, n2; } Naive_Triangle_T;

/// Naive_Poly {{{

int32_t Naive_Poly_NbVertices(const Naive_Poly *theHandle);

void Naive_Poly_Vertices(const Naive_Poly *theHandle, Naive_Point3d_T *theVertices);

int32_t Naive_Poly_NbTriangles(const Naive_Poly *theHandle);

void Naive_Poly_Triangles(const Naive_Poly *theHandle, Naive_Triangle_T *theTriangles);

void Naive_Poly_Release(Naive_Poly *theHandle);

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

Naive_Poly *Naive_Tessellation_TetraSphere(const Naive_Point3d_T *theCenter, double theRadius, int32_t theLevel);

/// }}}

/// Release {{{

void Naive_Release_Int32Array(int32_t *theArray);

void Naive_Release_DoubleArray(double *theArray);
]]

  dylib_ = ffi.load(getDylibPath("NaiveCGL"))

  if dylib_ then
    self:initEnums()
  else
    error("Failed to initialize NaiveCGL")
  end
end

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

function naivecgl.Naive_BndShape_ConvexHull2D(thePoints)
  if not dylib_ then
    return
  end

  local nbPoints = #thePoints
  local aPoints = ffi.new("Naive_Point2d_T[?]", nbPoints)

  for i = 1, nbPoints do
    aPoints[i - 1].x = thePoints[i][1]
    aPoints[i - 1].y = thePoints[i][2]
  end

  local aCH2D = dylib_.Naive_BndShape_ConvexHull2D_New(aPoints, nbPoints, naivecgl.Naive_ConvexHull2D_Quickhull)
  dylib_.Naive_BndShape_ConvexHull2D_Perform(aCH2D)

  local code = dylib_.Naive_BndShape_ConvexHull2D_Status(aCH2D)

  local result = {}

  if code == naivecgl.Naive_ConvexHull2D_Done then
    local count = dylib_.Naive_BndShape_ConvexHull2D_NbConvexPoints(aCH2D)
    local indices = ffi.new("int32_t[?]", count)
    dylib_.Naive_BndShape_ConvexHull2D_ConvexIndices(aCH2D, indices)

    for i = 1, count do
      result[i] = indices[i - 1] + 1
    end
  end

  dylib_.Naive_BndShape_ConvexHull2D_Release(aCH2D)

  return code, result
end

---Enclosing disc.
---@param thePoints number[][]
---@return number? Origin.X
---@return number? Origin.Y
---@return number? R
function naivecgl.Naive_BndShape_EnclosingDisc(thePoints)
  if not dylib_ then
    return
  end

  local nbPoints = #thePoints;
  local aPoints = ffi.new("Naive_Point2d_T[?]", nbPoints)

  for i = 1, nbPoints do
    aPoints[i - 1].x = thePoints[i][1]
    aPoints[i - 1].y = thePoints[i][2]
  end

  local aDisc = dylib_.Naive_BndShape_EnclosingDisc_New()
  dylib_.Naive_BndShape_EnclosingDisc_Rebuild(aDisc, nbPoints, aPoints)

  local anOrigin = ffi.new("Naive_Point2d_T")
  local aR = ffi.new("double[1]", 0)

  local ok = dylib_.Naive_BndShape_EnclosingDisc_Circle(aDisc, anOrigin, aR)
  dylib_.Naive_BndShape_EnclosingDisc_Release(aDisc)

  if ok == 1 then
    return anOrigin.x, anOrigin.y, aR[0]
  else
    return
  end
end

---comment
---@param theCenter any
---@param theRadius any
---@param theLevel any
---@return Poly_Triangulation?
function naivecgl.Naive_Tessellation_TetraSphere(theCenter, theRadius, theLevel)
  if not dylib_ then
    return
  end

  local aCenter = ffi.new("Naive_Point3d_T", { theCenter.X_, theCenter.Y_, theCenter.Z_ })
  local tetrasphere = dylib_.Naive_Tessellation_TetraSphere(aCenter, theRadius, theLevel)

  if tetrasphere == nil then
    return nil;
  end

  local nbVertices = dylib_.Naive_Poly_NbVertices(tetrasphere)
  local nbTriangles = dylib_.Naive_Poly_NbTriangles(tetrasphere)
  local aVertices = ffi.new("Naive_Point3d_T[?]", nbVertices)
  local aTriangles = ffi.new("Naive_Triangle_T[?]", nbTriangles)

  dylib_.Naive_Poly_Vertices(tetrasphere, aVertices)
  dylib_.Naive_Poly_Triangles(tetrasphere, aTriangles)

  local vList, tList = {}, {}

  for i = 1, nbVertices do
    local aVert = aVertices[i - 1]
    vList[i] = gp_Pnt(aVert.x, aVert.y, aVert.z)
  end

  for i = 1, nbTriangles do
    local aTri = aTriangles[i - 1]
    tList[i] = Poly_Triangle(aTri.n0 + 1, aTri.n1 + 1, aTri.n2 + 1)
  end

  dylib_.Naive_Poly_Release(tetrasphere)

  return Poly_Triangulation(vList, tList);
end

naivecgl:init()

return naivecgl
