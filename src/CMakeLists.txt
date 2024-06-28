aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} NAIVIS_SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Ext NAIVIS_EXT_SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Ghost NAIVIS_GHOST_SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/IO NAIVIS_IO_SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Mesh NAIVIS_MESH_SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/NaiveApp NAIVIS_NAIVEAPP_SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/NaiveDoc NAIVIS_NAIVEDOC_SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Sketch NAIVIS_SKETCH_SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Util NAIVIS_UTIL_SRC)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Widget NAIVIS_WIDGET_SRC)

set(
  PROJECT_SOURCES
  ${NAIVIS_SRC}
  ${NAIVIS_EXT_SRC}
  ${NAIVIS_GHOST_SRC}
  ${NAIVIS_IO_SRC}
  ${NAIVIS_MESH_SRC}
  ${NAIVIS_NAIVEAPP_SRC}
  ${NAIVIS_NAIVEDOC_SRC}
  ${NAIVIS_SKETCH_SRC}
  ${NAIVIS_UTIL_SRC}
  ${NAIVIS_WIDGET_SRC}
  ${CMAKE_CURRENT_SOURCE_DIR}/Widget/naivis.ui
  )

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(Naivis
    MANUAL_FINALIZATION
    WIN32 ${CMAKE_SOURCE_DIR}/resources/icon.rc
    ${PROJECT_SOURCES}
    ${CMAKE_SOURCE_DIR}/resources/res.qrc
    )
  # Define target properties for Android with Qt 6 as:
  #    set_property(TARGET Naivis APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
  #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
  # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
  if(ANDROID)
    add_library(Naivis SHARED
      ${PROJECT_SOURCES}
      )
    # Define properties for Android with Qt 5 after find_package() calls as:
    #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
  else()
    add_executable(Naivis
      ${PROJECT_SOURCES}
      )
  endif()
endif()

target_include_directories(
  Naivis
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

if(WIN32)
  target_link_libraries(
    Naivis
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::OpenGLWidgets
    Qt${QT_VERSION_MAJOR}::OpenGL
    ${OpenCASCADE_LIBRARIES}
    ${OPENGL_LIBRARIES}
    luaocct
    luajit::lib
    luajit::header
    )
else(WIN32)
  target_link_libraries(
    Naivis
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::OpenGLWidgets
    Qt${QT_VERSION_MAJOR}::OpenGL
    ${OpenCASCADE_LIBRARIES}
    ${OPENGL_LIBRARIES}
    luaocct
    -Wl,-E
    luajit::lib
    luajit::header
    )
endif(WIN32)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Naivis)
endif()
set_target_properties(Naivis PROPERTIES
  ${BUNDLE_ID_OPTION}
  MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
  MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
  MACOSX_BUNDLE TRUE
  WIN32_EXECUTABLE TRUE
  )

include(GNUInstallDirs)
install(TARGETS Naivis
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

add_custom_command(
  TARGET Naivis POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
      "$<TARGET_PROPERTY:Naivis,SOURCE_DIR>/../runtime"
      "$<TARGET_PROPERTY:Naivis,BINARY_DIR>/$<CONFIG>/runtime"
  COMMAND ${CMAKE_COMMAND} -E copy
      "${lua_inspect_SOURCE_DIR}/inspect.lua"
      "$<TARGET_PROPERTY:Naivis,BINARY_DIR>/$<CONFIG>/runtime/lua"
  COMMAND ${CMAKE_COMMAND} -E copy
      "${lua_json_SOURCE_DIR}/json.lua"
      "$<TARGET_PROPERTY:Naivis,BINARY_DIR>/$<CONFIG>/runtime/lua"
  COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${CMAKE_BINARY_DIR}/binder-gen/_meta"
      "$<TARGET_PROPERTY:Naivis,BINARY_DIR>/$<CONFIG>/_meta"
  )

if(WIN32)
  add_custom_command(
    TARGET Naivis POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${luv_BINARY_DIR}/$<CONFIG>/luv.dll"
        "$<TARGET_PROPERTY:Naivis,BINARY_DIR>/$<CONFIG>"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${luaocct_BINARY_DIR}/src/$<CONFIG>"
        "$<TARGET_PROPERTY:Naivis,BINARY_DIR>/$<CONFIG>"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${OpenCASCADE_LIBRARY_DIR}/../bin$<$<CONFIG:Debug>:d>"
        "$<TARGET_PROPERTY:Naivis,BINARY_DIR>/$<CONFIG>"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "$<TARGET_PROPERTY:libluajit,BINARY_DIR>/$<CONFIG>"
        "$<TARGET_PROPERTY:Naivis,BINARY_DIR>/$<CONFIG>"
    COMMAND $ENV{QT_DIR}/bin/windeployqt.exe
        $<TARGET_PROPERTY:Naivis,BINARY_DIR>/$<CONFIG>/Naivis.exe
    )
elseif(UNIX AND NOT APPLE)
  add_custom_command(
    TARGET Naivis POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        "${CMAKE_INSTALL_PREFIX}/bin/"
    COMMAND ${CMAKE_COMMAND} -E make_directory
        "${CMAKE_INSTALL_PREFIX}/lib/"
    COMMAND ${CMAKE_COMMAND} -E copy
        "$ENV{OCCT_DIR}/lib/*.so*"
        "${CMAKE_INSTALL_PREFIX}/lib"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${luv_BINARY_DIR}/$<CONFIG>/luv.so"
        "$<TARGET_PROPERTY:Naivis,BINARY_DIR>/$<CONFIG>"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${luv_BINARY_DIR}/$<CONFIG>/luv.so"
        "${CMAKE_INSTALL_PREFIX}/bin"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${luaocct_BINARY_DIR}/src/$<CONFIG>/*.so"
        "${CMAKE_INSTALL_PREFIX}/lib"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "$<TARGET_PROPERTY:Naivis,SOURCE_DIR>/../runtime"
        "${CMAKE_INSTALL_PREFIX}/bin/runtime"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${lua_inspect_SOURCE_DIR}/inspect.lua"
        "${CMAKE_INSTALL_PREFIX}/bin/runtime/lua"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${lua_json_SOURCE_DIR}/json.lua"
        "${CMAKE_INSTALL_PREFIX}/bin/runtime/lua"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_BINARY_DIR}/binder-gen/_meta"
        "${CMAKE_INSTALL_PREFIX}/bin/_meta"
  )
  install(SCRIPT "${CMAKE_SOURCE_DIR}/cmake/PostInstall-Linux.cmake")
endif()

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(Naivis)
endif()
