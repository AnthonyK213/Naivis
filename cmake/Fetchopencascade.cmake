include(FetchContent)

FetchContent_Declare(
  opencascade
  URL https://github.com/Open-Cascade-SAS/OCCT/archive/refs/tags/V7_6_0.zip
  URL_HASH SHA256=73f8f71be02c1d9a977e854bcc9202502184d339f1f212c19797a5f23baefabe
  SOURCE_DIR ${CMAKE_BINARY_DIR}/deps/opencascade
  )

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
