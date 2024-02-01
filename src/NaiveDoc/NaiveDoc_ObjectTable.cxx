#include "NaiveDoc_ObjectTable.hxx"
#include "NaiveDoc_Document.hxx"

#include <Mesh/Mesh_Util.hxx>
#include <Util/Util_AIS.hxx>
#include <Util/Util_XCAF.hxx>

#include <TNaming_Builder.hxx>
#include <TNaming_NamedShape.hxx>
#include <TPrsStd_AISPresentation.hxx>
#include <TPrsStd_AISViewer.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFPrs_AISObject.hxx>

IMPLEMENT_STANDARD_RTTIEXT(NaiveDoc_ObjectTable, Standard_Transient)

NaiveDoc_ObjectTable::NaiveDoc_ObjectTable(NaiveDoc_Document *theDoc)
    : myDoc(theDoc) {}

NaiveDoc_ObjectTable::~NaiveDoc_ObjectTable() {}

NaiveDoc_Id NaiveDoc_ObjectTable::AddShape(const TopoDS_Shape &theShape,
                                           Standard_Boolean theToUpdate) {
  Handle(TDocStd_Document) aDoc = myDoc->Document();

  if (aDoc.IsNull())
    return NaiveDoc_Id();

  aDoc->OpenCommand();

  TDF_Label aMain = aDoc->Main();
  TDF_Label aNewLabel = TDF_TagSource::NewChild(aMain);
  Handle(XCAFDoc_ShapeTool) anAsm = XCAFDoc_DocumentTool::ShapeTool(aNewLabel);
  TDF_Label aLabel = anAsm->AddShape(theShape, Standard_True);
  TDataStd_Name::Set(aNewLabel, "Shape");
  anAsm->UpdateAssemblies();
  Handle(TPrsStd_AISPresentation) aPrs =
      TPrsStd_AISPresentation::Set(aLabel, TNaming_NamedShape::GetID());
  aPrs->Display(Standard_True);
  Handle(NaiveDoc_Object) anObj = aPrs->GetAIS();
  NaiveDoc_Object_SetId(*anObj);
  anObj->SetDisplayMode(AIS_Shaded);
  Context()->Display(anObj, theToUpdate);

  myDoc->OnAddObject(myDoc);

  if (!aDoc->CommitCommand()) {
    std::cout << "Failed to commit command\n";
  }

  return NaiveDoc_Object_GetId(*anObj);
}

const Handle(AIS_InteractiveContext) & NaiveDoc_ObjectTable::Context() const {
  return myDoc->Context();
}

NaiveDoc_Id NaiveDoc_ObjectTable::AddMesh(const Handle(Poly_Triangulation) &
                                              theMesh,
                                          Standard_Boolean theToUpdate) {
  Handle(NaiveDoc_Object) anObj = Mesh_Util::CreateMeshVS(theMesh);
  NaiveDoc_Object_SetId(*anObj);
  anObj->SetDisplayMode(MeshVS_DMF_Shading);

  return addObject(anObj, theToUpdate);
}

void NaiveDoc_ObjectTable::Clear(Standard_Boolean theToUpdate) {
  NaiveDoc_ObjectList theList = myObjects.values();
  DeleteObjects(theList, theToUpdate);
}

Handle(NaiveDoc_Object)
    NaiveDoc_ObjectTable::FindId(const NaiveDoc_Id &theId) const {
  if (myObjects.contains(theId)) {
    return myObjects.value(theId);
  }

  return nullptr;
}

Standard_Boolean
NaiveDoc_ObjectTable::DeleteObject(const Handle(NaiveDoc_Object) & theObject,
                                   Standard_Boolean theToUpdate) {
  return DeleteObjects({theObject}, theToUpdate) == 1;
}

Standard_Boolean
NaiveDoc_ObjectTable::DeleteObject(const NaiveDoc_Id &theId,
                                   Standard_Boolean theToUpdate) {
  return DeleteObject(FindId(theId), theToUpdate);
}

Standard_Integer
NaiveDoc_ObjectTable::DeleteObjects(const NaiveDoc_ObjectList &theObjects,
                                    Standard_Boolean theToUpdate) {
  return 0;
}

Standard_Integer
NaiveDoc_ObjectTable::DeleteObjects(NaiveDoc_ObjectList &&theObjects,
                                    Standard_Boolean theToUpdate) {
  return 0;
}

Standard_Boolean
NaiveDoc_ObjectTable::ShowObject(const Handle(NaiveDoc_Object) & theObject,
                                 Standard_Boolean theToUpdate) {
  return ShowObjects({theObject}, theToUpdate) == 1;
}

Standard_Boolean
NaiveDoc_ObjectTable::ShowObject(const NaiveDoc_Id &theId,
                                 Standard_Boolean theToUpdate) {
  return ShowObject(FindId(theId), theToUpdate);
}

Standard_Integer
NaiveDoc_ObjectTable::ShowObjects(const NaiveDoc_ObjectList &theObjects,
                                  Standard_Boolean theToUpdate) {
  return 0;
}

Standard_Integer
NaiveDoc_ObjectTable::ShowObjects(NaiveDoc_ObjectList &&theObjects,
                                  Standard_Boolean theToUpdate) {
  return 0;
}

Standard_Integer NaiveDoc_ObjectTable::ShowAll(Standard_Boolean theToUpdate) {
  return ShowObjects(myObjects.values(), theToUpdate);
}

Standard_Boolean
NaiveDoc_ObjectTable::HideObject(const Handle(NaiveDoc_Object) & theObject,
                                 Standard_Boolean theToUpdate) {
  NaiveDoc_ObjectList anObjectList{theObject};
  return HideObjects(anObjectList, theToUpdate) == 1;
}

Standard_Boolean
NaiveDoc_ObjectTable::HideObject(const NaiveDoc_Id &theId,
                                 Standard_Boolean theToUpdate) {
  Handle(NaiveDoc_Object) anObj = FindId(theId);
  return HideObject(anObj, theToUpdate);
}

Standard_Integer
NaiveDoc_ObjectTable::HideObjects(const NaiveDoc_ObjectList &theObjects,
                                  Standard_Boolean theToUpdate) {
  Standard_Integer nbHide = 0;
  myDoc->Document()->NewCommand();

  for (const auto &anObj : theObjects) {
    if (anObj.IsNull())
      continue;

    if (anObj->IsKind(STANDARD_TYPE(XCAFPrs_AISObject))) {
      auto anXcafObj = Handle(XCAFPrs_AISObject)::DownCast(anObj);
      const TDF_Label aLabel = anXcafObj->GetLabel();
      Handle(TPrsStd_AISPresentation) aPrs;

      if (!aLabel.FindAttribute(TPrsStd_AISPresentation::GetID(), aPrs)) {
        continue;
      }

      aPrs->Erase(Standard_True);
    } else {
      continue;
    }

    nbHide++;
  }

  Handle(TPrsStd_AISViewer) aViewer;
  TPrsStd_AISViewer::Find(myDoc->Document()->Main(), aViewer);
  aViewer->Update();

  if (!myDoc->Document()->CommitCommand()) {
    std::cout << "Failed to commit command\n";
  }

  if (theToUpdate)
    Context()->UpdateCurrentViewer();

  return nbHide;
}

Standard_Integer
NaiveDoc_ObjectTable::HideObjects(NaiveDoc_ObjectList &&theObjects,
                                  Standard_Boolean theToUpdate) {
  return HideObjects(theObjects, theToUpdate);
}

Standard_Boolean
NaiveDoc_ObjectTable::PurgeObject(const Handle(NaiveDoc_Object) & theObject,
                                  Standard_Boolean theToUpdate) {
  NaiveDoc_ObjectList anObjectList{theObject};
  return PurgeObjects(anObjectList, theToUpdate) == 1;
}

Standard_Boolean
NaiveDoc_ObjectTable::PurgeObject(const NaiveDoc_Id &theId,
                                  Standard_Boolean theToUpdate) {
  Handle(NaiveDoc_Object) anObj = FindId(theId);
  return PurgeObject(anObj, theToUpdate);
}

Standard_Integer
NaiveDoc_ObjectTable::PurgeObjects(const NaiveDoc_ObjectList &theObjects,
                                   Standard_Boolean theToUpdate) {
  return 0;
}

Standard_Integer
NaiveDoc_ObjectTable::PurgeObjects(NaiveDoc_ObjectList &&theObjects,
                                   Standard_Boolean theToUpdate) {
  return 0;
}

Standard_Boolean
NaiveDoc_ObjectTable::SelectObject(const Handle(NaiveDoc_Object) & theObject,
                                   Standard_Boolean theSelect,
                                   Standard_Boolean theToUpdate) {
  if (theObject.IsNull() || NaiveDoc_Object_IsDeleted(*theObject) ||
      NaiveDoc_Object_IsHidden(*theObject)) {
    return Standard_False;
  }

  if (!myObjects.contains(NaiveDoc_Object_GetId(*theObject)))
    return Standard_False;

  if (!(Context()->IsSelected(theObject) ^ theSelect))
    return Standard_False;

  Context()->AddOrRemoveSelected(theObject, theToUpdate);

  return Standard_True;
}

Standard_Boolean
NaiveDoc_ObjectTable::SelectObject(const NaiveDoc_Id &theId,
                                   Standard_Boolean theSelect,
                                   Standard_Boolean theToUpdate) {
  return SelectObject(FindId(theId), theSelect, theToUpdate);
}

Standard_Integer
NaiveDoc_ObjectTable::SelectObjects(const NaiveDoc_ObjectList &theObjects,
                                    Standard_Boolean theSelect,
                                    Standard_Boolean theToUpdate) {
  Standard_Integer aCount = 0;

  for (const auto &anObj : theObjects) {
    if (SelectObject(anObj, theSelect, Standard_False))
      aCount++;
  }

  if (theToUpdate)
    myDoc->UpdateView();

  return aCount;
}

Standard_Integer NaiveDoc_ObjectTable::SelectAll(Standard_Boolean theToUpdate) {
  Standard_Integer aCount = 0;

  for (const auto &anObj : myObjects) {
    if (SelectObject(anObj, Standard_True, Standard_False))
      aCount++;
  }

  if (theToUpdate)
    myDoc->UpdateView();

  return aCount;
}

Standard_Integer
NaiveDoc_ObjectTable::UnselectAll(Standard_Boolean theToUpdate) {
  Standard_Integer nbSelected = Context()->NbSelected();

  for (Context()->InitSelected(); Context()->MoreSelected();
       Context()->NextSelected()) {
    Context()->AddOrRemoveSelected(Context()->SelectedInteractive(),
                                   Standard_False);
  }

  if (theToUpdate)
    myDoc->UpdateView();

  return nbSelected;
}

NaiveDoc_ObjectList NaiveDoc_ObjectTable::SelectedObjects() const {
  return Util_AIS::GetSelections(Context());
}

NaiveDoc_Id NaiveDoc_ObjectTable::addObject(const Handle(NaiveDoc_Object) &
                                                theObject,
                                            Standard_Boolean theToUpdate) {
  NaiveDoc_IdList anIds = addObjects({theObject}, theToUpdate);

  if (anIds.isEmpty())
    return NaiveDoc_Id();

  return anIds.first();
}

NaiveDoc_IdList
NaiveDoc_ObjectTable::addObjects(const NaiveDoc_ObjectList &theObjects,
                                 Standard_Boolean theToUpdate) {
  return {};
}

NaiveDoc_IdList
NaiveDoc_ObjectTable::addObjects(NaiveDoc_ObjectList &&theObjects,
                                 Standard_Boolean theToUpdate) {
  return {};
}

Standard_Boolean
NaiveDoc_ObjectTable::addObjectRaw(const Handle(NaiveDoc_Object) & theObject,
                                   Standard_Boolean theToUpdate) {
  if (theObject.IsNull())
    return Standard_False;

  NaiveDoc_Id anId = NaiveDoc_Object_GetId(*theObject);

  if (myObjects.contains(anId)) {
    if (!NaiveDoc_Object_IsDeleted(*theObject)) {
      return Standard_False;
    }

    NaiveDoc_Object_SetDeleted(*theObject, Standard_False);
  } else {
    myObjects.insert(anId, theObject);
  }

  Context()->Display(theObject, Standard_False);

  if (NaiveDoc_Object_IsHidden(*theObject))
    Context()->Erase(theObject, Standard_False);

  if (theToUpdate)
    myDoc->UpdateView();

  return Standard_True;
}

Standard_Boolean
NaiveDoc_ObjectTable::deleteObjectRaw(const Handle(NaiveDoc_Object) & theObject,
                                      Standard_Boolean theToUpdate) {
  if (theObject.IsNull())
    return Standard_False;

  if (NaiveDoc_Object_IsDeleted(*theObject))
    return Standard_False;

  NaiveDoc_Id anId = NaiveDoc_Object_GetId(*theObject);

  if (!myObjects.contains(anId))
    return Standard_False;

  NaiveDoc_Object_SetDeleted(*theObject, Standard_True);
  Context()->Remove(theObject, theToUpdate);

  return Standard_True;
}

Standard_Boolean
NaiveDoc_ObjectTable::showObjectRaw(const Handle(NaiveDoc_Object) & theObject,
                                    Standard_Boolean theToUpdate) {
  if (theObject.IsNull() || NaiveDoc_Object_IsDeleted(*theObject) ||
      !NaiveDoc_Object_IsHidden(*theObject))
    return Standard_False;

  NaiveDoc_Id anId = NaiveDoc_Object_GetId(*theObject);

  if (!myObjects.contains(anId)) {
    return Standard_False;
  }

  NaiveDoc_Object_SetHidden(*theObject, Standard_False);
  Context()->Display(theObject, theToUpdate);

  return Standard_True;
}

Standard_Boolean
NaiveDoc_ObjectTable::hideObjectRaw(const Handle(NaiveDoc_Object) & theObject,
                                    Standard_Boolean theToUpdate) {
  if (theObject.IsNull() || NaiveDoc_Object_IsDeleted(*theObject) ||
      NaiveDoc_Object_IsHidden(*theObject))
    return Standard_False;

  NaiveDoc_Id anId = NaiveDoc_Object_GetId(*theObject);

  if (!myObjects.contains(anId))
    return Standard_False;

  NaiveDoc_Object_SetHidden(*theObject, Standard_True);
  Context()->Erase(theObject, theToUpdate);

  return Standard_True;
}

Standard_Boolean
NaiveDoc_ObjectTable::purgeObjectRaw(const Handle(NaiveDoc_Object) & theObject,
                                     Standard_Boolean theToUpdate) {
  if (theObject.IsNull())
    return Standard_False;

  NaiveDoc_Id anId = NaiveDoc_Object_GetId(*theObject);

  if (!myObjects.contains(anId))
    return Standard_False;

  myObjects.remove(NaiveDoc_Object_GetId(*theObject));
  Context()->Remove(theObject, theToUpdate);

  return Standard_True;
}

void NaiveDoc_ObjectTable::purgeAllRaw(Standard_Boolean theToUpdate) {
  for (auto &anObj : myObjects) {
    Context()->Remove(anObj, Standard_False);
  }

  myObjects.clear();

  if (theToUpdate)
    myDoc->UpdateView();
}
