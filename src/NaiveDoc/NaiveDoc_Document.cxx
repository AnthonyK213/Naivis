#include "NaiveDoc_Document.hxx"
#include <Mesh/Mesh_MeshVSDriver.hxx>

IMPLEMENT_STANDARD_RTTIEXT(NaiveDoc_Document, LODoc_Document)

NaiveDoc_Document::NaiveDoc_Document() : LODoc_Document() {
  initDriverID();
  // myObjects = new NaiveDoc_ObjectTable(this);
}

void NaiveDoc_Document::AddObjectEvent(const Handle(LODoc_Document) & theDoc) {
  if (auto aDoc = Handle(NaiveDoc_Document)::DownCast(theDoc)) {
    OnAddObject(aDoc);
  }
}

void NaiveDoc_Document::initDriverID() {
  myDriverIDMap[LODoc_IMeshDriver] = Mesh_MeshVSDriver::GetID();
}
