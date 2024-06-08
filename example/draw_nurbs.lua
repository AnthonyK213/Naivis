local naivecgl = require("naivecgl")

local BRepBuilderAPI_MakeEdge = LuaOCCT.BRepBuilderAPI.BRepBuilderAPI_MakeEdge
local BRepBuilderAPI_MakeVertex = LuaOCCT.BRepBuilderAPI.BRepBuilderAPI_MakeVertex
local BRepBuilderAPI_MakeFace = LuaOCCT.BRepBuilderAPI.BRepBuilderAPI_MakeFace
local BRepPrimAPI_MakeSphere = LuaOCCT.BRepPrimAPI.BRepPrimAPI_MakeSphere
local Geom_BSplineCurve = LuaOCCT.Geom.Geom_BSplineCurve
local Geom_BSplineSurface = LuaOCCT.Geom.Geom_BSplineSurface
local gp = LuaOCCT.gp.gp
local gp_Ax2 = LuaOCCT.gp.gp_Ax2
local gp_Pnt = LuaOCCT.gp.gp_Pnt
local gp_Vec = LuaOCCT.gp.gp_Vec
local LODoc_Attribute = LuaOCCT.LODoc.LODoc_Attribute
local Quantity_Color = LuaOCCT.Quantity.Quantity_Color
local Ghost_Attribute = Naivis.Ghost.Ghost_Attribute
local Ghost_AttrOfVector = Naivis.Ghost.Ghost_AttrOfVector
local P3 = naivecgl.Naive_XYZ

local doc = Naivis.NaiveDoc.ActiveDoc
doc:Objects():Clear(false)
if not _G.__ghost__ then _G.__ghost__ = Naivis.Ghost.NewDocument() end
__ghost__:Clear(false)

if Naivis then
  Naivis.NaiveApp.Clear()
end

---
---@param theXYZ naivecgl.Naive_XYZ
---@return gp_Pnt
local function xyz_to_pnt(theXYZ)
  return gp_Pnt(theXYZ:X(), theXYZ:Y(), theXYZ:Z())
end

---
---@param theCrv naivecgl.Naive_NurbsCurve
local function display_nurbs_curve(theCrv, theN)
  theN = theN or 64
  local nbPoles = theCrv:NbPoles()
  local aPoles = theCrv:Poles()
  -- ---@type naivecgl.Naive_XYZ[]
  -- local aPoles = {}
  local t0 = theCrv:FirstParameter()
  local t1 = theCrv:LastParameter()

  local pAttr = Ghost_Attribute()
  pAttr:SetColor(Quantity_Color(LuaOCCT.Quantity.Quantity_NameOfColor.Quantity_NOC_CYAN))

  -- Draw control points.

  for i = 1, nbPoles do
    local aPole = aPoles:Value(i)
    local pole = BRepPrimAPI_MakeSphere(xyz_to_pnt(aPole), 0.3):Shape()
    __ghost__:AddShape(pole, pAttr, false)
  end

  -- Draw control polygon.

  local cpAttr = Ghost_Attribute()
  cpAttr:SetColor(Quantity_Color(LuaOCCT.Quantity.Quantity_NameOfColor.Quantity_NOC_MAGENTA))
  for i = 1, nbPoles - 1 do
    local aThis = aPoles:Value(i)
    local aNext = aPoles:Value(i + 1)
    local cp = BRepBuilderAPI_MakeEdge(xyz_to_pnt(aThis), xyz_to_pnt(aNext)):Edge()
    __ghost__:AddShape(cp, cpAttr, false)
  end

  -- Draw curve.

  local prevPnt = xyz_to_pnt(theCrv:PointAt(t0))
  local segAttr = Ghost_Attribute()
  segAttr:SetColor(Quantity_Color(LuaOCCT.Quantity.Quantity_NameOfColor.Quantity_NOC_BLACK))
  for i = 1, theN do
    local aPnt = xyz_to_pnt(theCrv:PointAt(t0 + i * (t1 - t0) / theN))
    if aPnt then
      local aSeg = BRepBuilderAPI_MakeEdge(prevPnt, aPnt):Shape()
      __ghost__:AddShape(aSeg, segAttr, false)
      prevPnt = aPnt
    else
      error("Invalid point?")
    end
  end
  
  local aKnots = theCrv:Knots()

  -- Draw knots.
  for i = 1, theCrv:NbKnots() do
    local aPnt = xyz_to_pnt(theCrv:PointAt(aKnots:Value(i)))
    if aPnt then
      local aVert = BRepBuilderAPI_MakeVertex(aPnt):Shape()
      __ghost__:AddShape(aVert, Ghost_Attribute(), false)
    end
  end
end

---
---@param thePoles naivecgl.Naive_XYZ[]
---@param theWeights number[]
---@param theKnots number[]
---@param theMults integer[]
---@param theDegree integer
---@param theCheck boolean?
---@param theN integer?
---@return naivecgl.Naive_NurbsCurve
---@return Geom_BSplineCurve
local function make_nurbs_curve(thePoles, theWeights, theKnots, theMults, theDegree, theCheck, theN)
  local aNurbsCurve = naivecgl.Naive_NurbsCurve.new(thePoles, theWeights, theKnots, theMults, theDegree)
  display_nurbs_curve(aNurbsCurve, theN)

  if theCheck then
    local poles = {}
    for i, p in ipairs(thePoles) do
      poles[i] = gp_Pnt(p:X(), p:Y(), p:Z())
    end
    local aBS = Geom_BSplineCurve(poles, theWeights, theKnots, theMults, theDegree, false, false)
    local aShape = BRepBuilderAPI_MakeEdge(aBS):Edge()
    local anAttr = Ghost_Attribute()
    anAttr:SetColor(Quantity_Color(LuaOCCT.Quantity.Quantity_NameOfColor.Quantity_NOC_RED));
    __ghost__:AddShape(aShape, anAttr, false)

    return aNurbsCurve, aBS
  end

  return aNurbsCurve, nil
end

local function draw_nurbs_curve(N)
  N = N or 64
  local S = math.sqrt(0.5)

  local aPoles = {
    P3(10, 0, 0),
    P3(10, 10, 0),
    P3(0, 10, 0),
    P3(-10, 10, 0),
    P3(-10, 0, 0),
    P3(-10, -10, 0),
    P3(0, -10, 0),
    P3(10, -10, 0),
    P3(10, 0, 0)
  }
  local aWeights = { 1, S, 1, S, 1, S, 1, S, 1 }
  local aKnots = { 0, 0.25, 0.5, 0.75, 1 }
  local aMults = { 3, 2, 2, 2, 3 }
  local aDegree = 2
  local aNurbsCurve, aBS = make_nurbs_curve(aPoles, aWeights, aKnots, aMults, aDegree, true)

  local vecRatio = 0.1
  local t = 0.42
  local ok, aD = aNurbsCurve:DerivativeAt(t, 2)
  if ok then
    local aP = gp_Pnt(aD:Value(1):X(), aD:Value(1):Y(), aD:Value(1):Z())

    local anAttr = Ghost_AttrOfVector()
    anAttr:SetColor(Quantity_Color(LuaOCCT.Quantity.Quantity_NameOfColor.Quantity_NOC_BLUE))
    for i = 2, aD:Size() do
      local aV = gp_Vec(aD:Value(i):X(), aD:Value(i):Y(), aD:Value(i):Z())
      __ghost__:AddVector(aV:Multiplied(vecRatio), aP, anAttr, false)
    end
  end

  -- Check!
  local poles = {}
  for i, p in ipairs(aPoles) do
    poles[i] = gp_Pnt(p:X(), p:Y(), p:Z())
  end
  local aShape = BRepBuilderAPI_MakeEdge(aBS):Edge()
  local anAttr = Ghost_Attribute()
  anAttr:SetColor(Quantity_Color(LuaOCCT.Quantity.Quantity_NameOfColor.Quantity_NOC_RED));
  local aBS_2 = aBS:DN(t, 2)
  __ghost__:AddShape(aShape, anAttr, false)
  __ghost__:AddVector(aBS_2:Multiplied(vecRatio), aBS:Value(t), Ghost_AttrOfVector(), false)
end

local function draw_nurbs_surface(N)
  N = N or 16

  local aUDegree = 2
  local aVDegree = 2
  local aPoles = {
    { P3(15, -10, 3),  P3(15, 0, 9), P3(15, 10, 2) },
    { P3(25, -10, 1),  P3(25, 0, 0), P3(25, 10, -6) },
    { P3(35, -10, -4), P3(35, 0, 1), P3(35, 10, 5) },
  }
  local aWeights = {
    { 0.3, 1.4, 2.9 },
    { 1.2, 1.2, 1.2 },
    { 0.8, 1.1, 1.8 },
  }
  local aUKnots = { 0, 1 }
  local aVKnots = { 0, 1 }
  local aUMults = { 3, 3 }
  local aVMults = { 3, 3 }
  local aNurbsSurface = naivecgl.Naive_NurbsSurface.new(
    aPoles, aWeights,
    aUKnots, aVKnots, aUMults, aVMults,
    aUDegree, aVDegree)

  local pAttr = Ghost_Attribute()
  pAttr:SetColor(Quantity_Color(LuaOCCT.Quantity.Quantity_NameOfColor.Quantity_NOC_CYAN))

  for _, c in ipairs(aPoles) do
    for _, p in ipairs(c) do
      local pole = BRepPrimAPI_MakeSphere(gp_Pnt(p:X(), p:Y(), p:Z()), 0.3):Shape()
      __ghost__:AddShape(pole, pAttr, false)
    end
  end

  local cpAttr = Ghost_Attribute()
  cpAttr:SetColor(Quantity_Color(LuaOCCT.Quantity.Quantity_NameOfColor.Quantity_NOC_MAGENTA))
  for i = 1, #aPoles do
    local aColThis = aPoles[i]
    local aColNext = aPoles[i + 1]
    for j = 1, #aColThis do
      local aThis = aColThis[j]
      local aNext = aColThis[j + 1]
      if aNext then
        local cp = BRepBuilderAPI_MakeEdge(
          gp_Pnt(aThis:X(), aThis:Y(), aThis:Z()),
          gp_Pnt(aNext:X(), aNext:Y(), aNext:Z())):Edge()
        __ghost__:AddShape(cp, cpAttr, false)
      end
      if aColNext then
        aNext = aColNext[j]
        local cp = BRepBuilderAPI_MakeEdge(
          gp_Pnt(aThis:X(), aThis:Y(), aThis:Z()),
          gp_Pnt(aNext:X(), aNext:Y(), aNext:Z())):Edge()
        __ghost__:AddShape(cp, cpAttr, false)
      end
    end
  end

  for i = 0, N do
    for j = 0, N do
      local aPnt = aNurbsSurface:PointAt(i / N, j / N)
      if aPnt then
        local aVert = BRepBuilderAPI_MakeVertex(gp_Pnt(aPnt:X(), aPnt:Y(), aPnt:Z())):Vertex()
        doc:Objects():AddShape(aVert, LODoc_Attribute(), false)
      end
    end
  end

  local u = 0.1
  local v = 0.4
  local ok, aD = aNurbsSurface:Evaluate(u, v, 2)
  if ok then
    local aP = gp_Pnt(aD:Value(1):X(), aD:Value(1):Y(), aD:Value(1):Z())

    local anAttr = Ghost_AttrOfVector()
    anAttr:SetColor(Quantity_Color(LuaOCCT.Quantity.Quantity_NameOfColor.Quantity_NOC_BLUE))
    for i = 2, aD:Size() do
      local aV = gp_Vec(aD:Value(i):X(), aD:Value(i):Y(), aD:Value(i):Z())
      __ghost__:AddVector(aV, aP, anAttr, false)
    end
  end

  -- Check!
  local poles = {}
  for i, col in ipairs(aPoles) do
    local c = {}
    for j, item in ipairs(col) do
      c[j] = gp_Pnt(item:X(), item:Y(), item:Z())
    end
    poles[i] = c
  end
  local aBS = Geom_BSplineSurface(poles, aWeights, aUKnots, aVKnots, aUMults, aVMults, aUDegree, aVDegree, false, false)
  local aShape = BRepBuilderAPI_MakeFace(aBS, 1e-2):Face()
  local anAttr = Ghost_Attribute()
  anAttr:SetColor(Quantity_Color(LuaOCCT.Quantity.Quantity_NameOfColor.Quantity_NOC_RED));
  local aBS_2 = aBS:DN(u, v, 1, 1)
  __ghost__:AddShape(aShape, anAttr, false)
  __ghost__:AddVector(aBS_2, aBS:Value(u, v), Ghost_AttrOfVector(), false)
end

local function nurbs_curve_insert_knot()
  local aPoles = {
    P3(-10, 34, 6),
    P3(-9, 15, -6),
    P3(-6, 20, 1),
    P3(0, 26, 2),
    P3(4, 17, -3),
    P3(10, 21, 10),
  }
  local aWeights = { 1.5, 2, 0.5, 1.1, 0.1, 1 }
  local aKnots = { 0, 1, 2, 3 }
  local aMults = { 4, 1, 1, 4 }
  local aDegree = 3
  local aNurbsCurve, aBS = make_nurbs_curve(aPoles, aWeights, aKnots, aMults, aDegree)
  aNurbsCurve:InsertKnot(0.7, 2)
  -- aNurbsCurve:IncreaseMultiplicity(2, 2)
  -- __ghost__:Clear(false)
  display_nurbs_curve(aNurbsCurve)

  local t = 0.7
  local aC = aNurbsCurve:CurvatureAt(t)
  local aP = aNurbsCurve:PointAt(t)
  if aC and aP then
    local aV = gp_Vec(aC:X(), aC:Y(), aC:Z())
    local o = xyz_to_pnt(aP)
    local r = 1 / aV:Magnitude()
    aV:Multiply(r * r)
    o:Translate(aV)
    local aS = BRepPrimAPI_MakeSphere(o, r):Shape()
    __ghost__:AddShape(aS, Ghost_Attribute(), false)
  end
end

draw_nurbs_curve()
draw_nurbs_surface()
nurbs_curve_insert_knot()

doc:UpdateView()
