#include <V3d_Viewer.hxx>

#include "NaiveDoc_CmdAddObjects.hxx"
#include "NaiveDoc_Document.hxx"
#include "NaiveDoc_ObjectTable.hxx"

#include <QStack>

static TCollection_AsciiString
getXcafNodeName(const XCAFPrs_DocumentNode &theNode,
                Standard_Boolean theIsInstanceName) {
  Handle(TDataStd_Name) aNodeName;

  if (theIsInstanceName) {
    if (theNode.Label.FindAttribute(TDataStd_Name::GetID(), aNodeName)) {
      return aNodeName->Get();
    }
  } else {
    if (theNode.RefLabel.FindAttribute(TDataStd_Name::GetID(), aNodeName)) {
      return aNodeName->Get();
    }
  }

  return {};
}

static TCollection_AsciiString
getXcafNodePathNames(const XCAFPrs_DocumentExplorer &theExpl,
                     Standard_Boolean theIsInstanceName,
                     Standard_Integer theLowerDepth) {
  TCollection_AsciiString aPath;

  for (Standard_Integer aDepth = theLowerDepth;
       aDepth <= theExpl.CurrentDepth(); ++aDepth) {
    const XCAFPrs_DocumentNode &aNode = theExpl.Current(aDepth);
    TCollection_AsciiString aName = getXcafNodeName(aNode, theIsInstanceName);

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

IMPLEMENT_STANDARD_RTTIEXT(NaiveDoc_Document, Standard_Transient)

NaiveDoc_Document::NaiveDoc_Document() {
  createXcafApp();
  myObjects = new NaiveDoc_ObjectTable(this);
}

NaiveDoc_Document::~NaiveDoc_Document() {}

Standard_Boolean NaiveDoc_Document::ImportStep(Standard_CString theFilePath) {
  createXcafApp();
  Handle(TDocStd_Document) aDoc = newDocument();

  if (!importStep(aDoc, theFilePath)) {
    return Standard_False;
  }

  std::swap(myDoc, aDoc);
  closeDocument(aDoc);
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

  for (XCAFPrs_DocumentExplorer aDocExpl = getXcafExplorer(); aDocExpl.More();
       aDocExpl.Next()) {
    TCollection_AsciiString aName =
        getXcafNodePathNames(aDocExpl, false, aDocExpl.CurrentDepth());
    aName = TCollection_AsciiString(aDocExpl.CurrentDepth() * 2, ' ') + aName +
            " @" + aDocExpl.Current().Id;
    std::cout << aName << '\n';
  }

  std::cout << std::endl;
}

void NaiveDoc_Document::DumpXcafDocumentTree(QTreeWidget *theTree) const {
  if (myDoc.IsNull() || !theTree)
    return;

  theTree->clear();

  QTreeWidgetItem *aRoot = new QTreeWidgetItem();
  aRoot->setText(0, "Assemblies");
  QStack<QTreeWidgetItem *> aStack{};
  aStack.push(aRoot);

  for (XCAFPrs_DocumentExplorer aDocExpl = getXcafExplorer(); aDocExpl.More();
       aDocExpl.Next()) {
    Standard_Integer aDepth = aDocExpl.CurrentDepth();
    TCollection_AsciiString aName =
        getXcafNodeName(aDocExpl.Current(), Standard_False);

    for (int i = 1; i < aStack.size() - aDepth; ++i) {
      aStack.pop();
    }

    if (aDepth == aStack.size() - 1) {
      QTreeWidgetItem *anItem = new QTreeWidgetItem();
      anItem->setText(0, aName.ToCString());
      QTreeWidgetItem *aParent = aStack.top();
      aParent->addChild(anItem);
      aStack.push(anItem);
    }
  }

  theTree->addTopLevelItem(aRoot);
  theTree->expandAll();
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

Handle(TDocStd_Document) NaiveDoc_Document::newDocument() {
  Handle(TDocStd_Document) aDoc;

  if (!myApp.IsNull())
    myApp->NewDocument(TCollection_ExtendedString("BinXCAF"), aDoc);

  if (!myDoc.IsNull())
    myDoc->SetUndoLimit(100);

  return aDoc;
}

void NaiveDoc_Document::closeDocument(Handle(TDocStd_Document) & theDoc,
                                      Standard_Boolean theToUpdate) {
  if (!theDoc.IsNull()) {
    if (theDoc->HasOpenCommand())
      theDoc->AbortCommand();

    theDoc->Main().Root().ForgetAllAttributes(Standard_True);
    myApp->Close(theDoc);
    theDoc.Nullify();
  }

  myObjects->purgeAllRaw(theToUpdate);
}

void NaiveDoc_Document::displayXcafDoc() {
  if (myDoc.IsNull()) {
    return;
  }

  NaiveDoc_ObjectList anObjList{};

  for (XCAFPrs_DocumentExplorer aDocExpl = getXcafExplorer(); aDocExpl.More();
       aDocExpl.Next()) {
    const XCAFPrs_DocumentNode &aNode = aDocExpl.Current();

    if (aNode.IsAssembly) {
      continue;
    }

    Handle(XCAFPrs_AISObject) anObj = new XCAFPrs_AISObject(aNode.RefLabel);
    anObj->SetLocalTransformation(aNode.Location);

    QString aName = QString::fromUtf8(
        getXcafNodePathNames(aDocExpl, false, aDocExpl.CurrentDepth())
            .ToCString());

    NaiveDoc_Object_SetId(*anObj);

    if (!aName.isNull() && !aName.isEmpty())
      NaiveDoc_Object_SetName(*anObj, aName);

    anObj->SetDisplayMode(AIS_Shaded);

    anObjList.push_back(anObj);
  }

  myObjects->addObjects(anObjList, Standard_True);
}

Standard_Boolean NaiveDoc_Document::importStep(Handle(TDocStd_Document) &
                                                   theDoc,
                                               Standard_CString theFilePath) {
  if (theDoc.IsNull()) {
    std::cout << "Error: The document is null\n";
    return Standard_False;
  }

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

    if (!aReader.Transfer(theDoc)) {
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
