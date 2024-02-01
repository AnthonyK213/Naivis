#ifndef _Naivis_Util_OCAF_HeaderFile
#define _Naivis_Util_OCAF_HeaderFile

#include <TDF_Tool.hxx>
#include <TDataStd_Name.hxx>
#include <TDocStd_Document.hxx>
#include <TPrsStd_AISPresentation.hxx>
#include <TPrsStd_AISViewer.hxx>
#include <XCAFPrs_DocumentExplorer.hxx>
#include <XCAFPrs_DocumentNode.hxx>
#include <XCAFPrs_Driver.hxx>

namespace Util_OCAF {

TCollection_AsciiString GetXcafNodeName(const XCAFPrs_DocumentNode &theNode,
                                        Standard_Boolean theIsInstanceName);

TCollection_AsciiString
GetXcafNodePathNames(const XCAFPrs_DocumentExplorer &theExpl,
                     Standard_Boolean theIsInstanceName,
                     Standard_Integer theLowerDepth);

Standard_Boolean ImportStep(Handle(TDocStd_Document) & theDoc,
                            Standard_CString theFilePath);

// Handle(TPrsStd_AISPresentation) GetXcafPrs(const TDF_Label &theLabel);

Handle(TPrsStd_AISViewer) InitAISViewer(const Handle(TDocStd_Document) & theDoc,
                   const Handle(AIS_InteractiveContext) & theCtx);

} // namespace Util_OCAF

#endif
