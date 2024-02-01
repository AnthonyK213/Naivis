#include "NaiveDoc_ObjectTable.hxx"
#include "NaiveDoc_Document.hxx"

#include <Mesh/Mesh_Util.hxx>
#include <Util/Util_AIS.hxx>
#include <Util/Util_OCAF.hxx>

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

const Handle(AIS_InteractiveContext) & NaiveDoc_ObjectTable::Context() const {
  return myDoc->Context();
}

NaiveDoc_Id NaiveDoc_ObjectTable::AddShape(const TopoDS_Shape &theShape,
                                           Standard_Boolean theToUpdate) {
  Handle(TDocStd_Document) aDoc = myDoc->Document();

  if (aDoc.IsNull())
    return NaiveDoc_Id();

  if (aDoc->HasOpenCommand())
    return NaiveDoc_Id();

  aDoc->OpenCommand();

  Handle(XCAFDoc_ShapeTool) anAsm =
      XCAFDoc_DocumentTool::ShapeTool(aDoc->Main());
  TDF_Label aLabel = anAsm->AddShape(theShape, Standard_True);
  anAsm->UpdateAssemblies();
  Handle(TPrsStd_AISPresentation) aPrs =
      TPrsStd_AISPresentation::Set(aLabel, TNaming_NamedShape::GetID());
  aPrs->SetMode(AIS_Shaded);
  aPrs->Display(Standard_True);

  aDoc->CommitCommand();

  myDoc->OnAddObject(myDoc);

  if (theToUpdate)
    myDoc->UpdateView();

  return aLabel;
}

NaiveDoc_Id NaiveDoc_ObjectTable::AddMesh(const Handle(Poly_Triangulation) &
                                              theMesh,
                                          Standard_Boolean theToUpdate) {
  return {};
}

void NaiveDoc_ObjectTable::Clear(Standard_Boolean theToUpdate) {}

Handle(NaiveDoc_Object)
    NaiveDoc_ObjectTable::FindId(const NaiveDoc_Id &theId) const {
  return nullptr;
}

Standard_Boolean
NaiveDoc_ObjectTable::DeleteObject(const Handle(NaiveDoc_Object) & theObject,
                                   Standard_Boolean theToUpdate) {
  return DeleteObjects({theObject}, theToUpdate) == 1;
}

Standard_Integer
NaiveDoc_ObjectTable::DeleteObjects(const NaiveDoc_ObjectList &theObjects,
                                    Standard_Boolean theToUpdate) {
  return 0;
}

Standard_Boolean
NaiveDoc_ObjectTable::ShowObject(const Handle(NaiveDoc_Object) & theObject,
                                 Standard_Boolean theToUpdate) {
  return ShowObjects({theObject}, theToUpdate) == 1;
}

Standard_Integer
NaiveDoc_ObjectTable::ShowObjects(const NaiveDoc_ObjectList &theObjects,
                                  Standard_Boolean theToUpdate) {
  return 0;
}

Standard_Integer NaiveDoc_ObjectTable::ShowAll(Standard_Boolean theToUpdate) {
  return 0;
}

Standard_Boolean
NaiveDoc_ObjectTable::HideObject(const Handle(NaiveDoc_Object) & theObject,
                                 Standard_Boolean theToUpdate) {
  NaiveDoc_ObjectList anObjectList{theObject};
  return HideObjects(anObjectList, theToUpdate) == 1;
}

Standard_Integer
NaiveDoc_ObjectTable::HideObjects(const NaiveDoc_ObjectList &theObjects,
                                  Standard_Boolean theToUpdate) {
  Standard_Integer nbHide = 0;

  if (myDoc->Document()->HasOpenCommand())
    return nbHide;

  myDoc->Document()->OpenCommand();

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

  myDoc->Document()->CommitCommand();

  if (theToUpdate)
    Context()->UpdateCurrentViewer();

  return nbHide;
}

Standard_Boolean
NaiveDoc_ObjectTable::PurgeObject(const Handle(NaiveDoc_Object) & theObject,
                                  Standard_Boolean theToUpdate) {
  NaiveDoc_ObjectList anObjectList{theObject};
  return PurgeObjects(anObjectList, theToUpdate) == 1;
}

Standard_Integer
NaiveDoc_ObjectTable::PurgeObjects(const NaiveDoc_ObjectList &theObjects,
                                   Standard_Boolean theToUpdate) {
  return 0;
}

Standard_Boolean
NaiveDoc_ObjectTable::SelectObject(const Handle(NaiveDoc_Object) & theObject,
                                   Standard_Boolean theSelect,
                                   Standard_Boolean theToUpdate) {
  return Standard_True;
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
  return 0;
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

Standard_Boolean
NaiveDoc_ObjectTable::addObjectRaw(const Handle(NaiveDoc_Object) & theObject,
                                   Standard_Boolean theToUpdate) {
  return Standard_True;
}

Standard_Boolean
NaiveDoc_ObjectTable::deleteObjectRaw(const Handle(NaiveDoc_Object) & theObject,
                                      Standard_Boolean theToUpdate) {
  return Standard_True;
}

Standard_Boolean
NaiveDoc_ObjectTable::showObjectRaw(const Handle(NaiveDoc_Object) & theObject,
                                    Standard_Boolean theToUpdate) {
  return Standard_True;
}

Standard_Boolean
NaiveDoc_ObjectTable::hideObjectRaw(const Handle(NaiveDoc_Object) & theObject,
                                    Standard_Boolean theToUpdate) {
  return Standard_True;
}

Standard_Boolean
NaiveDoc_ObjectTable::purgeObjectRaw(const Handle(NaiveDoc_Object) & theObject,
                                     Standard_Boolean theToUpdate) {
  return Standard_True;
}

void NaiveDoc_ObjectTable::purgeAllRaw(Standard_Boolean theToUpdate) {}
