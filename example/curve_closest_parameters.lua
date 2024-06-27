local Naivis = Naivis
local nvs = nvs

local gp = nvs.occ.gp.gp
local gp_Pnt = nvs.occ.gp.gp_Pnt
local Geom_Circle = nvs.occ.Geom.Geom_Circle
local BRepBuilderAPI_MakeVertex = nvs.occ.BRepBuilderAPI.BRepBuilderAPI_MakeVertex
local BRepBuilderAPI_MakeEdge = nvs.occ.BRepBuilderAPI.BRepBuilderAPI_MakeEdge
local LODoc_Attribute = nvs.occ.LODoc.LODoc_Attribute

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

local tList = nvs.occ.LOUtil.LOUtil_Curve.ClosestParameters(circle, point)
nvs.print("closest_parameters:", tList)

for _, t in ipairs(tList) do
  local pnt = circle:Value(t)
  local vtx = BRepBuilderAPI_MakeVertex(pnt):Vertex()
  doc:Objects():AddShape(vtx, LODoc_Attribute(), false)
end

doc:UpdateView()
