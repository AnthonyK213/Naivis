#include "Ext_Bind.hxx"

#include <NaiveApp/NaiveApp_Extension.hxx>
#include <NaiveApp/NaiveApp_ExtensionMgr.hxx>
#include <NaiveApp/NaiveApp_Settings.hxx>

void Ext_NaiveApp(lua_State *L) {
  LuaBridge__G(L)
      .Begin_Namespace(Naivis)
      .Begin_Namespace(NaiveApp)

      .Begin_Class(NaiveApp_Settings)
      .Bind_Method(NaiveApp_Settings, Value)
      .Bind_Method(NaiveApp_Settings, SetValue)
      .End_Class()

      .Begin_Class(NaiveApp_Extension)
      .Bind_Method(NaiveApp_Extension, IsValid)
      .Bind_Method(NaiveApp_Extension, IsLoaded)
      .Bind_Method(NaiveApp_Extension, Path)
      .Bind_Method(NaiveApp_Extension, Name)
      .End_Class()

      .Begin_Class(NaiveApp_ExtensionMgr)
      .Bind_Method(NaiveApp_ExtensionMgr, Find)
      .End_Class()

      .End_Namespace()
      .End_Namespace();
}
