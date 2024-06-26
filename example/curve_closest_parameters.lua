local Naivis = Naivis
local LuaOCCT = LuaOCCT
local inspect = require("inspect")

local gp = LuaOCCT.gp.gp
local gp_Pnt = LuaOCCT.gp.gp_Pnt
local Geom_Circle = LuaOCCT.Geom.Geom_Circle
local BRepBuilderAPI_MakeVertex = LuaOCCT.BRepBuilderAPI.BRepBuilderAPI_MakeVertex
local BRepBuilderAPI_MakeEdge = LuaOCCT.BRepBuilderAPI.BRepBuilderAPI_MakeEdge
local LODoc_Attribute = LuaOCCT.LODoc.LODoc_Attribute

local doc = Naivis.NaiveDoc.ActiveDoc
doc:Objects():Clear(false)
if not _G.__ghost__ then _G.__ghost__ = Naivis.Ghost.NewDocument() end
__ghost__:Clear(false)

local circle = Geom_Circle(gp.XOY(), 9)
local edge = BRepBuilderAPI_MakeEdge(circle):Edge()
doc:Objects():AddShape(edge, LODoc_Attribute(), false)

local point = gp_Pnt(5, 1, 4)
local vertex = BRepBuilderAPI_MakeVertex(point):Vertex()
doc:Objects():AddShape(vertex, LODoc_Attribute(), false)

local tList = LuaOCCT.LOUtil.LOUtil_Curve.ClosestParameters(circle, point)
print("closest_parameters:", inspect(tList))

for _, t in ipairs(tList) do
  local pnt = circle:Value(t)
  local vtx = BRepBuilderAPI_MakeVertex(pnt):Vertex()
  doc:Objects():AddShape(vtx, LODoc_Attribute(), false)
end

doc:UpdateView()
