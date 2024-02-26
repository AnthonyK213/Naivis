#include "Ext_Qt.hxx"

#include <QVariant>

void Ext_Qt(lua_State *L) {
  LuaBridge__G(L)
      .Begin_Namespace(Qt)

      .Begin_Namespace(QUuid_StringFormat)
      .Bind_Enum(QUuid, WithBraces)
      .Bind_Enum(QUuid, WithoutBraces)
      .Bind_Enum(QUuid, Id128)
      .End_Namespace()

      .Begin_Class(QUuid)
      .addConstructor<void()>()
      .Bind_Method_Static(QUuid, createUuid)
      .Bind_Property_Readonly(QUuid, isNull)
      .addFunction(
          "toString", +[](const QUuid &self) { return self.toString(); },
          luabridge::overload<QUuid::StringFormat>(&QUuid::toString))
      .addFunction(
          "__tostring", +[](const QUuid &self) { return self.toString(); })
      .End_Class()

      .Begin_Class(QVariant)
      .addConstructor<void(), void(QUuid), void(const QString &), void(bool),
                      void(double), void(float), void(int)>()
      .Bind_Method(QVariant, clear)
      .Bind_Method(QVariant, isNull)
      .Bind_Method(QVariant, isValid)
      .Bind_Method(QVariant, swap)
      .addFunction(
          "setHash", +[](QVariant &self,
                         const QVariantHash &hash) { self.setValue(hash); })
      .addFunction(
          "setList", +[](QVariant &self,
                         const QVariantList &list) { self.setValue(list); })
      .addFunction(
          "setString",
          +[](QVariant &self, const QString &str) { self.setValue(str); })
      .Bind_Method(QVariant, toBool)
      .addFunction(
          "toDouble",
          +[](const QVariant &self) -> std::tuple<bool, double> {
            bool ok = false;
            auto result = self.toDouble(&ok);
            return {ok, result};
          })
      .addFunction(
          "toFloat",
          +[](const QVariant &self) -> std::tuple<bool, float> {
            bool ok = false;
            auto result = self.toFloat(&ok);
            return {ok, result};
          })
      .addFunction(
          "toInt",
          +[](const QVariant &self) -> std::tuple<bool, int> {
            bool ok = false;
            auto result = self.toInt(&ok);
            return {ok, result};
          })
      .Bind_Method(QVariant, toHash)
      .Bind_Method(QVariant, toList)
      .Bind_Method(QVariant, toString)
      .Bind_Method(QVariant, toUuid)
      .End_Class()

      .End_Namespace();
}
