include(FetchContent)

if(DEFINED ENV{LUAOCCT_DIR})
  FetchContent_Declare(
    LuaOCCT
    SOURCE_DIR $ENV{LUAOCCT_DIR}
    )
else()
  FetchContent_Declare(
    LuaOCCT
    GIT_REPOSITORY https://github.com/AnthonyK213/LuaOCCT
    GIT_TAG origin/main
    GIT_SHALLOW 1
    SOURCE_DIR ${CMAKE_BINARY_DIR}/deps/LuaOCCT
    )
endif()

# FetchContent_MakeAvailable(LuaOCCT)
FetchContent_GetProperties(luaocct)
if(NOT luaocct_POPULATED)
  FetchContent_Populate(LuaOCCT)
  add_subdirectory(
    ${luaocct_SOURCE_DIR}
    ${luaocct_BINARY_DIR}
    EXCLUDE_FROM_ALL
    )
endif()
