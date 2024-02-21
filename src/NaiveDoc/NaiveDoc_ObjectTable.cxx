#include <TNaming_Builder.hxx>
#include <TNaming_NamedShape.hxx>
#include <TPrsStd_AISPresentation.hxx>
#include <TPrsStd_AISViewer.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFPrs_AISObject.hxx>

#include "NaiveDoc_Attribute.hxx"
#include "NaiveDoc_Document.hxx"
#include "NaiveDoc_ObjectTable.hxx"
#include <Util/Util_AIS.hxx>
#include <Util/Util_Mesh.hxx>
#include <Util/Util_OCAF.hxx>

#define Naive_OPEN_COMMAND(R)                                                  \
  if (myDoc->Document()->HasOpenCommand())                                     \
    return R;                                                                  \
  myDoc->Document()->OpenCommand();

#define Naive_COMMIT_COMMAND()                                                 \
  myDoc->Document()->CommitCommand();                                          \
  if (theToUpdate)                                                             \
    myDoc->UpdateView();

IMPLEMENT_STANDARD_RTTIEXT(NaiveDoc_ObjectTable, Standard_Transient)

NaiveDoc_ObjectTable::NaiveDoc_ObjectTable(NaiveDoc_Document *theDoc)
    : myDoc(theDoc) {}

NaiveDoc_ObjectTable::~NaiveDoc_ObjectTable() {}

const Handle(AIS_InteractiveContext) & NaiveDoc_ObjectTable::Context() const {
  return myDoc->Context();
}

NaiveDoc_Id NaiveDoc_ObjectTable::AddShape(const TopoDS_Shape &theShape,
                                           Standard_Boolean theToUpdate) {
  Naive_OPEN_COMMAND({});

  auto anAsm = XCAFDoc_DocumentTool::ShapeTool(myDoc->Document()->Main());
  TDF_Label aLabel = anAsm->AddShape(theShape, Standard_True);
  anAsm->UpdateAssemblies();
  auto aPrs = TPrsStd_AISPresentation::Set(aLabel, TNaming_NamedShape::GetID());
  aPrs->SetMode(AIS_Shaded);
  aPrs->Display();
  Handle(NaiveDoc_Object) anObj = aPrs->GetAIS();

  Naive_COMMIT_COMMAND();

  myDoc->OnAddObject(myDoc);

  return aLabel;
}

NaiveDoc_Id NaiveDoc_ObjectTable::AddMesh(const Handle(Poly_Triangulation) &
                                              theMesh,
                                          Standard_Boolean theToUpdate) {
  return {};
}

void NaiveDoc_ObjectTable::Clear(Standard_Boolean theToUpdate) {}

Handle(NaiveDoc_Object)
    NaiveDoc_ObjectTable::Find(const NaiveDoc_Id &theId) const {
  Handle(TPrsStd_AISPresentation) aPrs = NaiveDoc_Attribute::GetPrs(theId);
  if (aPrs.IsNull())
    return nullptr;

  return aPrs->GetAIS();
}

Standard_Boolean
NaiveDoc_ObjectTable::DeleteObject(const NaiveDoc_Id &theId,
                                   Standard_Boolean theToUpdate) {
  return DeleteObjects({theId}, theToUpdate) == 1;
}

Standard_Boolean
NaiveDoc_ObjectTable::DeleteObject(const Handle(NaiveDoc_Object) & theObject,
                                   Standard_Boolean theToUpdate) {
  return DeleteObjects({theObject}, theToUpdate) == 1;
}

Standard_Integer
NaiveDoc_ObjectTable::DeleteObjects(const NaiveDoc_IdList &theIds,
                                    Standard_Boolean theToUpdate) {
  Naive_OPEN_COMMAND(0);

  Standard_Integer nbDelete = 0;
  auto anAsm = XCAFDoc_DocumentTool::ShapeTool(myDoc->Document()->Main());

  for (const NaiveDoc_Id &theId : theIds) {
    auto aPrs = NaiveDoc_Attribute::GetPrs(theId);
    if (aPrs.IsNull())
      continue;

    aPrs->Erase(Standard_True);
    anAsm->RemoveComponent(theId);
    ++nbDelete;
  }

  anAsm->UpdateAssemblies();

  Naive_COMMIT_COMMAND();

  return nbDelete;
}

Standard_Integer
NaiveDoc_ObjectTable::DeleteObjects(const NaiveDoc_ObjectList &theObjects,
                                    Standard_Boolean theToUpdate) {
  Naive_OPEN_COMMAND(0);

  Standard_Integer nbDelete = 0;
  auto anAsm = XCAFDoc_DocumentTool::ShapeTool(myDoc->Document()->Main());

  for (const auto &theObject : theObjects) {
    NaiveDoc_Id anId = NaiveDoc_Attribute::GetId(theObject);
    if (anId.IsNull())
      continue;

    auto aPrs = NaiveDoc_Attribute::GetPrs(anId);
    if (aPrs.IsNull())
      continue;

    aPrs->Erase(Standard_True);
    anAsm->RemoveComponent(anId);
    ++nbDelete;
  }

  anAsm->UpdateAssemblies();

  Naive_COMMIT_COMMAND();

  return nbDelete;
}

Standard_Boolean
NaiveDoc_ObjectTable::ShowObject(const NaiveDoc_Id &theId,
                                 Standard_Boolean theToUpdate) {
  return ShowObjects({theId}, theToUpdate) == 1;
}

Standard_Boolean
NaiveDoc_ObjectTable::ShowObject(const Handle(NaiveDoc_Object) & theObject,
                                 Standard_Boolean theToUpdate) {
  return ShowObjects({theObject}, theToUpdate) == 1;
}

Standard_Integer
NaiveDoc_ObjectTable::ShowObjects(const NaiveDoc_IdList &theIds,
                                  Standard_Boolean theToUpdate) {
  Naive_OPEN_COMMAND(0);

  Standard_Integer nbShow = 0;

  for (const NaiveDoc_Id &theId : theIds) {
    auto aPrs = NaiveDoc_Attribute::GetPrs(theId);
    if (aPrs.IsNull())
      continue;

    if (aPrs->IsDisplayed())
      continue;

    aPrs->Display();
    ++nbShow;
  }

  Naive_COMMIT_COMMAND();

  return nbShow;
}

Standard_Integer
NaiveDoc_ObjectTable::ShowObjects(const NaiveDoc_ObjectList &theObjects,
                                  Standard_Boolean theToUpdate) {
  Naive_OPEN_COMMAND(0);

  Standard_Integer nbShow = 0;

  for (const auto &theObject : theObjects) {
    auto aPrs = NaiveDoc_Attribute::GetPrs(theObject);
    if (aPrs.IsNull())
      continue;

    if (aPrs->IsDisplayed())
      continue;

    aPrs->Display();
    ++nbShow;
  }

  Naive_COMMIT_COMMAND();

  return nbShow;
}

Standard_Integer NaiveDoc_ObjectTable::ShowAll(Standard_Boolean theToUpdate) {
  Naive_OPEN_COMMAND(0);

  Standard_Integer nbShow = 0;

  for (auto anIter = myDoc->GetXcafExplorer(); anIter.More(); anIter.Next()) {
    auto aPrs = NaiveDoc_Attribute::GetPrs(anIter.Current().Label);
    if (aPrs.IsNull())
      continue;

    if (aPrs->IsDisplayed())
      continue;

    aPrs->Display();
    ++nbShow;
  }

  Naive_COMMIT_COMMAND();

  return nbShow;
}

Standard_Boolean
NaiveDoc_ObjectTable::HideObject(const NaiveDoc_Id &theId,
                                 Standard_Boolean theToUpdate) {
  NaiveDoc_IdList anIdList{theId};
  return HideObjects(anIdList, theToUpdate) == 1;
}

Standard_Boolean
NaiveDoc_ObjectTable::HideObject(const Handle(NaiveDoc_Object) & theObject,
                                 Standard_Boolean theToUpdate) {
  NaiveDoc_ObjectList anObjectList{theObject};
  return HideObjects(anObjectList, theToUpdate) == 1;
}

Standard_Integer
NaiveDoc_ObjectTable::HideObjects(const NaiveDoc_IdList &theIds,
                                  Standard_Boolean theToUpdate) {
  Naive_OPEN_COMMAND(0);

  Standard_Integer nbHide = 0;

  for (const NaiveDoc_Id &anId : theIds) {
    Handle(TPrsStd_AISPresentation) aPrs = NaiveDoc_Attribute::GetPrs(anId);
    if (aPrs.IsNull())
      continue;

    aPrs->Erase();
    ++nbHide;
  }

  Naive_COMMIT_COMMAND();

  return nbHide;
}

Standard_Integer
NaiveDoc_ObjectTable::HideObjects(const NaiveDoc_ObjectList &theObjects,
                                  Standard_Boolean theToUpdate) {
  Naive_OPEN_COMMAND(0);

  Standard_Integer nbHide = 0;

  for (const auto &anObj : theObjects) {
    Handle(TPrsStd_AISPresentation) aPrs = NaiveDoc_Attribute::GetPrs(anObj);
    if (aPrs.IsNull())
      continue;

    /// https://tracker.dev.opencascade.org/view.php?id=30142
    /// The AISObject is recreated during UNDO even if this is an ERASE
    /// operation?
    aPrs->Erase();
    ++nbHide;
  }

  Naive_COMMIT_COMMAND();

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
NaiveDoc_ObjectTable::SelectObject(const NaiveDoc_Id &theId,
                                   Standard_Boolean theSelect,
                                   Standard_Boolean theToUpdate) {
  return SelectObject(Find(theId), theSelect, theToUpdate);
}

Standard_Boolean
NaiveDoc_ObjectTable::SelectObject(const Handle(NaiveDoc_Object) & theObject,
                                   Standard_Boolean theSelect,
                                   Standard_Boolean theToUpdate) {
  if (theObject.IsNull() || !Context()->IsDisplayed(theObject))
    return Standard_False;

  if (Context()->IsSelected(theObject) != theSelect)
    Context()->AddOrRemoveSelected(theObject, theToUpdate);

  return Standard_True;
}

Standard_Integer
NaiveDoc_ObjectTable::SelectObjects(const NaiveDoc_IdList &theIds,
                                    Standard_Boolean theSelect,
                                    Standard_Boolean theToUpdate) {
  Standard_Integer aCount = 0;

  for (const NaiveDoc_Id &anId : theIds) {
    if (SelectObject(anId, theSelect, Standard_False))
      aCount++;
  }

  if (theToUpdate)
    myDoc->UpdateView();

  return aCount;
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
  Context()->ClearSelected(Standard_False);
  Standard_Integer nbSelect = 0;

  for (auto anIter = myDoc->GetXcafExplorer(); anIter.More(); anIter.Next()) {
    auto aPrs = NaiveDoc_Attribute::GetPrs(anIter.Current().Label);
    if (aPrs.IsNull())
      continue;
    Handle(NaiveDoc_Object) anObj = aPrs->GetAIS();
    if (anObj.IsNull())
      continue;
    Context()->AddOrRemoveSelected(anObj, Standard_False);
    ++nbSelect;
  }

  if (theToUpdate)
    myDoc->UpdateView();

  return nbSelect;
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
