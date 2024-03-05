include(FetchContent)

FetchContent_Declare(
  NaiveCGL
  GIT_REPOSITORY https://github.com/AnthonyK213/NaiveCGL
  GIT_TAG origin/main
  SOURCE_DIR ${CMAKE_BINARY_DIR}/deps/NaiveCGL
  )

FetchContent_Declare(
  LuaOCCT
  GIT_REPOSITORY https://github.com/AnthonyK213/LuaOCCT
  GIT_TAG origin/main
  SOURCE_DIR ${CMAKE_BINARY_DIR}/deps/LuaOCCT
  )

# FetchContent_MakeAvailable(NaiveCGL)
FetchContent_GetProperties(naivecgl)
if(NOT naivecgl_POPULATED)
  FetchContent_Populate(NaiveCGL)
  add_subdirectory(
    ${naivecgl_SOURCE_DIR}
    ${naivecgl_BINARY_DIR}
    EXCLUDE_FROM_ALL
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
