#include "Document_NaiveDoc.hxx"

Document_NaiveDoc::Document_NaiveDoc() { CreateXcafApp(); }

Document_NaiveDoc::~Document_NaiveDoc() {}

bool Document_NaiveDoc::CreateXcafApp() {
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

void Document_NaiveDoc::NewDocument() {
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

Handle(TDocStd_Document) Document_NaiveDoc::Document() const { return myDoc; }

XCAFPrs_DocumentExplorer Document_NaiveDoc::GetExplorer(
    const XCAFPrs_DocumentExplorerFlags flags) const {
  return {myDoc, flags};
}

bool Document_NaiveDoc::ImportStep(const char *filePath) {
  CreateXcafApp();
  NewDocument();

  STEPCAFControl_Controller::Init();
  STEPControl_Controller::Init();

  STEPCAFControl_Reader aReader;

  aReader.SetColorMode(Standard_True);
  aReader.SetNameMode(Standard_True);
  aReader.SetLayerMode(Standard_True);
  aReader.SetSHUOMode(Standard_True);

  try {
    if (aReader.ReadFile(filePath) != IFSelect_RetDone) {
      std::cout << "Error: On reading STEP file " << filePath << '\n';
      return false;
    }

    if (!aReader.Transfer(myDoc)) {
      std::cout << "Error: On transferring STEP file " << filePath << '\n';
      return false;
    }

    std::cout << "File " << filePath << " is imported successfully" << '\n';
    return true;
  } catch (const Standard_Failure &theFailure) {
    std::cout << "Exception raised during STEP import: "
              << theFailure.GetMessageString() << '\n';
    return false;
  }
}

bool Document_NaiveDoc::ExportStep(const char *filePath) {
  STEPCAFControl_Writer aWriter;

  aWriter.SetColorMode(Standard_True);
  aWriter.SetNameMode(Standard_True);
  aWriter.SetLayerMode(Standard_True);
  aWriter.SetSHUOMode(Standard_True);

  try {
    if (aWriter.Transfer(myDoc) != IFSelect_RetDone) {
      std::cout << "Error: On transferring STEP file " << filePath << '\n';
      return false;
    }

    if (aWriter.Write(filePath) != IFSelect_RetDone) {
      std::cout << "Error: On writing STEP file " << filePath << '\n';
      return false;
    }

    std::cout << "File " << filePath << " is exported successfully" << '\n';
    return true;
  } catch (const Standard_Failure &theFailure) {
    std::cout << "Exception raised during STEP export: "
              << theFailure.GetMessageString() << '\n';
    return false;
  }
}

Handle(Poly_Triangulation) Document_NaiveDoc::ImportStl(const char *filePath) {
  try {
    Handle(Poly_Triangulation) aStlMesh = RWStl::ReadFile(filePath);
    return aStlMesh;
  } catch (const Standard_Failure &theFailure) {
    std::cout << "Exception raised during STL import: "
              << theFailure.GetMessageString() << '\n';
    return nullptr;
  }
}

bool Document_NaiveDoc::ExportStl(const char *filePath,
                                  const Handle(Poly_Triangulation) & theMesh) {
  try {
    OSD_Path aPath{filePath};
    return RWStl::WriteAscii(theMesh, aPath);
  } catch (const Standard_Failure &theFailure) {
    std::cout << "Exception raised during STL export: "
              << theFailure.GetMessageString() << '\n';
    return false;
  }
}

void Document_NaiveDoc::DumpXcafDocumentTree() const {
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
Document_NaiveDoc::GetXcafNodePathNames(const XCAFPrs_DocumentExplorer &theExpl,
                                        bool theIsInstanceName,
                                        int theLowerDepth) {
  TCollection_AsciiString aPath;

  for (int aDepth = theLowerDepth; aDepth <= theExpl.CurrentDepth(); ++aDepth) {
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
