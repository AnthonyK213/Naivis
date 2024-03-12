local naivecgl = require("naivecgl")
local gp_Pnt = LuaOCCT.gp.gp_Pnt
local doc = Naivis.NaiveDoc.ActiveDoc

doc:Objects():Clear()

local tetrasphere = naivecgl.Naive_Tessellation_TetraSphere(gp_Pnt(), 10, 10)
doc:Objects():AddMesh(tetrasphere, true)
