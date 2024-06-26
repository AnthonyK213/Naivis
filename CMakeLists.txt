cmake_minimum_required(VERSION 3.20)

list(APPEND CMAKE_PREFIX_PATH "$ENV{QT_DIR}/lib/cmake/Qt6/")
list(APPEND CMAKE_PREFIX_PATH "$ENV{OCCT_DIR}")

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

option(LUAOCCT_BINDER_SUPPRESS_REGENERATION "Suppress regeneration of LuaOCCT-binder" ON)
option(ENABLE_UNIT_TESTS "" OFF)

include(cmake/FetchLuaOCCT.cmake)
include(cmake/FetchBinder.cmake)
include(cmake/FetchLuaJIT.cmake)
include(cmake/FetchLuaBridge3.cmake)
include(cmake/FetchNaiveCGL.cmake)
include(cmake/FetchEigen3.cmake)
include(cmake/Fetchluv.cmake)
include(cmake/FetchLuaPackages.cmake)

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

find_package(OpenGL REQUIRED)
message("OPENGL_LIBRARIES: ${OPENGL_LIBRARIES}")

find_package(OpenCASCADE COMPONENTS FoundationClasses REQUIRED)
find_package(OpenCASCADE COMPONENTS ModelingAlgorithms REQUIRED)
find_package(OpenCASCADE COMPONENTS ModelingData REQUIRED)
find_package(OpenCASCADE COMPONENTS DataExchange REQUIRED)
find_package(OpenCASCADE COMPONENTS Visualization REQUIRED)
message("OpenCASCADE_INCLUDE_DIR: ${OpenCASCADE_INCLUDE_DIR}")
message("OpenCASCADE_LIBRARY_DIR: ${OpenCASCADE_LIBRARY_DIR}")
message("OpenCASCADE_LIBRARIES: ${OpenCASCADE_LIBRARIES}")

message("naivecgl_SOURCE_DIR: ${naivecgl_SOURCE_DIR}")
message("naivecgl_BINARY_DIR: ${naivecgl_BINARY_DIR}")

message("eigen3_SOURCE_DIR: ${eigen3_SOURCE_DIR}")
message("eigen3_BINARY_DIR: ${eigen3_BINARY_DIR}")

message("luabridge3_SOURCE_DIR: ${luabridge3_SOURCE_DIR}")
message("luabridge3_BINARY_DIR: ${luabridge3_BINARY_DIR}")

message("luaocct_SOURCE_DIR: ${luaocct_SOURCE_DIR}")
message("luaocct_BINARY_DIR: ${luaocct_BINARY_DIR}")

include_directories(
  ${OpenCASCADE_INCLUDE_DIR}
  ${eigen3_SOURCE_DIR}
  ${luajit_BINARY_DIR}
  ${luabridge3_SOURCE_DIR}/Source
  ${luaocct_SOURCE_DIR}/include
  )

link_directories(
  ${OpenCASCADE_LIBRARY_DIR}
  )

add_subdirectory(src)
