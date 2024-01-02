#ifndef _Naivis_NaiveDoc_ObjectTable_HeaderFile
#define _Naivis_NaiveDoc_ObjectTable_HeaderFile

#include <Standard_Handle.hxx>
#include <Standard_Type.hxx>

#include <QHash>
#include <QUuid>

#include "NaiveDoc_Object.hxx"

class NaiveDoc_ObjectTable;
DEFINE_STANDARD_HANDLE(NaiveDoc_ObjectTable, Standard_Transient)

class NaiveDoc_ObjectTable : public Standard_Transient {
public:
  NaiveDoc_ObjectTable();

  ~NaiveDoc_ObjectTable();

public:
  void AddObject(const Handle(NaiveDoc_Object) & theObject);

  void RemoveObject(const Handle(NaiveDoc_Object) & theObject);

  DEFINE_STANDARD_RTTIEXT(NaiveDoc_ObjectTable, Standard_Transient)

private:
  QHash<QUuid, Handle(NaiveDoc_Object)> myObjects;
};

#endif
