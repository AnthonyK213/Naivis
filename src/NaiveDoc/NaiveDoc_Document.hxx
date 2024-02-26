#ifndef _Naivis_NaiveDoc_Document_HeaderFile
#define _Naivis_NaiveDoc_Document_HeaderFile

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
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFPrs_DocumentExplorer.hxx>
#include <XCAFPrs_DocumentNode.hxx>

#include "NaiveDoc_Object.hxx"
#include "NaiveDoc_ObjectTable.hxx"
#include <NaiveApp/NaiveApp_Application.hxx>

#include <QObject>

using NaiveDoc_DocumentNode = XCAFPrs_DocumentNode;

class NaiveDoc_Document : public QObject, public Standard_Transient {
  Q_OBJECT

public:
  NaiveDoc_Document();

  ~NaiveDoc_Document();

public:
  const Handle(NaiveDoc_ObjectTable) & Objects() const { return myObjects; }

  const Handle(AIS_InteractiveContext) & Context() const { return myContext; }

  void SetContext(const Handle(AIS_InteractiveContext) & theContext);

  const Handle(TDocStd_Document) & Document() const { return myDoc; }

  Standard_Boolean ImportStep(Standard_CString theFilePath);

  Standard_Boolean ExportStep(Standard_CString theFilePath);

  Handle(Poly_Triangulation) ImportStl(Standard_CString theFilePath);

  Standard_Boolean ExportStl(Standard_CString theFilePath,
                             const Handle(Poly_Triangulation) & theMesh);

  void DumpXcafDocumentTree() const;

  void Undo();

  void Redo();

  void UpdateView();

  XCAFPrs_DocumentExplorer
  GetXcafExplorer(const XCAFPrs_DocumentExplorerFlags theFlags =
                      XCAFPrs_DocumentExplorerFlags_None) const;

  DEFINE_STANDARD_RTTIEXT(NaiveDoc_Document, Standard_Transient)

signals:
  void OnAddObject(const Handle(NaiveDoc_Document) & theDoc);

  void OnDeleteObject();

  void OnUndeleteObject();

private:
  Standard_Boolean createXcafApp();

  Handle(TDocStd_Document)
      newDocument(Standard_Boolean theEmpty = Standard_False);

  void closeDocument(Handle(TDocStd_Document) & theDoc,
                     Standard_Boolean theToUpdate = Standard_False);

  void displayXcafDoc();

private:
  Handle(NaiveApp_Application) myApp;
  Handle(TDocStd_Document) myDoc;
  Handle(AIS_InteractiveContext) myContext;
  Handle(NaiveDoc_ObjectTable) myObjects;
};

DEFINE_STANDARD_HANDLE(NaiveDoc_Document, Standard_Transient)

#endif
