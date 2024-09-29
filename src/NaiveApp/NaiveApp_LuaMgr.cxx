#include "NaiveApp_LuaMgr.hxx"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>

#include <luaocct/luaocct.h>

#include <Ext/Ext_Load.hxx>

#include <iostream>

NaiveApp_LuaMgr::NaiveApp_LuaMgr(QObject *theParent)
    : QObject(theParent), myL(nullptr) {}

NaiveApp_LuaMgr::~NaiveApp_LuaMgr() { lua_close(myL); }

bool NaiveApp_LuaMgr::Init() {
  myL = luaL_newstate();

  if (myL == nullptr) {
    std::cout << "Cannot create state: not enough memory\n";
    return false;
  }

  lua_gc(myL, LUA_GCSTOP, 0);
  luaL_openlibs(myL);
  luaopen_luaocct(myL);
  Ext_Load(myL);
  lua_gc(myL, LUA_GCRESTART, -1);

  auto rtp = QCoreApplication::applicationDirPath().toUtf8().toStdString();
  PathAppend(rtp + "/runtime/lua");
  CPathAppend(rtp);

  return true;
}

lua_State *NaiveApp_LuaMgr::L() const { return myL; }

const QString &NaiveApp_LuaMgr::File() const { return myFile; }

bool NaiveApp_LuaMgr::SetFile(const QString &theFile) {
  myFile = theFile;

  /* WORKAROUND: Add the file directory to the runtime path. */
  QFileInfo info(myFile);
  auto rtp = info.dir().absolutePath().toUtf8().toStdString();
  PathAppend(rtp);
  CPathAppend(rtp);

  return true;
}

bool NaiveApp_LuaMgr::DoString(const std::string &theCode,
                               std::string &theErr) {
  if (luaL_dostring(myL, theCode.c_str()) != 0) {
    theErr = lua_tostring(myL, -1);
    lua_pop(myL, -1);
    return false;
  }

  return true;
}

bool NaiveApp_LuaMgr::DoFile(std::string &theErr) {
  return DoFile(myFile.toUtf8().toStdString(), theErr);
}

bool NaiveApp_LuaMgr::DoFile(const std::string &theFile, std::string &theErr) {
  if (luaL_dofile(myL, theFile.c_str()) != 0) {
    theErr = lua_tostring(myL, -1);
    lua_pop(myL, -1);
    return false;
  }

  return true;
}

bool NaiveApp_LuaMgr::PathAppend(const std::string &thePath,
                                 bool theToComplete) {
  lua_getglobal(myL, "package");
  lua_getfield(myL, -1, "path");
  std::string path = lua_tostring(myL, -1);
  if (!path.empty())
    path.append(";");
  path.append(thePath);
  if (theToComplete)
    path.append("/?.lua");
  lua_pop(myL, 1);
  lua_pushstring(myL, path.c_str());
  lua_setfield(myL, -2, "path");
  lua_pop(myL, 1);

  return true;
}

bool NaiveApp_LuaMgr::CPathAppend(const std::string &thePath,
                                  bool theToComplete) {
  lua_getglobal(myL, "package");
  lua_getfield(myL, -1, "cpath");
  std::string path = lua_tostring(myL, -1);
  if (!path.empty())
    path.append(";");
  path.append(thePath);
  if (theToComplete) {
#ifdef _WIN32
    path.append("/?.dll");
#else
    path.append("/?.so");
#endif
  }
  lua_pop(myL, 1);
  lua_pushstring(myL, path.c_str());
  lua_setfield(myL, -2, "cpath");
  lua_pop(myL, 1);

  return true;
}
