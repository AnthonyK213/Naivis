include(FetchContent)

FetchContent_Declare(
  luv
  URL https://github.com/luvit/luv/releases/download/1.48.0-2/luv-1.48.0-2.tar.gz
  URL_HASH SHA256=2c3a1ddfebb4f6550293a40ee789f7122e97647eede51511f57203de48c03b7a
  SOURCE_DIR ${CMAKE_BINARY_DIR}/deps/luv
  )

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
