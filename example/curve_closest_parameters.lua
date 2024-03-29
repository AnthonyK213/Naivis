local doc = Naivis.NaiveDoc.ActiveDoc
local gp = LuaOCCT.gp.gp
local gp_Pnt = LuaOCCT.gp.gp_Pnt
local Geom_Circle = LuaOCCT.Geom.Geom_Circle
local BRepBuilderAPI_MakeVertex = LuaOCCT.BRepBuilderAPI.BRepBuilderAPI_MakeVertex
local BRepBuilderAPI_MakeEdge = LuaOCCT.BRepBuilderAPI.BRepBuilderAPI_MakeEdge
local inspect = require("inspect")

doc:Objects():Clear(false)

local circle = Geom_Circle(gp.XOY(), 9)
local edge = BRepBuilderAPI_MakeEdge(circle):Edge()
doc:Objects():AddShape(edge, false)

local point = gp_Pnt(5, 1, 4)
local vertex = BRepBuilderAPI_MakeVertex(point):Vertex()
doc:Objects():AddShape(vertex, false)

local tList = LuaOCCT.LOUtil.LOUtil_Curve.ClosestParameters(circle, point)
print("closest_parameters:", inspect(tList))

for _, t in ipairs(tList) do
  local pnt = circle:Value(t)
  local vtx = BRepBuilderAPI_MakeVertex(pnt):Vertex()
  doc:Objects():AddShape(vtx, false)
end

doc:UpdateView()
