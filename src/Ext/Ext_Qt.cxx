#include "Ext_Qt.hxx"

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
      .Bind_Method(QUuid, toString)
      .Bind_Property_Readonly(QUuid, isNull)
      .End_Class()

      .End_Namespace();
}
