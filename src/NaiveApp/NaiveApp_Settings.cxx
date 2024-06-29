#include "NaiveApp_Settings.hxx"

#include <QFile>
#include <QHash>

NaiveApp_Settings::NaiveApp_Settings(const QString &thePath, QObject *theParent)
    : QObject(theParent) {
  Init(thePath);
}

void NaiveApp_Settings::Init(const QString &thePath) {
  mySettings = new QSettings(thePath, QSettings::IniFormat, this);

  QFile aFile(thePath);
  if (!aFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    mySettings->beginGroup("script_editor");
    mySettings->setValue("font", "monospace");
    mySettings->setValue("font_size", 10);
    mySettings->endGroup();
  }
}

QVariant NaiveApp_Settings::Value(const QString &theSection,
                                  const QString &theKey) const {
  QString aPath = QString("%1/%2").arg(theSection).arg(theKey);
  return mySettings->value(aPath);
}

void NaiveApp_Settings::SetValue(const QString &theSection,
                                 const QString &theKey,
                                 const QVariant &theValue) {
  mySettings->beginGroup(theSection);
  mySettings->setValue(theKey, theValue);
  mySettings->endGroup();
}
