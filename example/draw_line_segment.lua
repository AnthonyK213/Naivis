local naivecgl = require("naivecgl")
local inspect = require("inspect")
local gp_Pnt = LuaOCCT.gp.gp_Pnt
local BRepBuilderAPI_MakeVertex = LuaOCCT.BRepBuilderAPI.BRepBuilderAPI_MakeVertex
local BRepBuilderAPI_MakeEdge = LuaOCCT.BRepBuilderAPI.BRepBuilderAPI_MakeEdge
local doc = Naivis.ActiveDoc

doc.Objects_:Clear()

local p1 = gp_Pnt()
local p2 = gp_Pnt(3, 1, 4)

local edge = BRepBuilderAPI_MakeEdge(p1, p2):Edge()
local id = doc.Objects_:AddShape(edge, true)

print(id)
