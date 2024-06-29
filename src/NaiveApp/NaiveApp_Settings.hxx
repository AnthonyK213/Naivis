#ifndef _Naivis_NaiveApp_Settings_HeaderFile
#define _Naivis_NaiveApp_Settings_HeaderFile

#include <QSettings>

class NaiveApp_Settings : public QObject {
  Q_OBJECT

public:
  NaiveApp_Settings(const QString &thePath, QObject *theParent = nullptr);

  void Init(const QString &thePath);

  QVariant Value(const QString &theSection, const QString &theKey) const;

  void SetValue(const QString &theSection, const QString &theKey,
                const QVariant &theValue);

private:
  QSettings *mySettings;
};

#endif
