local naivecgl = require("naivecgl")

local BRepBuilderAPI_MakeEdge = LuaOCCT.BRepBuilderAPI.BRepBuilderAPI_MakeEdge
local BRepBuilderAPI_MakeVertex = LuaOCCT.BRepBuilderAPI.BRepBuilderAPI_MakeVertex
local Geom_Circle = LuaOCCT.Geom.Geom_Circle
local Geom_BSplineCurve = LuaOCCT.Geom.Geom_BSplineCurve
local gp = LuaOCCT.gp.gp
local gp_Ax2 = LuaOCCT.gp.gp_Ax2
local gp_Pnt = LuaOCCT.gp.gp_Pnt

local N = 64
local S = math.sqrt(0.5)

local doc = Naivis.NaiveDoc.ActiveDoc
doc:Objects():Clear(false)

local aDegree = 2
local aPoles = { { 1, 0, 0 }, { 1, 1, 0 }, { 0, 1, 0 }, { -1, 1, 0 }, { -1, 0, 0 }, { -1, -1, 0 }, { 0, -1, 0 }, { 1, -1, 0 }, { 1, 0, 0 } }
local aWeights = { 1, S, 1, S, 1, S, 1, S, 1 }
local aKnots = { 0, 0.25, 0.5, 0.75, 1 }
local aMults = { 3, 2, 2, 2, 3 }
local aNurbsCurve = naivecgl.Naive_NurbsCurve.new(aPoles, aWeights, aKnots, aMults, aDegree)

-- for i = 1, #aPoles - 1 do
--   local anEdge = BRepBuilderAPI_MakeEdge(gp_Pnt(aPoles[i][1], aPoles[i][2], aPoles[i][3]),
--     gp_Pnt(aPoles[i + 1][1], aPoles[i + 1][2], aPoles[i + 1][3])):Edge()
--   doc:Objects():AddShape(anEdge, false)
-- end

for i = 1, #aPoles do
  local o = gp_Ax2(gp_Pnt(aPoles[i][1], aPoles[i][2], aPoles[i][3]), gp.DZ())
  local edge = BRepBuilderAPI_MakeEdge(Geom_Circle(o, 0.03)):Edge()
  doc:Objects():AddShape(edge, false)
end

for i = 0, N do
  local aPnt = aNurbsCurve:PointAt(i * 1 / N)
  if aPnt then
    local aVert = BRepBuilderAPI_MakeVertex(aPnt):Vertex()
    doc:Objects():AddShape(aVert, false)
  end
end

-- Check!
local poles = {}
for i, p in ipairs(aPoles) do
  poles[i] = gp_Pnt(p[1], p[2], p[3])
end
local aBS = Geom_BSplineCurve(poles, aWeights, aKnots, aMults, aDegree, false, false)
local aShape = BRepBuilderAPI_MakeEdge(aBS):Edge()
doc:Objects():AddShape(aShape, false)

doc:UpdateView()
