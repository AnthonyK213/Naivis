local naivecgl = require("naivecgl")
local gp_Pnt = LuaOCCT.gp.gp_Pnt
local BRepBuilderAPI_MakeVertex = LuaOCCT.BRepBuilderAPI.BRepBuilderAPI_MakeVertex
local BRepBuilderAPI_MakeEdge = LuaOCCT.BRepBuilderAPI.BRepBuilderAPI_MakeEdge
local LODoc_Attribute = LuaOCCT.LODoc.LODoc_Attribute
local doc = Naivis.NaiveDoc.ActiveDoc

doc:Objects():Clear(false)

local nbPoints = 100
local aPoints = {}

for i = 1, nbPoints do
  local x = math.random() * 20 - 10
  local y = math.random() * 20 - 10

  aPoints[i] = { x, y }

  doc:Objects():AddShape(BRepBuilderAPI_MakeVertex(gp_Pnt(x, y, 0)):Vertex(), LODoc_Attribute(), false)
end

local aCH2D = naivecgl.bndshape.ConvexHull2D.new(aPoints)
aCH2D:Perform()
local code = aCH2D:Status()
local convexIndices = aCH2D:ConvexIndices()
aCH2D:Dispose()

if code == naivecgl.NS.Naive_Ok then
  local count = #convexIndices

  for i = 1, count do
    local thisIndex = convexIndices[i]
    local nextIndex = convexIndices[i % count + 1]

    local p1 = gp_Pnt(aPoints[thisIndex][1], aPoints[thisIndex][2], 0)
    local p2 = gp_Pnt(aPoints[nextIndex][1], aPoints[nextIndex][2], 0)

    doc:Objects():AddShape(BRepBuilderAPI_MakeEdge(p1, p2):Edge(), LODoc_Attribute(), false)
  end
else
  print("Failed", code)
end

doc:UpdateView()
