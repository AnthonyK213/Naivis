local Naivis = Naivis
local nvs = nvs

local gp_Pnt = nvs.occ.gp.gp_Pnt
local Bnd_OBB = nvs.occ.Bnd.Bnd_OBB
local BRepBuilderAPI_MakeVertex = nvs.occ.BRepBuilderAPI.BRepBuilderAPI_MakeVertex
local BRepBuilderAPI_MakeEdge = nvs.occ.BRepBuilderAPI.BRepBuilderAPI_MakeEdge
local LODoc_Attribute = nvs.occ.LODoc.LODoc_Attribute
local Quantity_Color = nvs.occ.Quantity.Quantity_Color
local Ghost_Attribute = Naivis.Ghost.Ghost_Attribute

local doc = Naivis.NaiveDoc.ActiveDoc
doc:Objects():Clear(false)
if not _G.__ghost__ then _G.__ghost__ = Naivis.Ghost.NewDocument() end
__ghost__:Clear(false)

local nbPnts = 100
local aPnts = {}

for i = 1, nbPnts do
  local x = math.random() * 20 - 10
  local y = math.random() * 20 - 10
  local z = math.random() * 20 - 10

  aPnts[i] = gp_Pnt(x, y, z)
  doc:Objects():AddShape(BRepBuilderAPI_MakeVertex(aPnts[i]):Vertex(), LODoc_Attribute(), false)
end

local anOBB = Bnd_OBB()
anOBB:ReBuild(aPnts, true)
local aCorners = anOBB:GetVertex()

local function addEdge(m, n)
  local anAttr = Ghost_Attribute()
  anAttr:SetColor(Quantity_Color(nvs.occ.Quantity.Quantity_NameOfColor.Quantity_NOC_RED))
  __ghost__:AddShape(BRepBuilderAPI_MakeEdge(aCorners[m], aCorners[n]):Edge(), anAttr, false)
end

addEdge(1, 2)
addEdge(2, 4)
addEdge(4, 3)
addEdge(3, 1)
addEdge(5, 6)
addEdge(6, 8)
addEdge(8, 7)
addEdge(7, 5)
addEdge(1, 5)
addEdge(2, 6)
addEdge(3, 7)
addEdge(4, 8)

doc:UpdateView()
