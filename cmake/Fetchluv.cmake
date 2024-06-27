include(ExternalProject)

ExternalProject_Add(
  luv
  GIT_REPOSITORY https://github.com/luvit/luv.git
  GIT_TAG 572e67ad9300cd5b29d5f50a5507e35ec73c0d23
  INSTALL_COMMAND ""
  )

ExternalProject_Get_property(luv SOURCE_DIR BINARY_DIR)
set(luv_SOURCE_DIR ${SOURCE_DIR})
set(luv_BINARY_DIR ${BINARY_DIR})
message("luv_SOURCE_DIR: ${luv_SOURCE_DIR}")
message("luv_BINARY_DIR: ${luv_BINARY_DIR}")
