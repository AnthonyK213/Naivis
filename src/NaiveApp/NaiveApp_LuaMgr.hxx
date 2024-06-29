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

  operator lua_State *() { return myL; }

  lua_State *L() const;

  const QString &File() const;

  bool SetFile(const QString &theFile);

  bool DoString(const std::string &theCode, std::string &theErr);

  bool DoFile(std::string &theErr) const;

  bool DoFile(const std::string &theFile, std::string &theErr) const;

  bool PathAppend(const std::string &thePath, bool theToComplete = true) const;

  bool CPathAppend(const std::string &thePath, bool theToComplete = true) const;

private:
  lua_State *myL;
  QString myFile;
};

#endif
