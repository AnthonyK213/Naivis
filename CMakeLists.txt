cmake_minimum_required(VERSION 3.5)

if(WIN32)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
endif()

list(APPEND CMAKE_PREFIX_PATH "$ENV{QT_DIR}/lib/cmake/Qt6/")
list(APPEND CMAKE_PREFIX_PATH "$ENV{NAIVECGL_DIR}/lib/cmake/naivecgl")

project(Naivis VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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
find_package(NaiveCGL REQUIRED)

message("NaiveCGL-Include-Dir: ${NaiveCGL_INCLUDE_DIRS}")
message("NaiveCGL-Lib-Dir: ${NaiveCGL_LIBRARY_DIRS}")

include_directories(
  ${OpenCASCADE_INCLUDE_DIR}
  ${NaiveCGL_INCLUDE_DIR}
  )

link_directories(
  ${OpenCASCADE_LIBRARY_DIR}
  ${NaiveCGL_LIBRARY_DIR}
  )

add_subdirectory(src)
