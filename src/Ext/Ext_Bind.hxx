#ifndef _Naivis_Ext_Bind_HeaderFile
#define _Naivis_Ext_Bind_HeaderFile

#include <QList>
#include <QString>
#include <QUuid>

#include <luaocct/lbind.h>

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

template <class T> struct Stack<QList<T>> {
  static Result push(lua_State *L, const QList<T> &array1) {
    const int init_stack_size = lua_gettop(L);

    lua_createtable(L, array1.size(), 0);

    for (qsizetype i = 0; i < array1.size(); ++i) {
      lua_pushinteger(L, static_cast<lua_Integer>(i + 1));

      auto result = Stack<T>::push(L, array1.at(i));

      if (!result) {
        lua_pop(L, lua_gettop(L) - init_stack_size);
        return result;
      }

      lua_settable(L, -3);
    }

    return {};
  }

  static TypeResult<QList<T>> get(lua_State *L, int index) {
    if (!lua_istable(L, index)) {
      return makeErrorCode(ErrorCode::InvalidTypeCast);
    }

    const int init_stack_size = lua_gettop(L);

    QList<T> array1{};
    array1.reserve(get_length(L, index));

    const int abs_index = lua_absindex(L, index);

    lua_pushnil(L);

    while (lua_next(L, abs_index) != 0) {
      auto item = Stack<T>::get(L, -1);

      if (!item) {
        lua_pop(L, lua_gettop(L) - init_stack_size);
        return item.error();
      }

      array1.push_back(*item);

      lua_pop(L, 1);
    }

    return array1;
  }
};

template <>
struct Stack<QUuid::StringFormat>
    : Enum<QUuid::StringFormat, QUuid::StringFormat::WithBraces,
           QUuid::StringFormat::WithoutBraces, QUuid::StringFormat::Id128> {};

} // namespace luabridge

#endif
