#ifndef _Naivis_NaiveDoc_Document_HeaderFile
#define _Naivis_NaiveDoc_Document_HeaderFile

#include "NaiveDoc_Attribute.hxx"
#include "NaiveDoc_Object.hxx"
#include "NaiveDoc_ObjectTable.hxx"

#include <luaocct/LODoc_Document.hxx>

#include <QObject>

typedef XCAFPrs_DocumentNode NaiveDoc_DocumentNode;

class NaiveDoc_Document : public QObject, public LODoc_Document {
  Q_OBJECT

public:
  NaiveDoc_Document();

  virtual void AddObjectEvent(const Handle(LODoc_Document) &
                              theDoc) Standard_OVERRIDE;

  DEFINE_STANDARD_RTTIEXT(NaiveDoc_Document, LODoc_Document)

private:
  virtual void initDriverID() Standard_OVERRIDE;

signals:
  void OnAddObject(const Handle(NaiveDoc_Document) & theDoc);

  void OnDeleteObject();

  void OnUndeleteObject();
};

DEFINE_STANDARD_HANDLE(NaiveDoc_Document, Standard_Transient)

#endif
