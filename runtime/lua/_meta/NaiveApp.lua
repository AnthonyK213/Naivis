---@meta _
error('Cannot require a meta file')

Naivis.NaiveApp = {}

---@class NaiveApp_Settings
Naivis.NaiveApp.NaiveApp_Settings = {}

---@type NaiveApp_Settings
Naivis.NaiveApp.Settings = {}

---@class NaiveApp_Extension
Naivis.NaiveApp.NaiveApp_Extension = {}

---@return boolean
function Naivis.NaiveApp.NaiveApp_Extension:IsValid() end

---@return boolean
function Naivis.NaiveApp.NaiveApp_Extension:IsLoaded() end

---@return string
function Naivis.NaiveApp.NaiveApp_Extension:Path() end

---@return string
function Naivis.NaiveApp.NaiveApp_Extension:Name() end

---@class NaiveApp_ExtensionMgr
Naivis.NaiveApp.NaiveApp_ExtensionMgr = {}

---@type NaiveApp_ExtensionMgr
Naivis.NaiveApp.ExtensionMgr = {}

---@param theName string
---@return NaiveApp_Extension
function Naivis.NaiveApp.NaiveApp_ExtensionMgr:Find(theName) end

---Clear the output buffer.
function Naivis.NaiveApp.Clear() end

---@return string
function Naivis.NaiveApp.CurrentScriptDir() end
