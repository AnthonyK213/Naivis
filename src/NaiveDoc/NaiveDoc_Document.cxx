#include <V3d_Viewer.hxx>

#include "NaiveDoc_CmdAddObjects.hxx"
#include "NaiveDoc_Document.hxx"
#include "NaiveDoc_ObjectTable.hxx"

IMPLEMENT_STANDARD_RTTIEXT(NaiveDoc_Document, Standard_Transient)

NaiveDoc_Document::NaiveDoc_Document() {
  createXcafApp();

  myObjects = new NaiveDoc_ObjectTable();

  myContext = nullptr;

  myUndoStack = new QUndoStack();
  myUndoStack->setUndoLimit(100);
}

NaiveDoc_Document::~NaiveDoc_Document() { delete myUndoStack; }

void NaiveDoc_Document::NewDocument() {
  if (!myDoc.IsNull()) {
    if (myDoc->HasOpenCommand())
      myDoc->AbortCommand();

    myDoc->Main().Root().ForgetAllAttributes(Standard_True);
    myApp->Close(myDoc);
    myDoc.Nullify();
  }

  if (!myApp.IsNull())
    myApp->NewDocument(TCollection_ExtendedString("BinXCAF"), myDoc);

  if (!myDoc.IsNull())
    myDoc->SetUndoLimit(10);
}

Handle(TDocStd_Document) NaiveDoc_Document::Document() const { return myDoc; }

Handle(NaiveDoc_ObjectTable) NaiveDoc_Document::Objects() const {
  return myObjects;
}

Handle(AIS_InteractiveContext) NaiveDoc_Document::Context() const {
  return myContext;
}

void NaiveDoc_Document::SetContext(const Handle(AIS_InteractiveContext) &
                                   theContext) {
  myContext = theContext;
}

XCAFPrs_DocumentExplorer NaiveDoc_Document::GetExplorer(
    const XCAFPrs_DocumentExplorerFlags flags) const {
  return {myDoc, flags};
}

Standard_Boolean NaiveDoc_Document::ImportStep(Standard_CString theFilePath) {
  createXcafApp();
  NewDocument();

  if (!importStep(theFilePath)) {
    return Standard_False;
  }

  displayXcafDoc();
  return Standard_True;
}

Standard_Boolean NaiveDoc_Document::ExportStep(Standard_CString theFilePath) {
  STEPCAFControl_Writer aWriter;

  aWriter.SetColorMode(Standard_True);
  aWriter.SetNameMode(Standard_True);
  aWriter.SetLayerMode(Standard_True);
  aWriter.SetSHUOMode(Standard_True);

  try {
    if (aWriter.Transfer(myDoc) != IFSelect_RetDone) {
      std::cout << "Error: On transferring STEP file " << theFilePath << '\n';
      return Standard_False;
    }

    if (aWriter.Write(theFilePath) != IFSelect_RetDone) {
      std::cout << "Error: On writing STEP file " << theFilePath << '\n';
      return Standard_False;
    }

    std::cout << "File " << theFilePath << " is exported successfully" << '\n';
    return Standard_True;
  } catch (const Standard_Failure &theFailure) {
    std::cout << "Exception raised during STEP export: "
              << theFailure.GetMessageString() << '\n';
    return Standard_True;
  }
}

Handle(Poly_Triangulation)
    NaiveDoc_Document::ImportStl(Standard_CString theFilePath) {
  try {
    Handle(Poly_Triangulation) aStlMesh = RWStl::ReadFile(theFilePath);
    return aStlMesh;
  } catch (const Standard_Failure &theFailure) {
    std::cout << "Exception raised during STL import: "
              << theFailure.GetMessageString() << '\n';
    return nullptr;
  }
}

Standard_Boolean NaiveDoc_Document::ExportStl(Standard_CString theFilePath,
                                              const Handle(Poly_Triangulation) &
                                                  theMesh) {
  try {
    OSD_Path aPath{theFilePath};
    return RWStl::WriteAscii(theMesh, aPath);
  } catch (const Standard_Failure &theFailure) {
    std::cout << "Exception raised during STL export: "
              << theFailure.GetMessageString() << '\n';
    return Standard_False;
  }
}

void NaiveDoc_Document::DumpXcafDocumentTree() const {
  if (myDoc.IsNull())
    return;

  for (XCAFPrs_DocumentExplorer aDocExpl = GetExplorer(); aDocExpl.More();
       aDocExpl.Next()) {
    TCollection_AsciiString aName =
        getXcafNodePathNames(aDocExpl, false, aDocExpl.CurrentDepth());
    aName = TCollection_AsciiString(aDocExpl.CurrentDepth() * 2, ' ') + aName +
            " @" + aDocExpl.Current().Id;
    std::cout << aName << '\n';
  }

  std::cout << std::endl;
}

void NaiveDoc_Document::Undo() { myUndoStack->undo(); }

void NaiveDoc_Document::Redo() { myUndoStack->redo(); }

void NaiveDoc_Document::UpdateView() { myContext->CurrentViewer()->Redraw(); }

Handle(NaiveDoc_Object) NaiveDoc_Document::AddShape(
    const TopoDS_Shape &theShape, Standard_Boolean theToUpdate) {
  Handle(NaiveDoc_Object) anObj = new AIS_Shape(theShape);
  NaiveDoc_Object_SetId(*anObj);
  anObj->SetDisplayMode(AIS_Shaded);

  addObject(anObj, theToUpdate);

  return anObj;
}

void NaiveDoc_Document::addObject(const Handle(NaiveDoc_Object) & theObject,
                                  Standard_Boolean theToUpdate) {
  NaiveDoc_ObjectList anObjectList{theObject};
  addObjects(anObjectList, theToUpdate);
}

void NaiveDoc_Document::addObjects(const NaiveDoc_ObjectList &theObjects,
                                   Standard_Boolean theToUpdate) {
  NaiveDoc_CmdAddObjects *cmd =
      new NaiveDoc_CmdAddObjects(this, theObjects, theToUpdate);
  myUndoStack->push(cmd);
}

TCollection_AsciiString
NaiveDoc_Document::getXcafNodePathNames(const XCAFPrs_DocumentExplorer &theExpl,
                                        Standard_Boolean theIsInstanceName,
                                        Standard_Integer theLowerDepth) {
  TCollection_AsciiString aPath;

  for (Standard_Integer aDepth = theLowerDepth;
       aDepth <= theExpl.CurrentDepth(); ++aDepth) {
    const XCAFPrs_DocumentNode &aNode = theExpl.Current(aDepth);
    TCollection_AsciiString aName;
    Handle(TDataStd_Name) aNodeName;

    if (theIsInstanceName) {
      if (aNode.Label.FindAttribute(TDataStd_Name::GetID(), aNodeName)) {
        aName = aNodeName->Get();
      }
    } else {
      if (aNode.RefLabel.FindAttribute(TDataStd_Name::GetID(), aNodeName)) {
        aName = aNodeName->Get();
      }
    }

    if (aName.IsEmpty()) {
      TDF_Tool::Entry(aNode.Label, aName);
    }

    if (aNode.IsAssembly) {
      aName += "$";
    }

    aPath += aName;
  }

  return aPath;
}

void NaiveDoc_Document::displayXcafDoc() {
  if (myDoc.IsNull()) {
    return;
  }

  NaiveDoc_ObjectList anObjList{};

  for (XCAFPrs_DocumentExplorer aDocExpl = GetExplorer(); aDocExpl.More();
       aDocExpl.Next()) {
    const XCAFPrs_DocumentNode &aNode = aDocExpl.Current();

    if (aNode.IsAssembly) {
      continue;
    }

    Handle(XCAFPrs_AISObject) anObj = new XCAFPrs_AISObject(aNode.RefLabel);
    anObj->SetLocalTransformation(aNode.Location);
    anObj->SetOwner(new TCollection_HAsciiString(aNode.Id));

    QString aName =
        QString::fromUtf8(getXcafNodePathNames(aDocExpl, false, 1).ToCString());

    NaiveDoc_Object_SetId(*anObj);
    if (aName.isNull() || aName.isEmpty())
      NaiveDoc_Object_SetName(*anObj, aName);
    anObj->SetDisplayMode(AIS_Shaded);

    anObjList.push_back(anObj);
  }

  addObjects(anObjList, Standard_True);
}

Standard_Boolean NaiveDoc_Document::importStep(Standard_CString theFilePath) {
  STEPCAFControl_Controller::Init();
  STEPControl_Controller::Init();

  STEPCAFControl_Reader aReader;

  aReader.SetColorMode(Standard_True);
  aReader.SetNameMode(Standard_True);
  aReader.SetLayerMode(Standard_True);
  aReader.SetSHUOMode(Standard_True);

  try {
    if (aReader.ReadFile(theFilePath) != IFSelect_RetDone) {
      std::cout << "Error: On reading STEP file " << theFilePath << '\n';
      return Standard_False;
    }

    if (!aReader.Transfer(myDoc)) {
      std::cout << "Error: On transferring STEP file " << theFilePath << '\n';
      return Standard_False;
    }

    std::cout << "File " << theFilePath << " is imported successfully" << '\n';

    return Standard_True;
  } catch (const Standard_Failure &theFailure) {
    std::cout << "Exception raised during STEP import: "
              << theFailure.GetMessageString() << '\n';
    return Standard_False;
  }
}

Standard_Boolean NaiveDoc_Document::createXcafApp() {
  if (!myApp.IsNull())
    return true;

  try {
    myApp = new TDocStd_Application();
    BinXCAFDrivers::DefineFormat(myApp);
    return true;
  } catch (const Standard_Failure &theFailure) {
    std::cout << "Error: " << theFailure.GetMessageString() << std::endl;
    return false;
  }
}
