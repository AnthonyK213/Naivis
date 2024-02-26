#ifndef _Naivis_NaiveApp_Settings_HeaderFile
#define _Naivis_NaiveApp_Settings_HeaderFile

#include <QSettings>
#include <QSharedPointer>

class NaiveApp_Settings {
public:
  NaiveApp_Settings(const QString &thePath);

  void Init(const QString &thePath);

  QVariant Value(const QString &theSection, const QString &theKey) const;

  void SetValue(const QString &theSection, const QString &theKey,
                const QVariant &theValue);

private:
  QSharedPointer<QSettings> mySettings;
};

#endif
