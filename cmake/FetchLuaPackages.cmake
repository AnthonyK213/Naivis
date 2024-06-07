include(FetchContent)

FetchContent_Declare(
  lua_inspect
  GIT_REPOSITORY https://github.com/kikito/inspect.lua
  GIT_TAG origin/master
  SOURCE_DIR ${CMAKE_BINARY_DIR}/deps/lua_inspect
  )

FetchContent_Declare(
  lua_json
  GIT_REPOSITORY https://github.com/rxi/json.lua
  GIT_TAG origin/master
  SOURCE_DIR ${CMAKE_BINARY_DIR}/deps/lua_json
  )

FetchContent_GetProperties(lua_inspect)
if(NOT lua_inspcet_source_POPULATED)
  FetchContent_Populate(lua_inspect)
endif()

FetchContent_GetProperties(lua_json)
if(NOT lua_json_source_POPULATED)
  FetchContent_Populate(lua_json)
endif()
