#include "documentManager.h"

DocumentManager::DocumentManager() { createXcafApp(); }

DocumentManager::~DocumentManager() {}

bool DocumentManager::createXcafApp() {
  if (!m_app.IsNull())
    return true;

  try {
    m_app = new TDocStd_Application();
    BinXCAFDrivers::DefineFormat(m_app);
    return true;
  } catch (const Standard_Failure &theFailure) {
    std::cout << "Error: " << theFailure.GetMessageString() << std::endl;
    return false;
  }
}

void DocumentManager::newDocument() {
  if (!m_doc.IsNull()) {
    if (m_doc->HasOpenCommand())
      m_doc->AbortCommand();

    m_doc->Main().Root().ForgetAllAttributes(Standard_True);
    m_app->Close(m_doc);
    m_doc.Nullify();
  }

  if (!m_app.IsNull())
    m_app->NewDocument(TCollection_ExtendedString("BinXCAF"), m_doc);

  if (!m_doc.IsNull())
    m_doc->SetUndoLimit(10);
}

Handle(TDocStd_Document) DocumentManager::document() const { return m_doc; }

XCAFPrs_DocumentExplorer
DocumentManager::explorer(const XCAFPrs_DocumentExplorerFlags flags) const {
  return {m_doc, flags};
}

bool DocumentManager::importStep(const char *filePath) {
  createXcafApp();
  newDocument();

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

    if (!aReader.Transfer(m_doc)) {
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

bool DocumentManager::exportStep(const char *filePath) {
  STEPCAFControl_Writer aWriter;

  aWriter.SetColorMode(Standard_True);
  aWriter.SetNameMode(Standard_True);
  aWriter.SetLayerMode(Standard_True);
  aWriter.SetSHUOMode(Standard_True);

  try {
    if (aWriter.Transfer(m_doc) != IFSelect_RetDone) {
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

Handle(Poly_Triangulation) DocumentManager::importStl(const char *filePath) {
  try {
    Handle(Poly_Triangulation) aStlMesh = RWStl::ReadFile(filePath);
    return aStlMesh;
  } catch (const Standard_Failure &theFailure) {
    std::cout << "Exception raised during STL import: "
              << theFailure.GetMessageString() << '\n';
    return nullptr;
  }
}

bool DocumentManager::exportStl(const char *filePath,
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

void DocumentManager::dumpXcafDocumentTree() const {
  if (m_doc.IsNull())
    return;

  for (XCAFPrs_DocumentExplorer aDocExpl = explorer(); aDocExpl.More();
       aDocExpl.Next()) {
    TCollection_AsciiString aName =
        getXcafNodePathNames(aDocExpl, false, aDocExpl.CurrentDepth());
    aName = TCollection_AsciiString(aDocExpl.CurrentDepth() * 2, ' ') + aName +
            " @" + aDocExpl.Current().Id;
    std::cout << aName << '\n';
  }

  std::cout << std::endl;
}

TCollection_AsciiString
DocumentManager::getXcafNodePathNames(const XCAFPrs_DocumentExplorer &theExpl,
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
