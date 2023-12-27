#include "NaiveDoc_Document.hxx"

IMPLEMENT_STANDARD_RTTIEXT(NaiveDoc_Document, Standard_Transient)

NaiveDoc_Document::NaiveDoc_Document() { createXcafApp(); }

NaiveDoc_Document::~NaiveDoc_Document() {}

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

XCAFPrs_DocumentExplorer NaiveDoc_Document::GetExplorer(
    const XCAFPrs_DocumentExplorerFlags flags) const {
  return {myDoc, flags};
}

Standard_Boolean NaiveDoc_Document::ImportStep(Standard_CString theFilePath) {
  createXcafApp();
  NewDocument();

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
        GetXcafNodePathNames(aDocExpl, false, aDocExpl.CurrentDepth());
    aName = TCollection_AsciiString(aDocExpl.CurrentDepth() * 2, ' ') + aName +
            " @" + aDocExpl.Current().Id;
    std::cout << aName << '\n';
  }

  std::cout << std::endl;
}

TCollection_AsciiString
NaiveDoc_Document::GetXcafNodePathNames(const XCAFPrs_DocumentExplorer &theExpl,
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
