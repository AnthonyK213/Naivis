local nvs = nvs

local BRepAlgoAPI_Cut = nvs.occ.BRepAlgoAPI.BRepAlgoAPI_Cut
local BRepAlgoAPI_Fuse = nvs.occ.BRepAlgoAPI.BRepAlgoAPI_Fuse
local BRepAlgoAPI_Common = nvs.occ.BRepAlgoAPI.BRepAlgoAPI_Common
local BRepPrimAPI_MakeSphere = nvs.occ.BRepPrimAPI.BRepPrimAPI_MakeSphere
local BRepPrimAPI_MakeCylinder = nvs.occ.BRepPrimAPI.BRepPrimAPI_MakeCylinder
local gp = nvs.occ.gp.gp
local gp_Ax2 = nvs.occ.gp.gp_Ax2
local gp_Pnt = nvs.occ.gp.gp_Pnt
local Message_ProgressRange = nvs.occ.Message.Message_ProgressRange
local LODoc_Attribute = nvs.occ.LODoc.LODoc_Attribute

local doc = Naivis.NaiveDoc.ActiveDoc
doc:Objects():Clear(false)
if not _G.__ghost__ then _G.__ghost__ = Naivis.Ghost.NewDocument() end
__ghost__:Clear(false)

local aCyl = BRepPrimAPI_MakeCylinder(gp_Ax2(), 5, 45):Shape()
local aSph = BRepPrimAPI_MakeSphere(gp_Pnt(7, 0, 0), 12):Shape()

local aBoolRes = BRepAlgoAPI_Common(aCyl, aSph, Message_ProgressRange())
if aBoolRes:IsDone() then
  doc:Objects():AddShape(aBoolRes:Shape(), LODoc_Attribute(), false)
end

doc:UpdateView()
