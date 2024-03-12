#include "NaiveDoc_Document.hxx"
#include <Mesh/Mesh_MeshVSDriver.hxx>

IMPLEMENT_STANDARD_RTTIEXT(NaiveDoc_Document, LODoc_Document)

NaiveDoc_Document::NaiveDoc_Document() : QObject(), LODoc_Document() {
  initDriverID();
}

void NaiveDoc_Document::AddObjectEvent() { OnAddObject(this); }

void NaiveDoc_Document::initDriverID() {
  myDriverIDMap[LODoc_IMeshDriver] = Mesh_MeshVSDriver::GetID();
}
