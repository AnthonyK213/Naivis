local Util = {}

---Reload module.
---@param module string Module name.
---@return any module
function Util.require(module)
  if package.loaded[module] then
    package.loaded[module] = nil
  end
  return require(module)
end

return Util
