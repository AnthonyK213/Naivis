local Naivis = Naivis
local nvs = nvs

local BRepBuilderAPI_MakeVertex = nvs.occ.BRepBuilderAPI.BRepBuilderAPI_MakeVertex
local BRepBuilderAPI_MakeEdge = nvs.occ.BRepBuilderAPI.BRepBuilderAPI_MakeEdge
local GeomAPI_Interpolate = nvs.occ.GeomAPI.GeomAPI_Interpolate
local Ghost_Attribute = Naivis.Ghost.Ghost_Attribute
local Quantity_Color = nvs.occ.Quantity.Quantity_Color
local gp_Pnt = nvs.occ.gp.gp_Pnt

local doc = Naivis.NaiveDoc.ActiveDoc
doc:Objects():Clear(false)
if not _G.__ghost__ then _G.__ghost__ = Naivis.Ghost.NewDocument() end
__ghost__:Clear(false)

local points = {
  gp_Pnt(3, 1, 4),
  gp_Pnt(1, 5, 9),
  gp_Pnt(2, 6, 5),
  gp_Pnt(3, 5, 8),
  gp_Pnt(9, 7, 9),
}

local pnt_attr = Ghost_Attribute()
pnt_attr:SetColor(Quantity_Color(nvs.occ.Quantity.Quantity_NameOfColor.Quantity_NOC_RED))

for _, p in ipairs(points) do
  local v = BRepBuilderAPI_MakeVertex(p):Shape()
  __ghost__:AddShape(v, pnt_attr, false)
end

local interpolate = GeomAPI_Interpolate(points, false, 1e-2)
interpolate:Perform();

if interpolate:IsDone() then
  local crv_attr = Ghost_Attribute()
  crv_attr:SetColor(Quantity_Color(nvs.occ.Quantity.Quantity_NameOfColor.Quantity_NOC_CYAN))
  local e = BRepBuilderAPI_MakeEdge(interpolate:Curve()):Edge()
  __ghost__:AddShape(e, crv_attr, false)
end

doc:UpdateView()
