local Naivis = Naivis
local nvs = nvs
local naivecgl = require("naivecgl")

local gp = nvs.occ.gp.gp
local gp_Pnt = nvs.occ.gp.gp_Pnt
local gp_Ax2 = nvs.occ.gp.gp_Ax2
local BRepBuilderAPI_MakeVertex = nvs.occ.BRepBuilderAPI.BRepBuilderAPI_MakeVertex
local Geom_Circle = nvs.occ.Geom.Geom_Circle
local BRepBuilderAPI_MakeEdge = nvs.occ.BRepBuilderAPI.BRepBuilderAPI_MakeEdge
local LODoc_Attribute = nvs.occ.LODoc.LODoc_Attribute
local Quantity_Color = nvs.occ.Quantity.Quantity_Color
local Ghost_Attribute = Naivis.Ghost.Ghost_Attribute
local P2 = naivecgl.Naive_XY

local doc = Naivis.NaiveDoc.ActiveDoc
doc:Objects():Clear(false)
if not _G.__ghost__ then _G.__ghost__ = Naivis.Ghost.NewDocument() end
__ghost__:Clear(false)

local nbPoints = 100
local aPoints = {}

for i = 1, nbPoints do
  local x = math.random() * 20 - 10
  local y = math.random() * 20 - 10
  aPoints[i] = P2(x, y)
  doc:Objects():AddShape(BRepBuilderAPI_MakeVertex(gp_Pnt(x, y, 0)):Vertex(), LODoc_Attribute(), false)
end

local aDisc = naivecgl.bndshape.EnclosingDisc.new()
aDisc:ReBuild(aPoints)
local o, r = aDisc:Circle()
aDisc:Dispose()

local circle = Geom_Circle(gp_Ax2(gp_Pnt(o:X(), o:Y(), 0), gp.DZ()), r)
local edge = BRepBuilderAPI_MakeEdge(circle):Edge()
local anAttr = Ghost_Attribute()
anAttr:SetColor(Quantity_Color(nvs.occ.Quantity.Quantity_NameOfColor.Quantity_NOC_RED))
__ghost__:AddShape(edge, anAttr, false)

doc:UpdateView()
