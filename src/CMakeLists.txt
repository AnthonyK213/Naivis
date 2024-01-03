include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(LuaOCCT)

set(PROJECT_SOURCES
  main.cpp
  Ext/Ext_Load.cxx
  Ext/Ext_NaiveDoc.cxx
  Ext/Ext_Override.cxx
  Ext/Ext_Qt.cxx
  IO/IO_LogStream.cxx
  Mesh/HalfEdgeMesh_DataSource.cxx
  Mesh/XSDRAWSTLVRML_DataSource_1.cxx
  Mesh/Mesh_MeshBuilder.cxx
  Mesh/Mesh_Util.cxx
  NaiveDoc/NaiveDoc_CmdAddObjects.cxx
  NaiveDoc/NaiveDoc_CmdRemoveObjects.cxx
  NaiveDoc/NaiveDoc_Document.cxx
  NaiveDoc/NaiveDoc_Object.cxx
  NaiveDoc/NaiveDoc_ObjectInfo.cxx
  NaiveDoc/NaiveDoc_ObjectTable.cxx
  Sketch/Sketch_Command.cxx
  Sketch/Sketch_Manager.cxx
  Widget/naivis.cpp
  Widget/naivis.ui
  Widget/Widget_OcctViewer.cxx
  )

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(Naivis
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
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
  ${NaiveCGL_LIBRARIES}
  ${LUA_LIBRARIES}
  luaocct
  )

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

if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(Naivis)
endif()

add_custom_command(
  TARGET Naivis POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
      "$<TARGET_PROPERTY:Naivis,SOURCE_DIR>/../lua"
      "$<TARGET_PROPERTY:Naivis,BINARY_DIR>/$<CONFIG>"
  )

if(WIN32)
  add_custom_command(
    TARGET Naivis POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "$ENV{NAIVECGL_DIR}/bin"
        "$<TARGET_PROPERTY:Naivis,BINARY_DIR>/$<CONFIG>"
    )
  add_custom_command(
    TARGET Naivis POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "$ENV{LUA_DIR}/bin/lua51.dll"
        "$<TARGET_PROPERTY:Naivis,BINARY_DIR>/$<CONFIG>"
    COMMAND ${CMAKE_COMMAND} -E copy
        "$ENV{LUA_DIR}/bin/lua51.dll"
        "$<TARGET_PROPERTY:Naivis,BINARY_DIR>/$<CONFIG>"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "$<TARGET_PROPERTY:luaocct,BINARY_DIR>/$<CONFIG>"
        "$<TARGET_PROPERTY:Naivis,BINARY_DIR>/$<CONFIG>"
    COMMAND $ENV{QT_DIR}/bin/windeployqt.exe
        $<TARGET_PROPERTY:Naivis,BINARY_DIR>/$<CONFIG>/Naivis.exe
    )
endif()
