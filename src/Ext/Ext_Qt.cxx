#include "Ext_Bind.hxx"

#include <QVariant>

#define Naivis_QVARIANT_FROM(T, N)                                             \
  addStaticFunction(                                                           \
      "from" #N, +[](const T &v) { return QVariant(v); })                      \
      .addFunction(                                                            \
          "set" #N, +[](QVariant &self, const T &v) { self.setValue(v); })

#define Naivis_QVARIANT_TO(T, N)                                               \
  addFunction(                                                                 \
      "to" #N, +[](const QVariant &self) -> std::tuple<bool, T> {              \
        bool ok = false;                                                       \
        T result = self.to##N(&ok);                                            \
        return {ok, result};                                                   \
      })

void Ext_Qt(lua_State *L) {
  LuaBridge__G(L)
      .Begin_Namespace(Qt)

      .Begin_Namespace(QUuid_StringFormat)
      .Bind_Enum(QUuid, WithBraces)
      .Bind_Enum(QUuid, WithoutBraces)
      .Bind_Enum(QUuid, Id128)
      .End_Namespace()

      .Begin_Class(QUuid)
      .addConstructor<void(), void(const QUuid &)>()
      .Bind_Method_Static(QUuid, createUuid)
      .Bind_Method(QUuid, isNull)
      .addFunction(
          "toString", +[](const QUuid &self) { return self.toString(); },
          luabridge::overload<QUuid::StringFormat>(&QUuid::toString))
      .End_Class()

      .Begin_Class(QVariant)
      .addConstructor<void(), void(const QVariant &)>()
      .Bind_Method(QVariant, clear)
      .Bind_Method(QVariant, isNull)
      .Bind_Method(QVariant, isValid)
      .Bind_Method(QVariant, swap)
      .Naivis_QVARIANT_FROM(bool, Bool)
      .Naivis_QVARIANT_FROM(int, Int)
      .Naivis_QVARIANT_FROM(float, Float)
      .Naivis_QVARIANT_FROM(double, Double)
      .Naivis_QVARIANT_FROM(QUuid, Uuid)
      .Naivis_QVARIANT_FROM(QString, String)
      .Naivis_QVARIANT_FROM(QVariantList, List)
      .Naivis_QVARIANT_FROM(QVariantHash, Hash)
      .Bind_Method(QVariant, toBool)
      .Naivis_QVARIANT_TO(int, Int)
      .Naivis_QVARIANT_TO(float, Float)
      .Naivis_QVARIANT_TO(double, Double)
      .Bind_Method(QVariant, toHash)
      .Bind_Method(QVariant, toList)
      .Bind_Method(QVariant, toString)
      .Bind_Method(QVariant, toUuid)
      .End_Class()

      .End_Namespace();
}
