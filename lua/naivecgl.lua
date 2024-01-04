local ffi = require("ffi")

local gp_Pnt = LuaOCCT.gp.gp_Pnt
local Poly_Triangle = LuaOCCT.Poly.Poly_Triangle
local Poly_Triangulation = LuaOCCT.Poly.Poly_Triangulation

local naivecgl = {}

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
typedef enum {
  Naive_Ok,
  Naive_Fail,
} Naive_Code;

typedef struct { double x, y; } Naive_Point2d_T;
typedef struct { double x, y, z; } Naive_Point3d_T;
typedef struct { int32_t n0, n1, n2; } Naive_Triangle_T;
typedef void Naive_Mesh;
typedef void Naive_Poly;

/// Naive_Poly {{{

int32_t Naive_Poly_NbVertices(const Naive_Poly *theHandle);

void Naive_Poly_Vertices(const Naive_Poly *theHandle, Naive_Point3d_T *theVertices);

int32_t Naive_Poly_NbTriangles(const Naive_Poly *theHandle);

void Naive_Poly_Triangles(const Naive_Poly *theHandle, Naive_Triangle_T *theTriangles);

void Naive_Poly_Release(Naive_Poly *theHandle);

/// }}}

/// BndShape {{{

Naive_Code Naive_BndShape_ConvexHull2D(const Naive_Point2d_T *thePoints, int32_t *theCount, int32_t **theConvexIndices);

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
    self.Naive_Ok = dylib_.Naive_Ok
    self.Naive_Fail = dylib_.Naive_Fail
  else
    error("Failed to initialize NaiveCGL")
  end
end

function naivecgl.Naive_BndShape_ConvexHull2D(thePoints)
  if not dylib_ then
    return
  end

  local nbPoints = #thePoints
  local aPoints = ffi.new("Naive_Point2d_T[?]", nbPoints)
  local aCount = ffi.new("int[1]", nbPoints)

  for i = 1, nbPoints do
    aPoints[i - 1].x = thePoints[i][1]
    aPoints[i - 1].y = thePoints[i][2]
  end

  local aConvexIndices = ffi.new("int*[1]", nil)

  local code = dylib_.Naive_BndShape_ConvexHull2D(aPoints, aCount, aConvexIndices)

  local result = {}

  if code == dylib_.Naive_Ok then
    local count = aCount[0]

    for i = 1, count do
      result[i] = aConvexIndices[0][i - 1] + 1
    end
  end

  dylib_.Naive_Release_Int32Array(aConvexIndices[0])

  return code, result
end

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
