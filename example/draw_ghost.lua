local Naivis = Naivis
local LuaOCCT = LuaOCCT

local Ghost_AttrOfVector = Naivis.Ghost.Ghost_AttrOfVector
local gp = LuaOCCT.gp.gp
local gp_Ax3 = LuaOCCT.gp.gp_Ax3
local gp_Pnt = LuaOCCT.gp.gp_Pnt
local gp_Vec = LuaOCCT.gp.gp_Vec

local doc = Naivis.NaiveDoc.ActiveDoc
doc:Objects():Clear(false)
if not _G.__ghost__ then _G.__ghost__ = Naivis.Ghost.NewDocument() end
__ghost__:Clear(false)

local anAttr = Ghost_AttrOfVector()
anAttr:SetBodyR(1)
anAttr:SetTipR(2)
local idV = __ghost__:AddVector(gp_Vec(10, 10, 10), gp_Pnt(3, -1, 4), anAttr, true)
local idAx3 = __ghost__:AddAx3(gp_Ax3(gp.XOY()), anAttr, true)
