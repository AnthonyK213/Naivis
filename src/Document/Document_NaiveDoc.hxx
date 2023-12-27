#ifndef _Naivis_Io_DocumentManager_Header
#define _Naivis_Io_DocumentManager_Header

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

/// @brief XCAF document manager.
class Document_NaiveDoc {
public:
  Document_NaiveDoc();

  virtual ~Document_NaiveDoc();

public:
  /// @brief Create new XCAF document.
  void NewDocument();

  /// @brief Current document.
  /// @return
  Handle(TDocStd_Document) Document() const;

  /// @brief Iterator of objects in current document.
  /// @param flags
  /// @return
  XCAFPrs_DocumentExplorer
  GetExplorer(const XCAFPrs_DocumentExplorerFlags flags =
                  XCAFPrs_DocumentExplorerFlags_None) const;

  bool ImportStep(const char *filePath);

  bool ExportStep(const char *filePath);

  Handle(Poly_Triangulation) ImportStl(const char *filePath);

  bool ExportStl(const char *filePath,
                 const Handle(Poly_Triangulation) & theMesh);

  void DumpXcafDocumentTree() const;

public:
  static TCollection_AsciiString
  GetXcafNodePathNames(const XCAFPrs_DocumentExplorer &theExpl,
                       bool theIsInstanceName, int theLowerDepth = 0);

protected:
  bool CreateXcafApp();

protected:
  Handle(TDocStd_Application) myApp;

  Handle(TDocStd_Document) myDoc;
};

#endif
