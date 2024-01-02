#include "NaiveDoc_ObjectTable.hxx"

IMPLEMENT_STANDARD_RTTIEXT(NaiveDoc_ObjectTable, Standard_Transient)

NaiveDoc_ObjectTable::NaiveDoc_ObjectTable() {}

NaiveDoc_ObjectTable::~NaiveDoc_ObjectTable() {}

void NaiveDoc_ObjectTable::AddObject(const Handle(NaiveDoc_Object) &
                                     theObject) {
  myObjects.insert(NaiveDoc_Object_GetId(*theObject), theObject);
}

void NaiveDoc_ObjectTable::RemoveObject(const Handle(NaiveDoc_Object) &
                                        theObject) {
  myObjects.remove(NaiveDoc_Object_GetId(*theObject));
}
