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

Naivis.NaiveDoc = {}

---@class NaiveDoc_Document : LODoc_Document
Naivis.NaiveDoc.NaiveDoc_Document = {}

---@type NaiveDoc_Document
Naivis.NaiveDoc.ActiveDoc = {}

Naivis.Ghost = {}

---@enum Ghost_ObjectType
Naivis.Ghost.Ghost_ObjectType = {
  Ghost_OTNone = 0,
  Ghost_OTShape = 1,
  Ghost_OTMesh = 2,
  Ghost_OTVector = 3,
  Ghost_OTAx3 = 4,
}

---@class Ghost_Id
Naivis.Ghost.Ghost_Id = {}

---
---@return boolean
function Naivis.Ghost.Ghost_Id:IsNull() end

---
---@return string
function Naivis.Ghost.Ghost_Id:ToString() end

---@class Ghost_Object : Standard_Transient
Naivis.Ghost.Ghost_Object = {}

---@class Ghost_Attribute : LODoc_Attribute
---@overload fun():Ghost_Attribute
Naivis.Ghost.Ghost_Attribute = {}

---
---@return Ghost_ObjectType
function Naivis.Ghost.Ghost_Attribute:Type() end

---
---@return Ghost_Id
function Naivis.Ghost.Ghost_Attribute:Id() end

---
---@param theObj Ghost_Object
---@return Ghost_Attribute
function Naivis.Ghost.Ghost_Attribute.Get(theObj) end

---
---@param theObj Ghost_Object
---@param theAttr Ghost_Attribute
---@return Ghost_Attribute
function Naivis.Ghost.Ghost_Attribute.Set(theObj, theAttr) end

---
---@param theH Standard_Transient
---@return Ghost_Attribute
function Naivis.Ghost.Ghost_Attribute.DonwCast(theH) end

---@class Ghost_AttrOfVector : Ghost_Attribute
---@overload fun():Ghost_AttrOfVector
Naivis.Ghost.Ghost_AttrOfVector = {}

---
---@return number
function Naivis.Ghost.Ghost_AttrOfVector:BodyR() end

---
---@param theBodyR number
function Naivis.Ghost.Ghost_AttrOfVector:SetBodyR(theBodyR) end

---
---@return number
function Naivis.Ghost.Ghost_AttrOfVector:TipR() end

---
---@param theTipR number
function Naivis.Ghost.Ghost_AttrOfVector:SetTipR(theTipR) end

---
---@return number
function Naivis.Ghost.Ghost_AttrOfVector:TipL() end

---
---@param theTipL number
function Naivis.Ghost.Ghost_AttrOfVector:SetTipL(theTipL) end

---
---@param theObj Ghost_Object
---@return Ghost_AttrOfVector
function Naivis.Ghost.Ghost_AttrOfVector.Get(theObj) end

---
---@param theObj Ghost_Object
---@param theAttr Ghost_AttrOfVector
---@return Ghost_AttrOfVector
function Naivis.Ghost.Ghost_AttrOfVector.Set(theObj, theAttr) end

---
---@param theH Standard_Transient
---@return Ghost_AttrOfVector
function Naivis.Ghost.Ghost_AttrOfVector.DonwCast(theH) end

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
---@param theAttr Ghost_AttrOfVector
---@param theToUpdate boolean
function Naivis.Ghost.Ghost_Document:AddVector(theV, theAnchor, theAttr, theToUpdate) end

---
---@param theAx3 gp_Ax3
---@param theAttr Ghost_AttrOfVector
---@param theToUpdate boolean
function Naivis.Ghost.Ghost_Document:AddAx3(theAx3, theAttr, theToUpdate) end

---
---@param theId Ghost_Id
---@param theToUpdate boolean
---@return boolean
function Naivis.Ghost.Ghost_Document:DeleteObject(theId, theToUpdate) end

---
---@param theIds Ghost_Id[]
---@param theToUpdate boolean
---@return integer
function Naivis.Ghost.Ghost_Document:DeleteObject(theIds, theToUpdate) end

---
---@param theId Ghost_Id
---@return Ghost_Object?
function Naivis.Ghost.Ghost_Document:Find(theId) end

---
---@param theToUpdate boolean
function Naivis.Ghost.Ghost_Document:Clear(theToUpdate) end

---
---@return Ghost_Document
function Naivis.Ghost.NewDocument() end
