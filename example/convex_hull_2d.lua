local Naivis = Naivis
local nvs = nvs
local naivecgl = require("naivecgl")

local gp_Pnt = nvs.occ.gp.gp_Pnt
local BRepBuilderAPI_MakeVertex = nvs.occ.BRepBuilderAPI.BRepBuilderAPI_MakeVertex
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

local aCH2D = naivecgl.bndshape.ConvexHull2D.new(aPoints)
local aCode = aCH2D:Perform()
local convexIndices = aCH2D:ConvexIndices()
aCH2D:Dispose()

if aCode == naivecgl.NS.Naive_Code_ok then
  local count = #convexIndices

  for i = 1, count do
    local thisIndex = convexIndices[i]
    local nextIndex = convexIndices[i % count + 1]

    local p1 = gp_Pnt(aPoints[thisIndex]:X(), aPoints[thisIndex]:Y(), 0)
    local p2 = gp_Pnt(aPoints[nextIndex]:X(), aPoints[nextIndex]:Y(), 0)

    local anAttr = Ghost_Attribute()
    anAttr:SetColor(Quantity_Color(nvs.occ.Quantity.Quantity_NameOfColor.Quantity_NOC_RED))
    __ghost__:AddShape(BRepBuilderAPI_MakeEdge(p1, p2):Edge(), anAttr, false)
  end
else
  print("Failed", aCode)
end

doc:UpdateView()
