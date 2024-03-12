local gp_Pnt = LuaOCCT.gp.gp_Pnt
local Bnd_OBB = LuaOCCT.Bnd.Bnd_OBB
local BRepBuilderAPI_MakeVertex = LuaOCCT.BRepBuilderAPI.BRepBuilderAPI_MakeVertex
local BRepBuilderAPI_MakeEdge = LuaOCCT.BRepBuilderAPI.BRepBuilderAPI_MakeEdge
local doc = Naivis.NaiveDoc.ActiveDoc

doc:Objects():Clear()

local nbPnts = 100
local aPnts = {}

for i = 1, nbPnts do
  local x = math.random() * 20 - 10
  local y = math.random() * 20 - 10
  local z = math.random() * 20 - 10

  aPnts[i] = gp_Pnt(x, y, z)
  doc:Objects():AddShape(BRepBuilderAPI_MakeVertex(aPnts[i]):Vertex(), false)
end

local anOBB = Bnd_OBB()
anOBB:ReBuild(aPnts, true)
local aCorners = anOBB:GetVertex()

local function addEdge(m, n)
  doc:Objects():AddShape(BRepBuilderAPI_MakeEdge(aCorners[m], aCorners[n]):Edge(), false)
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
