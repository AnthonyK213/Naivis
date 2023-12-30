#ifndef _Naivis_Ext_Bind_HeaderFile
#define _Naivis_Ext_Bind_HeaderFile

#include <QString>
#include <QUuid>

#include <LuaOCCT/lbind.h>

namespace luabridge {

template <> struct Stack<QString> {
  static Result push(lua_State *L, const QString &s) {
    lua_pushstring(L, s.toUtf8().toStdString().c_str());
    return {};
  }

  static TypeResult<QString> get(lua_State *L, int index) {
    if (lua_type(L, index) != LUA_TSTRING) {
      return makeErrorCode(ErrorCode::InvalidTypeCast);
    }

    std::size_t length = 0;
    const char *str = lua_tolstring(L, index, &length);

    if (str == nullptr) {
      return makeErrorCode(ErrorCode::InvalidTypeCast);
    }

    return QString::fromUtf8(str);
  }

  static bool isInstance(lua_State *L, int index) {
    return lua_type(L, index) == LUA_TSTRING;
  }
};

template <>
struct Stack<QUuid::StringFormat>
    : Enum<QUuid::StringFormat, QUuid::StringFormat::WithBraces,
           QUuid::StringFormat::WithoutBraces, QUuid::StringFormat::Id128> {};

} // namespace luabridge

#endif
