local M = {}

M.inspect = require("inspect")
M.json = require("json")
M.occ = LuaOCCT
-- M.uv = require("luv")

function M.print(...)
  for i = 1, select('#', ...) do
    local o = select(i, ...)
    if type(o) == "string" then
      print(o)
    else
      print(M.inspect(o, { newline = "\n", indent = "  " }))
    end
  end

  return ...
end

---Reload module.
---@param module string Module name.
---@return any module
function M.require(module)
  if package.loaded[module] then
    package.loaded[module] = nil
  end
  return require(module)
end

return M
