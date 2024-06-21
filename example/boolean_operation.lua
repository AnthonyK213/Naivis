local LuaOCCT = LuaOCCT

local BRepAlgoAPI_Cut = LuaOCCT.BRepAlgoAPI.BRepAlgoAPI_Cut
local BRepAlgoAPI_Fuse = LuaOCCT.BRepAlgoAPI.BRepAlgoAPI_Fuse
local BRepAlgoAPI_Common = LuaOCCT.BRepAlgoAPI.BRepAlgoAPI_Common
local BRepPrimAPI_MakeSphere = LuaOCCT.BRepPrimAPI.BRepPrimAPI_MakeSphere
local BRepPrimAPI_MakeCylinder = LuaOCCT.BRepPrimAPI.BRepPrimAPI_MakeCylinder
local gp = LuaOCCT.gp.gp
local gp_Ax2 = LuaOCCT.gp.gp_Ax2
local gp_Pnt = LuaOCCT.gp.gp_Pnt
local Message_ProgressRange = LuaOCCT.Message.Message_ProgressRange
local LODoc_Attribute = LuaOCCT.LODoc.LODoc_Attribute

local doc = Naivis.NaiveDoc.ActiveDoc
doc:Objects():Clear(false)
if not _G.__ghost__ then _G.__ghost__ = Naivis.Ghost.NewDocument() end
__ghost__:Clear(false)

local aCyl = BRepPrimAPI_MakeCylinder(gp_Ax2(), 5, 45):Shape()
local aSph = BRepPrimAPI_MakeSphere(gp_Pnt(7, 0, 0), 12):Shape()

local aFuse = BRepAlgoAPI_Common(aCyl, aSph, Message_ProgressRange())
if aFuse:IsDone() then
  doc:Objects():AddShape(aFuse:Shape(), LODoc_Attribute(), false)
end

doc:UpdateView()
