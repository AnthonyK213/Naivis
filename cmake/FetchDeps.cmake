include(FetchContent)

FetchContent_Declare(
  NaiveCGL
  GIT_REPOSITORY https://github.com/AnthonyK213/NaiveCGL
  GIT_TAG origin/main
  SOURCE_DIR ${CMAKE_BINARY_DIR}/deps/NaiveCGL
  )

FetchContent_MakeAvailable(NaiveCGL)
