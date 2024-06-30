#ifndef _Naivis_NaiveApp_LuaMgr_HeaderFile
#define _Naivis_NaiveApp_LuaMgr_HeaderFile

#include <QObject>
#include <QString>

struct lua_State;

class NaiveApp_LuaMgr : public QObject {
  Q_OBJECT

public:
  NaiveApp_LuaMgr(QObject *theParent = nullptr);

  ~NaiveApp_LuaMgr();

  bool Init();

  lua_State *L() const;

  const QString &File() const;

  bool SetFile(const QString &theFile);

  bool DoString(const std::string &theCode, std::string &theErr);

  bool DoFile(std::string &theErr);

  bool DoFile(const std::string &theFile, std::string &theErr);

  bool PathAppend(const std::string &thePath, bool theToComplete = true);

  bool CPathAppend(const std::string &thePath, bool theToComplete = true);

private:
  lua_State *myL;
  QString myFile;
};

#endif
