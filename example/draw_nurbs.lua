local naivecgl = require("naivecgl")

local BRepBuilderAPI_MakeEdge = LuaOCCT.BRepBuilderAPI.BRepBuilderAPI_MakeEdge
local BRepBuilderAPI_MakeVertex = LuaOCCT.BRepBuilderAPI.BRepBuilderAPI_MakeVertex
local BRepPrimAPI_MakeSphere = LuaOCCT.BRepPrimAPI.BRepPrimAPI_MakeSphere
local Geom_BSplineCurve = LuaOCCT.Geom.Geom_BSplineCurve
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

  local t = 0.42
  local ok, aD = aNurbsCurve:DerivativeAt(t, 2)
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
  for i, p in ipairs(aPoles) do
    poles[i] = gp_Pnt(p:X(), p:Y(), p:Z())
  end
  local aBS = Geom_BSplineCurve(poles, aWeights, aKnots, aMults, aDegree, false, false)
  local aShape = BRepBuilderAPI_MakeEdge(aBS):Edge()
  local anAttr = Ghost_Attribute()
  anAttr:SetColor(Quantity_Color(LuaOCCT.Quantity.Quantity_NameOfColor.Quantity_NOC_RED));
  local aBS_2 = aBS:DN(t, 2)
  __ghost__:AddShape(aShape, anAttr, false)
  __ghost__:AddVector(aBS_2, aBS:Value(t), Ghost_AttrOfVector(), false)
end

local function draw_nurbs_surface(N)
  N = N or 16

  local aUDegree = 2
  local aVDegree = 2
  local aPoles = {
    { P3(15, -10, 3), P3(25, -10, 1), P3(35, -10, -4) },
    { P3(15, 0, 9),   P3(25, 0, 5),   P3(35, 0, 1) },
    { P3(15, 10, 2),  P3(25, 10, -6), P3(35, 10, 5) },
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
