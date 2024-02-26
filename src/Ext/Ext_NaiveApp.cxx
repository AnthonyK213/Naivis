#include "Ext_NaiveApp.hxx"

#include <NaiveApp/NaiveApp_Settings.hxx>

void Ext_NaiveApp(lua_State *L) {
  LuaBridge__G(L)
      .Begin_Namespace(Naivis)
      .Begin_Namespace(NaiveApp)

      .Begin_Class(NaiveApp_Settings)
      .Bind_Method(NaiveApp_Settings, Value)
      .Bind_Method(NaiveApp_Settings, SetValue)
      .End_Class()

      .End_Namespace()
      .End_Namespace();
}
