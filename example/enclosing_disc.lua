local naivecgl = require("naivecgl")
local gp = LuaOCCT.gp.gp
local gp_Pnt = LuaOCCT.gp.gp_Pnt
local gp_Ax2 = LuaOCCT.gp.gp_Ax2
local BRepBuilderAPI_MakeVertex = LuaOCCT.BRepBuilderAPI.BRepBuilderAPI_MakeVertex
local Geom_Circle = LuaOCCT.Geom.Geom_Circle
local BRepBuilderAPI_MakeEdge = LuaOCCT.BRepBuilderAPI.BRepBuilderAPI_MakeEdge
local doc = Naivis.NaiveDoc.ActiveDoc

doc:Objects():Clear(false)

local nbPoints = 100
local aPoints = {}

for i = 1, nbPoints do
  local x = math.random() * 20 - 10
  local y = math.random() * 20 - 10
  aPoints[i] = { x, y }
  doc:Objects():AddShape(BRepBuilderAPI_MakeVertex(gp_Pnt(x, y, 0)):Vertex(), false)
end

local aDisc = naivecgl.bndshape.EnclosingDisc.new()
aDisc:ReBuild(aPoints)
local ok, ox, oy, r = aDisc:Circle()
aDisc:Dispose()

if ok then
  local circle = Geom_Circle(gp_Ax2(gp_Pnt(ox, oy, 0), gp.DZ()), r)
  local edge = BRepBuilderAPI_MakeEdge(circle):Edge()
  doc:Objects():AddShape(edge, false)
end

doc:UpdateView()
