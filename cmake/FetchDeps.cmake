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

FetchContent_Declare(
  Eigen3
  URL https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz
  URL_HASH SHA256=8586084f71f9bde545ee7fa6d00288b264a2b7ac3607b974e54d13e7162c1c72
  SOURCE_DIR ${CMAKE_BINARY_DIR}/deps/Eigen3
  )

FetchContent_Declare(
  LuaBridge3
  GIT_REPOSITORY https://github.com/kunitoki/LuaBridge3.git
  GIT_TAG origin/master
  SOURCE_DIR ${CMAKE_BINARY_DIR}/deps/LuaBridge3
  )

FetchContent_Declare(
  LuaJIT_Source
  GIT_REPOSITORY https://github.com/LuaJIT/LuaJIT
  GIT_TAG origin/v2.1
  SOURCE_DIR ${CMAKE_BINARY_DIR}/deps/LuaJIT_Source
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  )

FetchContent_Declare(
  LuaJIT
  GIT_REPOSITORY https://github.com/zhaozg/luajit-cmake
  GIT_TAG origin/master
  SOURCE_DIR ${CMAKE_BINARY_DIR}/deps/luajit-cmake
  )

FetchContent_Declare(
  luv
  URL https://github.com/luvit/luv/releases/download/1.48.0-2/luv-1.48.0-2.tar.gz
  URL_HASH SHA256=2c3a1ddfebb4f6550293a40ee789f7122e97647eede51511f57203de48c03b7a
  SOURCE_DIR ${CMAKE_BINARY_DIR}/deps/luv
  )

FetchContent_Declare(
  opencascade
  URL https://github.com/Open-Cascade-SAS/OCCT/archive/refs/tags/V7_6_0.zip
  URL_HASH SHA256=73f8f71be02c1d9a977e854bcc9202502184d339f1f212c19797a5f23baefabe
  SOURCE_DIR ${CMAKE_BINARY_DIR}/deps/opencascade
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

# FetchContent_MakeAvailable(Eigen3)
FetchContent_GetProperties(Eigen3)
if(NOT eigen3_POPULATED)
  set(BUILD_TESTING OFF)
  set(EIGEN_BUILD_TESTING OFF)
  set(EIGEN_MPL2_ONLY ON)
  set(EIGEN_BUILD_PKGCONFIG OFF)
  set(EIGEN_BUILD_DOC OFF)
  FetchContent_Populate(Eigen3)
  add_subdirectory(
    ${eigen3_SOURCE_DIR}
    ${eigen3_BINARY_DIR}
    EXCLUDE_FROM_ALL
    )
endif()

# FetchContent_MakeAvailable(LuaBridge3)
FetchContent_GetProperties(LuaBridge3)
if(NOT luabridge3_POPULATED)
  FetchContent_Populate(LuaBridge3)
  add_subdirectory(
    ${luabridge3_SOURCE_DIR}
    ${luabridge3_BINARY_DIR}
    EXCLUDE_FROM_ALL
    )
endif()

# FetchContent_MakeAvailable(LuaJIT_Source)
FetchContent_GetProperties(LuaJIT_Source)
if(NOT luajit_source_POPULATED)
  FetchContent_Populate(LuaJIT_Source)
endif()

# FetchContent_MakeAvailable(LuaJIT)
FetchContent_GetProperties(LuaJIT)
if(NOT luajit_POPULATED)
  FetchContent_Populate(LuaJIT)
  add_subdirectory(
    ${luajit_SOURCE_DIR}
    ${luajit_BINARY_DIR}
    EXCLUDE_FROM_ALL
    )

  target_include_directories(
    buildvm
    PRIVATE
    ${luajit_BINARY_DIR}
    )
endif()

# # FetchContent_MakeAvailable(luv)
# FetchContent_GetProperties(luv)
# if(NOT luv_POPULATED)
#   FetchContent_Populate(luv)
#   add_subdirectory(
#     ${luv_SOURCE_DIR}
#     ${luv_BINARY_DIR}
#     EXCLUDE_FROM_ALL
#     )
# endif()

# # FetchContent_MakeAvailable(opencascade)
# FetchContent_GetProperties(opencascade)
# if(NOT opencascade_POPULATED)
#   FetchContent_Populate(opencascade)
#   add_subdirectory(
#     ${opencascade_SOURCE_DIR}
#     ${opencascade_BINARY_DIR}
#     EXCLUDE_FROM_ALL
#     )
# endif()
