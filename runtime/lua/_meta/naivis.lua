---@meta _
error('Cannot require a meta file')

Naivis.NaiveApp = {}

---@class NaiveApp_Settings
Naivis.NaiveApp.NaiveApp_Settings = {}

---@type NaiveApp_Settings
Naivis.NaiveApp.Settings = {}

---Clear the output buffer.
function Naivis.NaiveApp.Clear() end

Naivis.NaiveDoc = {}

---@class NaiveDoc_Document : LODoc_Document
Naivis.NaiveDoc.NaiveDoc_Document = {}

---@type NaiveDoc_Document
Naivis.NaiveDoc.ActiveDoc = {}

Naivis.Ghost = {}

---@class Ghost_Attribute : LODoc_Attribute
---@overload fun():Ghost_Attribute
Naivis.Ghost.Ghost_Attribute = {}

---@class Ghost_Document
Naivis.Ghost.Ghost_Document = {}

---
---@param theShape TopoDS_Shape
---@param theAttr Ghost_Attribute
---@param theToUpdate boolean
function Naivis.Ghost.Ghost_Document:AddShape(theShape, theAttr, theToUpdate) end

---
---@param theMesh Poly_Triangulation
---@param theAttr Ghost_Attribute
---@param theToUpdate boolean
function Naivis.Ghost.Ghost_Document:AddMesh(theMesh, theAttr, theToUpdate) end

---
---@param theV gp_Vec
---@param theAnchor gp_Pnt
---@param theAttr Ghost_Attribute
---@param theToUpdate boolean
function Naivis.Ghost.Ghost_Document:AddVector(theV, theAnchor, theAttr, theToUpdate) end

---
---@param theAx3 gp_Ax3
---@param theAttr Ghost_Attribute
---@param theToUpdate boolean
function Naivis.Ghost.Ghost_Document:AddAx3(theAx3, theAttr, theToUpdate) end

---
---@param theId QUuid
---@param theToUpdate boolean
---@return boolean
function Naivis.Ghost.Ghost_Document:DeleteObject(theId, theToUpdate) end

---
---@param theIds QUuid[]
---@param theToUpdate boolean
---@return integer
function Naivis.Ghost.Ghost_Document:DeleteObject(theIds, theToUpdate) end

---
---@param theToUpdate boolean
function Naivis.Ghost.Ghost_Document:Clear(theToUpdate) end

---
---@return Ghost_Document
function Naivis.Ghost.NewDocument() end
