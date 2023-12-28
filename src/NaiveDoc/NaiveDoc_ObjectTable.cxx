#include "NaiveDoc_ObjectTable.hxx"
#include "NaiveDoc_Object.hxx"

IMPLEMENT_STANDARD_RTTIEXT(NaiveDoc_ObjectTable, Standard_Transient)

NaiveDoc_ObjectTable::NaiveDoc_ObjectTable() {}

NaiveDoc_ObjectTable::~NaiveDoc_ObjectTable() {}

void NaiveDoc_ObjectTable::AddObject(const Handle(NaiveDoc_Object) &
                                     theObject) {
  myObjects.insert(theObject->Id(), theObject);
}

void NaiveDoc_ObjectTable::RemoveObject(const Handle(NaiveDoc_Object) &
                                        theObject) {
  myObjects.remove(theObject->Id());
}
