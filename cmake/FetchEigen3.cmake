include(FetchContent)

FetchContent_Declare(
  Eigen3
  URL https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz
  URL_HASH SHA256=8586084f71f9bde545ee7fa6d00288b264a2b7ac3607b974e54d13e7162c1c72
  SOURCE_DIR ${CMAKE_BINARY_DIR}/deps/Eigen3
  )

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
