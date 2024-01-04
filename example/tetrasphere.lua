local naivecgl = require("naivecgl")
local gp_Pnt = LuaOCCT.gp.gp_Pnt
local doc = Naivis.ActiveDoc

local tetrasphere = naivecgl.Naive_Tessellation_TetraSphere(gp_Pnt(), 10, 10)

doc.Objects_:AddMesh(tetrasphere, true)
