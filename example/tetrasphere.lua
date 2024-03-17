local naivecgl = require("naivecgl")
local gp_Pnt = LuaOCCT.gp.gp_Pnt
local doc = Naivis.NaiveDoc.ActiveDoc

doc:Objects():Clear(false)

local tetrasphere = naivecgl.Naive_Tessellation_TetraSphere(gp_Pnt(), 10, 10)
if tetrasphere then
  doc:Objects():AddMesh(tetrasphere, true)
end
