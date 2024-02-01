#include <TDF_LabelMap.hxx>
#include <TNaming_NamedShape.hxx>
#include <TPrsStd_AISPresentation.hxx>
#include <TPrsStd_AISViewer.hxx>
#include <TPrsStd_DriverTable.hxx>
#include <TPrsStd_NamedShapeDriver.hxx>
#include <V3d_Viewer.hxx>
#include <XCAFPrs_AISObject.hxx>
#include <XCAFPrs_Driver.hxx>

#include "NaiveDoc_Document.hxx"
#include "NaiveDoc_ObjectTable.hxx"
#include <Util/Util_XCAF.hxx>

IMPLEMENT_STANDARD_RTTIEXT(NaiveDoc_Document, Standard_Transient)

NaiveDoc_Document::NaiveDoc_Document() {
  createXcafApp();
  myDoc = newDocument();
  myObjects = new NaiveDoc_ObjectTable(this);
}

NaiveDoc_Document::~NaiveDoc_Document() {}

Standard_Boolean NaiveDoc_Document::ImportStep(Standard_CString theFilePath) {
  createXcafApp();
  Handle(TDocStd_Document) aDoc = newDocument();

  if (!Util_XCAF::ImportStep(aDoc, theFilePath)) {
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
  } catch (const Standard_Failure &theFailure) {
    std::cout << "Exception raised during STEP export: "
              << theFailure.GetMessageString() << '\n';

    return Standard_False;
  }

  std::cout << "File " << theFilePath << " is exported successfully" << '\n';

  return Standard_True;
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

  for (XCAFPrs_DocumentExplorer aDocExpl = GetXcafExplorer(); aDocExpl.More();
       aDocExpl.Next()) {
    TCollection_AsciiString aName = Util_XCAF::GetXcafNodePathNames(
        aDocExpl, Standard_False, aDocExpl.CurrentDepth());
    aName = TCollection_AsciiString(aDocExpl.CurrentDepth() * 2, ' ') + aName +
            " @" + aDocExpl.Current().Id;
    std::cout << aName << '\n';
  }

  std::cout << std::endl;
}

Standard_Boolean NaiveDoc_Document::createXcafApp() {
  if (!myApp.IsNull())
    return Standard_True;

  try {
    myApp = new TDocStd_Application();
    BinXCAFDrivers::DefineFormat(myApp);
    return Standard_True;
  } catch (const Standard_Failure &theFailure) {
    std::cout << "Error: " << theFailure.GetMessageString() << '\n';
    return Standard_False;
  }
}

Handle(TDocStd_Document) NaiveDoc_Document::newDocument() {
  Handle(TDocStd_Document) aDoc;

  if (!myApp.IsNull())
    myApp->NewDocument(TCollection_ExtendedString("BinXCAF"), aDoc);

  if (!aDoc.IsNull()) {
    aDoc->SetUndoLimit(100);
  }

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
  if (myDoc.IsNull())
    return;

  Handle(TPrsStd_AISViewer) aViewer;
  if (TPrsStd_AISViewer::Find(myDoc->Main(), aViewer)) {
    aViewer->SetInteractiveContext(myContext);
  } else {
    aViewer = TPrsStd_AISViewer::New(myDoc->Main(), myContext);
  }

  TDF_LabelMap anAsmMap{};
  Standard_Boolean skip = Standard_False;
  Standard_Integer skipDepth = 0;

  for (XCAFPrs_DocumentExplorer aDocExpl = GetXcafExplorer(); aDocExpl.More();
       aDocExpl.Next()) {
    Standard_Integer aDepth = aDocExpl.CurrentDepth();

    /// To skip assembly instances which have already been visited.
    /// It seems that FreeCAD makes new copies for the assembly instances, but
    /// this breaks down the connections between the identical instances.
    /// The solution below is, at the first time visit one instance, visit its
    /// children normally; for the rest visits, just display the assembly
    /// instance and visit its siblings.
    /// Maybe there is a better solution? Never visit the instance's children
    /// unless the user enters "assembly editing mode"? This might make more
    /// sense.

    if (skip) {
      if (aDepth > skipDepth)
        continue;
      else
        skip = Standard_False;
    }

    const XCAFPrs_DocumentNode &aNode = aDocExpl.Current();

    if (aNode.IsAssembly) {
      if (anAsmMap.Contains(aNode.RefLabel)) {
        skip = Standard_True;
        skipDepth = aDepth;
      } else {
        anAsmMap.Add(aNode.RefLabel);
        continue;
      }
    }

    Handle(TPrsStd_AISPresentation) aPrs;
    if (!aNode.Label.FindAttribute(TPrsStd_AISPresentation::GetID(), aPrs)) {
      aPrs = TPrsStd_AISPresentation::Set(aNode.Label, XCAFPrs_Driver::GetID());
    }

    aPrs->Display(Standard_True);
    Handle(AIS_InteractiveObject) anObj = aPrs->GetAIS();

    if (anObj.IsNull())
      continue;

    if (aDepth > 0) {
      const XCAFPrs_DocumentNode &aFather = aDocExpl.Current(aDepth - 1);
      anObj->SetLocalTransformation(aFather.Location);
    }

    anObj->SetDisplayMode(AIS_Shaded);
    myContext->Display(anObj, Standard_False);
  }
}
