#ifndef _Naivis_Ext_Bind_HeaderFile
#define _Naivis_Ext_Bind_HeaderFile

#include <QHash>
#include <QList>
#include <QString>
#include <QUuid>

#include <luaocct_bind/lbind.h>

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

template <class K, class V> struct Stack<QHash<K, V>> {
  static Result push(lua_State *L, const QHash<K, V> &hash) {
    const int init_stack_size = lua_gettop(L);

    lua_createtable(L, 0, hash.size());

    for (auto it = hash.cbegin(); it != hash.cend(); ++it) {
      auto key = Stack<K>::push(L, it.key());
      if (!key) {
        lua_pop(L, lua_gettop(L) - init_stack_size);
        return key;
      }

      auto value = Stack<V>::push(L, it.value());
      if (!value) {
        lua_pop(L, lua_gettop(L) - init_stack_size);
        return value;
      }

      lua_settable(L, -3);
    }

    return {};
  }

  static TypeResult<QHash<K, V>> get(lua_State *L, int index) {
    if (!lua_istable(L, index)) {
      return makeErrorCode(ErrorCode::InvalidTypeCast);
    }

    const int init_stack_size = lua_gettop(L);
    QHash<K, V> hash{};
    const int abs_index = lua_absindex(L, index);
    lua_pushnil(L);

    while (lua_next(L, abs_index) != 0) {
      auto value = Stack<V>::get(L, -1);
      if (!value) {
        lua_pop(L, lua_gettop(L) - init_stack_size);
        return value.error();
      }

      auto key = Stack<K>::get(L, -2);
      if (!key) {
        lua_pop(L, lua_gettop(L) - init_stack_size);
        return key.error();
      }

      hash.insert(*key, *value);

      lua_pop(L, 1);
    }

    return hash;
  }
};

template <>
struct Stack<QUuid::StringFormat>
    : Enum<QUuid::StringFormat, QUuid::StringFormat::WithBraces,
           QUuid::StringFormat::WithoutBraces, QUuid::StringFormat::Id128> {};

} // namespace luabridge

#define LuaBridge__G(L) luabridge::getGlobalNamespace(L)

#define Begin_Namespace(N) beginNamespace(#N)
#define Begin_Namespace0() beginNamespace("LuaOCCT")
#define Begin_Namespace1(U) beginNamespace("LuaOCCT").beginNamespace(#U)
#define Begin_Namespace2(U, V)                                                 \
  beginNamespace("LuaOCCT").beginNamespace(#U).beginNamespace(#V)
#define End_Namespace() endNamespace()
#define End_Namespace0() endNamespace()
#define End_Namespace1() endNamespace().endNamespace()
#define End_Namespace2() endNamespace().endNamespace().endNamespace()

#define Begin_Class(T) beginClass<T>(#T)
#define End_Class() endClass()

#define Begin_Derive(D, B) deriveClass<D, B>(#D)
#define End_Derive() End_Class()

#define Bind_Enum(E, V) addProperty(#V, +[]() { return E::V; })

#define Bind_Property(T, G, S)                                                 \
  addProperty(#G "_", &T::G, &T::S)                                            \
      .addFunction(#G, &T::G)                                                  \
      .addFunction(#S, &T::S)
#define Bind_Property_Readonly(T, G)                                           \
  addProperty(#G "_", &T::G).addFunction(#G, &T::G)

#define Bind_Method(T, M) addFunction(#M, &T::M)
#define Bind_Method_Static(T, M) addStaticFunction(#M, &T::M)

#define Bind_DownCast(D)                                                       \
  addStaticFunction(                                                           \
      "DownCast", +[](const Handle(Standard_Transient) & h) -> Handle(D) {     \
        return Handle(D)::DownCast(h);                                         \
      })
#define Bind_DownCast1(D, B)                                                   \
  addStaticFunction(                                                           \
      "DownCast", +[](const Handle(B) & h) -> Handle(D) {                      \
        return Handle(D)::DownCast(h);                                         \
      })

#endif
