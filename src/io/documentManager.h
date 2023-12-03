#ifndef NAIVIS_IO_DOCUMENTMANAGER_H
#define NAIVIS_IO_DOCUMENTMANAGER_H

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
class DocumentManager {
public:
  DocumentManager();

  virtual ~DocumentManager();

public:
  /// @brief Create new XCAF document.
  void newDocument();

  /// @brief Current document.
  /// @return
  Handle(TDocStd_Document) document() const;

  /// @brief Iterator of objects in current document.
  /// @param flags
  /// @return
  XCAFPrs_DocumentExplorer
  explorer(const XCAFPrs_DocumentExplorerFlags flags =
               XCAFPrs_DocumentExplorerFlags_None) const;

  bool importStep(const char *filePath);

  bool exportStep(const char *filePath);

  Handle(Poly_Triangulation) importStl(const char *filePath);

  bool exportStl(const char *filePath,
                 const Handle(Poly_Triangulation) & theMesh);

  void dumpXcafDocumentTree() const;

public:
  static TCollection_AsciiString
  getXcafNodePathNames(const XCAFPrs_DocumentExplorer &theExpl,
                       bool theIsInstanceName, int theLowerDepth = 0);

protected:
  bool createXcafApp();

protected:
  Handle(TDocStd_Application) m_app;

  Handle(TDocStd_Document) m_doc;
};

#endif
