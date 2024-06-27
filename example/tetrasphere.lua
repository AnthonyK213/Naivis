local Naivis = Naivis
local nvs = nvs
local naivecgl = require("naivecgl")

local gp_Pnt = nvs.occ.gp.gp_Pnt
local LODoc_Attribute = nvs.occ.LODoc.LODoc_Attribute
local Poly_Triangle = nvs.occ.Poly.Poly_Triangle
local Poly_Triangulation = nvs.occ.Poly.Poly_Triangulation

local doc = Naivis.NaiveDoc.ActiveDoc
doc:Objects():Clear(false)
if not _G.__ghost__ then _G.__ghost__ = Naivis.Ghost.NewDocument() end
__ghost__:Clear(false)

local tetrasphere = naivecgl.tessellation.Naive_Tessellation_Sphere_TetraSphere(naivecgl.Naive_XYZ(), 10, 10)
local aVertices = tetrasphere:Vertices()
local aTriangles = tetrasphere:Triangles()

local vList = {}
for i = 1, aVertices:Size() do
  vList[i] = gp_Pnt(aVertices:Value(i):X(), aVertices:Value(i):Y(), aVertices:Value(i):Z())
end

local tList = {}
for i, t in ipairs(aTriangles) do
  tList[i] = Poly_Triangle(t[1], t[2], t[3])
end

doc:Objects():AddMesh(Poly_Triangulation(vList, tList), LODoc_Attribute(), false)

doc:UpdateView()
