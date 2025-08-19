include(ExternalProject)

ExternalProject_Add(
  luv
  GIT_REPOSITORY https://github.com/luvit/luv.git
  GIT_TAG origin/master
  # GIT_SHALLOW 1
  INSTALL_COMMAND ""
  )

ExternalProject_Get_property(luv SOURCE_DIR BINARY_DIR)
set(luv_SOURCE_DIR ${SOURCE_DIR})
set(luv_BINARY_DIR ${BINARY_DIR})
message("luv_SOURCE_DIR: ${luv_SOURCE_DIR}")
message("luv_BINARY_DIR: ${luv_BINARY_DIR}")
