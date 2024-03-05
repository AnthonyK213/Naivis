cmake_minimum_required(VERSION 3.20)

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

if(UNIX)
list(APPEND CMAKE_PREFIX_PATH "$ENV{OCCT_DIR}/lib/cmake/opencascade")
endif()

list(APPEND CMAKE_PREFIX_PATH "$ENV{QT_DIR}/lib/cmake/Qt6/")
list(APPEND CMAKE_PREFIX_PATH "$ENV{LUAOCCT_DIR}/lib/cmake/luaocct")

project(Naivis VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-${PROJECT_VERSION}/)

add_definitions(-D_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)

include(cmake/FetchDeps.cmake)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS
  Core
  Gui
  Widgets
  OpenGLWidgets
  OpenGL
  )

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
  Core
  Gui
  Widgets
  OpenGLWidgets
  OpenGL
  )

find_package(OpenCASCADE COMPONENTS FoundationClasses REQUIRED)
find_package(OpenCASCADE COMPONENTS ModelingAlgorithms REQUIRED)
find_package(OpenCASCADE COMPONENTS ModelingData REQUIRED)
find_package(OpenCASCADE COMPONENTS DataExchange REQUIRED)
find_package(OpenCASCADE COMPONENTS Visualization REQUIRED)
message("OCC-Include-Dir: ${OpenCASCADE_INCLUDE_DIR}")
message("OCC-Lib-Dir: ${OpenCASCADE_LIBRARY_DIR}")
message("OCC-Libs: ${OpenCASCADE_LIBRARIES}")

find_package(Eigen3 REQUIRED)

message("NaiveCGL-Source-Dir: ${naivecgl_SOURCE_DIR}")
message("NaiveCGL-Binary-Dir: ${naivecgl_BINARY_DIR}")

find_package(PkgConfig REQUIRED)  
pkg_check_modules(LuaJIT REQUIRED IMPORTED_TARGET luajit)

find_path(LuaBridge3_INCLUDE_DIR NAMES LuaBridge PATH_SUFFIXES luabridge3)
message("LuaBridge3-Include-Dir: ${LuaBridge3_INCLUDE_DIR}")

find_package(luaocct REQUIRED)
message("LuaOCCT-Include-Dir: ${LuaOCCT_INCLUDE_DIR}")
message("LuaOCCT-Lib-Dir: ${LuaOCCT_LIBRARY_DIR}")
message("LuaOCCT-Libs: ${LuaOCCT_LIBRARIES}")

include_directories(
  ${OpenCASCADE_INCLUDE_DIR}
  ${naivecgl_SOURCE_DIR}/include
  ${EIGEN3_INCLUDE_DIRS}
  ${LuaBridge3_INCLUDE_DIR}
  ${LuaOCCT_INCLUDE_DIR}
  )

link_directories(
  ${OpenCASCADE_LIBRARY_DIR}
  ${LuaOCCT_LIBRARY_DIR}
  )

add_subdirectory(src)
