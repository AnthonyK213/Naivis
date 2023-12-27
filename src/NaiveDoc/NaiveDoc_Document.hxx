#ifndef _Naivis_NaiveDoc_Document_HeaderFile
#define _Naivis_NaiveDoc_Document_HeaderFile

#include <AIS_InteractiveContext.hxx>
#include <BinXCAFDrivers.hxx>
#include <OSD_Path.hxx>
#include <RWStl.hxx>
#include <STEPCAFControl_Controller.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <STEPCAFControl_Writer.hxx>
#include <STEPControl_Controller.hxx>
#include <STEPControl_Reader.hxx>
#include <STEPControl_Writer.hxx>
#include <Standard.hxx>
#include <Standard_Transient.hxx>
#include <Standard_Type.hxx>
#include <StlAPI_Reader.hxx>
#include <TDF_Label.hxx>
#include <TDF_LabelSequence.hxx>
#include <TDF_Tool.hxx>
#include <TDataStd_Name.hxx>
#include <TDocStd_Application.hxx>
#include <TDocStd_Document.hxx>
#include <TopoDS_Shape.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFPrs_AISObject.hxx>
#include <XCAFPrs_DocumentExplorer.hxx>
#include <XCAFPrs_DocumentNode.hxx>

class NaiveDoc_Document;
DEFINE_STANDARD_HANDLE(NaiveDoc_Document, Standard_Transient)

class NaiveDoc_Document : public Standard_Transient {
public:
  NaiveDoc_Document();

  ~NaiveDoc_Document();

public:
  void NewDocument();

  Handle(TDocStd_Document) Document() const;

  XCAFPrs_DocumentExplorer
  GetExplorer(const XCAFPrs_DocumentExplorerFlags theFlags =
                  XCAFPrs_DocumentExplorerFlags_None) const;

  Standard_Boolean ImportStep(Standard_CString theFilePath);

  Standard_Boolean ExportStep(Standard_CString theFilePath);

  Handle(Poly_Triangulation) ImportStl(Standard_CString theFilePath);

  Standard_Boolean ExportStl(Standard_CString theFilePath,
                             const Handle(Poly_Triangulation) & theMesh);

  void DumpXcafDocumentTree() const;

public:
  static TCollection_AsciiString
  GetXcafNodePathNames(const XCAFPrs_DocumentExplorer &theExpl,
                       Standard_Boolean theIsInstanceName,
                       Standard_Integer theLowerDepth = 0);

  DEFINE_STANDARD_RTTIEXT(NaiveDoc_Document, Standard_Transient)

protected:
  Standard_Boolean createXcafApp();

protected:
  Handle(TDocStd_Application) myApp;
  Handle(TDocStd_Document) myDoc;
};

#endif
