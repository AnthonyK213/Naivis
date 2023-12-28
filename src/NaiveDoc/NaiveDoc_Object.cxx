#include "NaiveDoc_Object.hxx"

IMPLEMENT_STANDARD_RTTIEXT(NaiveDoc_Object, Standard_Transient)

NaiveDoc_Object::NaiveDoc_Object(const Handle(AIS_InteractiveObject) &
                                 theObject)
    : myObject(theObject), myUuid(), myName() {}

NaiveDoc_Object::~NaiveDoc_Object() {}

const Handle(AIS_InteractiveObject) & NaiveDoc_Object::Object() const {
  return myObject;
}

const QUuid &NaiveDoc_Object::Id() const { return myUuid; }

void NaiveDoc_Object::SetId(const QUuid &theId) { myUuid = theId; }

void NaiveDoc_Object::SetId() { myUuid = QUuid::createUuid(); }

const QString &NaiveDoc_Object::Name() const { return myName; }

void NaiveDoc_Object::SetName(const QString &theName) { myName = theName; }
