include(FetchContent)

if(DEFINED ENV{NAIVECGL_DIR})
  FetchContent_Declare(
    NaiveCGL
    SOURCE_DIR $ENV{NAIVECGL_DIR}
    )
else()
  FetchContent_Declare(
    NaiveCGL
    GIT_REPOSITORY https://github.com/AnthonyK213/NaiveCGL
    GIT_TAG origin/main
    SOURCE_DIR ${CMAKE_BINARY_DIR}/deps/NaiveCGL
    )
endif()

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
