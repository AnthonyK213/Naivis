local naivecgl = require("naivecgl")

local BRepBuilderAPI_MakeEdge = LuaOCCT.BRepBuilderAPI.BRepBuilderAPI_MakeEdge
local BRepBuilderAPI_MakeVertex = LuaOCCT.BRepBuilderAPI.BRepBuilderAPI_MakeVertex
local BRepPrimAPI_MakeSphere = LuaOCCT.BRepPrimAPI.BRepPrimAPI_MakeSphere
local Geom_Circle = LuaOCCT.Geom.Geom_Circle
local Geom_BSplineCurve = LuaOCCT.Geom.Geom_BSplineCurve
local gp = LuaOCCT.gp.gp
local gp_Ax2 = LuaOCCT.gp.gp_Ax2
local gp_Pnt = LuaOCCT.gp.gp_Pnt
local LODoc_Attribute = LuaOCCT.LODoc.LODoc_Attribute
local Quantity_Color = LuaOCCT.Quantity.Quantity_Color
local Ghost_Attribute = Naivis.Ghost.Ghost_Attribute
local P3 = naivecgl.Naive_Point3d

local doc = Naivis.NaiveDoc.ActiveDoc
doc:Objects():Clear(false)
if not _G.__ghost__ then _G.__ghost__ = Naivis.Ghost.NewDocument() end
__ghost__:Clear(false)

local function draw_nurbs_circle(N)
  N = N or 64
  local S = math.sqrt(0.5)

  local aDegree = 2
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
  local aNurbsCurve = naivecgl.Naive_NurbsCurve.new(aPoles, aWeights, aKnots, aMults, aDegree)

  local pAttr = Ghost_Attribute()
  pAttr:SetColor(Quantity_Color(LuaOCCT.Quantity.Quantity_NameOfColor.Quantity_NOC_CYAN))

  for _, p in ipairs(aPoles) do
    local pole = BRepPrimAPI_MakeSphere(gp_Pnt(p:X(), p:Y(), p:Z()), 0.3):Shape()
    __ghost__:AddShape(pole, pAttr, false)
  end

  for i = 0, N do
    local aPnt = aNurbsCurve:PointAt(i / N)
    if aPnt then
      local aVert = BRepBuilderAPI_MakeVertex(gp_Pnt(aPnt:X(), aPnt:Y(), aPnt:Z())):Vertex()
      doc:Objects():AddShape(aVert, LODoc_Attribute(), false)
    end
  end

  -- Check!
  -- local poles = {}
  -- for i, p in ipairs(aPoles) do
  --   poles[i] = gp_Pnt(p[1], p[2], p[3])
  -- end
  -- local aBS = Geom_BSplineCurve(poles, aWeights, aKnots, aMults, aDegree, false, false)
  -- local aShape = BRepBuilderAPI_MakeEdge(aBS):Edge()
  -- local anAttr = Ghost_Attribute()
  -- anAttr:SetColor(Quantity_Color(LuaOCCT.Quantity.Quantity_NameOfColor.Quantity_NOC_RED));
  -- __ghost__:AddShape(aShape, anAttr, false)
end

local function draw_nurbs_surface(N)
  N = N or 16

  local aUDegree = 2
  local aVDegree = 2
  local aPoles = {
    { P3(0, 0, 13),  P3(10, 0, 11),  P3(20, 0, 6) },
    { P3(0, 10, 19), P3(10, 10, 15), P3(20, 10, 11) },
    { P3(0, 20, 12), P3(10, 20, 4),  P3(20, 20, 15) },
  }
  local aWeights = {
    { 1, 1, 1 },
    { 1, 1, 1 },
    { 1, 1, 1 },
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
  pAttr:SetColor(Quantity_Color(LuaOCCT.Quantity.Quantity_NameOfColor.Quantity_NOC_RED))

  for _, c in ipairs(aPoles) do
    for _, p in ipairs(c) do
      local pole = BRepPrimAPI_MakeSphere(gp_Pnt(p:X(), p:Y(), p:Z()), 0.3):Shape()
      __ghost__:AddShape(pole, pAttr, false)
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
end

draw_nurbs_circle()
draw_nurbs_surface()

doc:UpdateView()
